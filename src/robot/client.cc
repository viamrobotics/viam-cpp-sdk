#define BOOST_LOG_DYN_LINK 1
// #include <grpcpp/client_context.h>
// #include <grpcpp/grpcpp.h>
// #include <grpcpp/support/status.h>
#include <robot/v1/robot.grpc.pb.h>

// #include "robot/v1/robot.grpc.pb.h"
//  #include <robot/v1/robot.grpc.pb.h>
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
#include <string>
#include <thread>
#include <vector>

#include "../components/resource_manager.h"
#include "../components/service_base.h"
#include "../registry/registry.h"
#include "../rpc/dial.h"
#include "common/v1/common.pb.h"
#include "grpcpp/channel.h"
#include "grpcpp/client_context.h"
#include "grpcpp/grpcpp.h"
#include "grpcpp/support/status.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using viam::common::v1::ResourceName;
using viam::robot::v1::RobotService;

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
	RobotClient(ViamChannel channel);
	std::vector<ResourceName> *resource_names();
	std::unique_ptr<RobotService::Stub> stub_;

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
	should_refresh.store(false);
	viam_channel.close();
}

void RobotClient::refresh() {
	viam::robot::v1::ResourceNamesRequest req;
	viam::robot::v1::ResourceNamesResponse resp;
	ClientContext ctx;
	Status response = stub_->ResourceNames(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error) << "Error getting resource names: "
					 << response.error_message();
		return;
	}

	ResourceManager new_resource_manager;
	google::protobuf::RepeatedPtrField<ResourceName> resources =
	    resp.resources();

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
	// TODO: come up with a better equality check here. equality is not
	// defined for `const ResourceName` unfortunately, but this is pretty
	// awkward as is.
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

// Create a robot client that is connected to the robot at the provided address.
//
// Args:
// 		address: Address of the robot (IP address, URI, URL, etc.)
// 		options: Options for connecting and refreshing
std::shared_ptr<RobotClient> RobotClient::at_address(std::string address,
						     Options options) {
	const char *uri = address.c_str();
	ViamChannel channel = dial(uri, options.dial_options);
	std::shared_ptr<RobotClient> robot =
	    RobotClient::with_channel(channel, options);
	robot->should_close_channel = true;

	return robot;
};
