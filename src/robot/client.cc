#include "google/protobuf/struct.pb.h"
#define BOOST_LOG_DYN_LINK 1
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>
#include <rpc/rpc.h>
#include <unistd.h>

#include <boost/log/trivial.hpp>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <set>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "../../src/common/utils.h"
#include "../components/resource_manager.h"
#include "../components/service_base.h"
#include "../registry/registry.h"
#include "../rpc/dial.h"
#include "common/v1/common.pb.h"
#include "grpcpp/channel.h"

using google::protobuf::RepeatedPtrField;
using grpc::Channel;
using grpc::ClientContext;
using viam::common::v1::PoseInFrame;
using viam::common::v1::ResourceName;
using viam::common::v1::Transform;
using viam::robot::v1::Discovery;
using viam::robot::v1::DiscoveryQuery;
using viam::robot::v1::FrameSystemConfig;
using viam::robot::v1::Operation;
using viam::robot::v1::RobotService;
using viam::robot::v1::Status;

// gRPC client for a robot. This class should be used for all interactions with
// a robot.
class RobotClient {
       public:
	void refresh();
	void close();
	static std::shared_ptr<RobotClient> at_address(std::string address,
						       Options options);
	static std::shared_ptr<RobotClient> with_channel(ViamChannel channel,
							 Options options);
	std::vector<Status> get_status(
	    std::vector<ResourceName> components = std::vector<ResourceName>());
	std::vector<Operation> get_operations();
	std::vector<FrameSystemConfig> get_frame_system_config(
	    std::vector<Transform> additional_transforms =
		std::vector<Transform>());

	void stop_all(
	    std::vector<std::tuple<ResourceName,
				   std::unordered_map<std::string, ProtoType>>>
		extra);
	void cancel_operation(std::string id);
	void block_for_operation(std::string id);
	RobotClient(ViamChannel channel);
	std::vector<ResourceName> *resource_names();
	std::unique_ptr<RobotService::Stub> stub_;
	ComponentBase get_component(ResourceName name);
	PoseInFrame transform_pose(
	    PoseInFrame query, std::string destination,
	    std::vector<Transform> additional_transforms =
		std::vector<Transform>());
	std::vector<Discovery> discover_components(
	    std::vector<DiscoveryQuery> queries);

       private:
	std::atomic<bool> should_refresh;
	unsigned int refresh_interval;
	// (RSDK-919): make use of should_close_channel
	bool should_close_channel;
	std::shared_ptr<Channel> channel;
	ViamChannel viam_channel;
	std::mutex lock;
	std::vector<ResourceName> resource_names_;
	ResourceManager resource_manager;
	void refresh_every();
};

void RobotClient::close() {
	// CR erodkin: have this close all tasks
	should_refresh.store(false);
	viam_channel.close();
}

// gets Statuses of components associated with robot. If a specific component
// vector is provided, only statuses for the given ResourceNames will be
// returned
std::vector<Status> RobotClient::get_status(
    std::vector<ResourceName> components) {
	viam::robot::v1::GetStatusRequest req;
	viam::robot::v1::GetStatusResponse resp;
	ClientContext ctx;
	RepeatedPtrField<ResourceName> *request_resource_names =
	    req.mutable_resource_names();

	for (ResourceName name : components) {
		*request_resource_names->Add() = name;
	}

	grpc::Status response = stub_->GetStatus(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error)
		    << "Error getting status: " << response.error_message()
		    << response.error_details();
	}

	RepeatedPtrField<Status> status = resp.status();

	std::vector<Status> statuses = std::vector<Status>();

	for (Status s : status) {
		statuses.push_back(s);
	}

	return statuses;
}

std::vector<Operation> RobotClient::get_operations() {
	viam::robot::v1::GetOperationsRequest req;
	viam::robot::v1::GetOperationsResponse resp;
	ClientContext ctx;

	std::vector<Operation> operations;

	grpc::Status response = stub_->GetOperations(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error)
		    << "Error getting operations: " << response.error_message();
	}
	for (int i = 0; i < resp.operations().size(); ++i) {
		operations.push_back(resp.operations().at(i));
	}
	return operations;
}

void RobotClient::cancel_operation(std::string id) {
	viam::robot::v1::CancelOperationRequest req;
	viam::robot::v1::CancelOperationResponse resp;
	ClientContext ctx;

	req.set_id(id);
	grpc::Status response = stub_->CancelOperation(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error)
		    << "Error canceling operation with id " << id;
	}
}

void RobotClient::block_for_operation(std::string id) {
	viam::robot::v1::BlockForOperationRequest req;
	viam::robot::v1::BlockForOperationResponse resp;
	ClientContext ctx;

	req.set_id(id);

	grpc::Status response = stub_->BlockForOperation(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error)
		    << "Error blocking for operation with id " << id;
	}
}

void RobotClient::refresh() {
	viam::robot::v1::ResourceNamesRequest req;
	viam::robot::v1::ResourceNamesResponse resp;
	ClientContext ctx;
	grpc::Status response = stub_->ResourceNames(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error) << "Error getting resource names: "
					 << response.error_message();
	}

	ResourceManager new_resource_manager;
	RepeatedPtrField<ResourceName> resources = resp.resources();

	const int num_resources = resources.size();
	std::vector<ResourceName> current_resources;

	for (int i = 0; i < num_resources; i++) {
		ResourceName name = resources.at(i);
		current_resources.push_back(name);
		if (name.type() != "component") {
			continue;
		}
		if (name.subtype() == "remote") {
			continue;
		}

		try {
			ComponentBase rpc_client =
			    Registry::lookup(name.subtype())
				.create_rpc_client(name.name(), channel);
			new_resource_manager.register_component(rpc_client);
		} catch (std::exception &exc) {
			BOOST_LOG_TRIVIAL(debug)
			    << "Error registering component " << name.subtype()
			    << ": " << exc.what();
		}
	}
	// TODO: come up with a better equality check here. equality is
	// not defined for `const ResourceName` unfortunately, but this
	// is pretty awkward as is.
	bool is_equal = true;
	if (current_resources.size() != resource_names_.size()) {
		is_equal = false;
	}
	if (is_equal) {
		for (int i = 0; i < current_resources.size(); ++i) {
			ResourceName r1, r2;
			r1 = current_resources.at(i);
			r2 = resource_names_.at(i);
			if (r1.namespace_() != r2.namespace_()) {
				is_equal = false;
				break;
			}
		}
	};
	if (is_equal) {
		return;
	}

	lock.lock();
	resource_names_ = current_resources;
	lock.unlock();
}

void RobotClient::refresh_every() {
	while (should_refresh.load()) {
		std::this_thread::sleep_for(
		    std::chrono::seconds(refresh_interval));
		refresh();
	}
};

RobotClient::RobotClient(ViamChannel vc) : viam_channel(vc) {
	should_close_channel = false;
	stub_ = std::move(RobotService::NewStub(vc.channel));
	channel = vc.channel;
}

std::vector<ResourceName> *RobotClient::resource_names() {
	lock.lock();
	return &resource_names_;
}

std::shared_ptr<RobotClient> RobotClient::with_channel(ViamChannel channel,
						       Options options) {
	std::unique_ptr<RobotService::Stub> s =
	    RobotService::NewStub(channel.channel);
	std::shared_ptr<RobotClient> robot =
	    std::make_shared<RobotClient>(channel);
	robot->refresh_interval = options.refresh_interval;
	robot->should_refresh = (robot->refresh_interval > 0);
	if (robot->should_refresh) {
		std::thread t(&RobotClient::refresh_every, robot);
		t.detach();
	};

	robot->refresh();
	return robot;
};

// Create a robot client that is connected to the robot at the provided
// address.
//
// Args:
// 		address: Address of the robot (IP address, URI, URL,
// etc.) 		options: Options for connecting and refreshing
std::shared_ptr<RobotClient> RobotClient::at_address(std::string address,
						     Options options) {
	const char *uri = address.c_str();
	ViamChannel channel = dial(uri, options.dial_options);
	std::shared_ptr<RobotClient> robot =
	    RobotClient::with_channel(channel, options);
	robot->should_close_channel = true;

	return robot;
};

std::vector<FrameSystemConfig> RobotClient::get_frame_system_config(
    std::vector<Transform> additional_transforms) {
	viam::robot::v1::FrameSystemConfigRequest req;
	viam::robot::v1::FrameSystemConfigResponse resp;
	ClientContext ctx;

	RepeatedPtrField<Transform> *req_transforms =
	    req.mutable_supplemental_transforms();
	// CR erodkin: test that this is right. why aren't we using the mutable
	// one?
	for (Transform transform : additional_transforms) {
		*req_transforms->Add() = transform;
	}

	grpc::Status response = stub_->FrameSystemConfig(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error)
		    << "Error getting frame system config: "
		    << response.error_message();
	}

	RepeatedPtrField<FrameSystemConfig> configs =
	    resp.frame_system_configs();

	std::vector<FrameSystemConfig> fs_configs =
	    std::vector<FrameSystemConfig>();

	for (FrameSystemConfig fs : configs) {
		fs_configs.push_back(fs);
	}

	return fs_configs;
}
PoseInFrame RobotClient::transform_pose(
    PoseInFrame query, std::string destination,
    std::vector<Transform> additional_transforms) {
	viam::robot::v1::TransformPoseRequest req;
	viam::robot::v1::TransformPoseResponse resp;
	ClientContext ctx;

	*req.mutable_source() = query;
	*req.mutable_destination() = destination;
	RepeatedPtrField<Transform> *req_transforms =
	    req.mutable_supplemental_transforms();

	for (Transform transform : additional_transforms) {
		*req_transforms->Add() = transform;
	}

	grpc::Status response = stub_->TransformPose(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error) << "Error getting PoseInFrame: "
					 << response.error_message();
	}

	return resp.pose();
}

std::vector<Discovery> RobotClient::discover_components(
    std::vector<DiscoveryQuery> queries) {
	viam::robot::v1::DiscoverComponentsRequest req;
	viam::robot::v1::DiscoverComponentsResponse resp;
	ClientContext ctx;

	RepeatedPtrField<DiscoveryQuery> *req_queries = req.mutable_queries();

	for (DiscoveryQuery query : queries) {
		*req_queries->Add() = query;
	}

	grpc::Status response = stub_->DiscoverComponents(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error) << "Error discovering components: "
					 << response.error_message();
	}

	std::vector<Discovery> components = std::vector<Discovery>();

	for (Discovery d : resp.discovery()) {
		components.push_back(d);
	}

	return components;
}

ComponentBase RobotClient::get_component(ResourceName name) {
	if (name.type() != "component") {
		std::string error =
		    "Expected resource type 'component' but got " + name.type();
		throw error;
	}
	lock.lock();
	return resource_manager.get_component(name.name());
}
//
void RobotClient::stop_all(
    std::vector<
	std::tuple<ResourceName, std::unordered_map<std::string, ProtoType>>>
	extra) {
	viam::robot::v1::StopAllRequest req;
	viam::robot::v1::StopAllResponse resp;
	ClientContext ctx;

	RepeatedPtrField<viam::robot::v1::StopExtraParameters> *ep =
	    req.mutable_extra();
	for (std::tuple<ResourceName,
			std::unordered_map<std::string, ProtoType>>
		 xtra : extra) {
		ResourceName name = std::get<0>(xtra);
		std::unordered_map<std::string, ProtoType> params =
		    std::get<1>(xtra);
		google::protobuf::Struct s = map_to_struct(params);
		viam::robot::v1::StopExtraParameters stop;
		*stop.mutable_name() = name;
		*stop.mutable_params() = s;
		*ep->Add() = stop;
	}
	grpc::Status response = stub_->StopAll(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error)
		    << "Error stopping all: " << response.error_message()
		    << response.error_details();
	}
}
