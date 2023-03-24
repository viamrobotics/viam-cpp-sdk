#include <algorithm>

#include "resource/resource_base.hpp"
#define BOOST_LOG_DYN_LINK 1
#include <common/v1/common.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>
#include <unistd.h>

#include <boost/log/trivial.hpp>
#include <chrono>
#include <common/proto_type.hpp>
#include <common/utils.hpp>
#include <components/service_base.hpp>
#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <registry/registry.hpp>
#include <resource/resource_manager.hpp>
#include <robot/client.hpp>
#include <rpc/dial.hpp>
#include <set>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

using google::protobuf::RepeatedPtrField;
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
using Viam::SDK::Options;
using Viam::SDK::ViamChannel;

// gRPC responses are frequently coming back with a spurious `Stream removed`
// error, leading to unhelpful and misleading logging. We should figure out why
// and fix that in `rust-utils`, but in the meantime this cleans up the logging
// error on the C++ side.
const std::string k_stream_removed = "Stream removed";

RobotClient::~RobotClient() { this->close(); }

void RobotClient::close() {
  should_refresh.store(false);
  for (std::shared_ptr<std::thread> t : threads) {
    t->~thread();
  }
  stop_all();
  viam_channel.close();
}

bool is_error_response(grpc::Status response) {
  return !response.ok() && (response.error_message() != k_stream_removed);
}

// gets Statuses of components associated with robot. If a specific component
// vector is provided, only statuses for the given ResourceNames will be
// returned
std::vector<Status>
RobotClient::get_status(std::vector<ResourceName> components) {
  viam::robot::v1::GetStatusRequest req;
  viam::robot::v1::GetStatusResponse resp;
  ClientContext ctx;
  RepeatedPtrField<ResourceName> *request_resource_names =
      req.mutable_resource_names();

  for (ResourceName name : components) {
    *request_resource_names->Add() = name;
  }

  grpc::Status response = stub_->GetStatus(&ctx, req, &resp);
  if (is_error_response(response)) {
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
  if (is_error_response(response)) {
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
  if (is_error_response(response)) {
    BOOST_LOG_TRIVIAL(error) << "Error canceling operation with id " << id;
  }
}

void RobotClient::block_for_operation(std::string id) {
  viam::robot::v1::BlockForOperationRequest req;
  viam::robot::v1::BlockForOperationResponse resp;
  ClientContext ctx;

  req.set_id(id);

  grpc::Status response = stub_->BlockForOperation(&ctx, req, &resp);
  if (is_error_response(response)) {
    BOOST_LOG_TRIVIAL(error) << "Error blocking for operation with id " << id;
  }
}

void RobotClient::refresh() {
  viam::robot::v1::ResourceNamesRequest req;
  viam::robot::v1::ResourceNamesResponse resp;
  ClientContext ctx;
  grpc::Status response = stub_->ResourceNames(&ctx, req, &resp);
  if (is_error_response(response)) {
    BOOST_LOG_TRIVIAL(error)
        << "Error getting resource names: " << response.error_message();
  }

  ResourceManager new_resource_manager;
  RepeatedPtrField<ResourceName> resources = resp.resources();

  const int num_resources = resources.size();
  std::vector<ResourceName> current_resources;

  for (auto &name : resources) {
    current_resources.push_back(name);
    // TODO(RSDK-2066): stop filtering on COMPONENT
    if (name.type() != COMPONENT) {
      continue;
    }
    if (name.subtype() == "remote") {
      continue;
    }

    // TODO(RSDK-2066): as we create wrappers, make sure components in wrappers
    // are being properly registered from name.subtype(), or update what we're
    // using for lookup
    std::shared_ptr<ResourceSubtype> rs = Registry::lookup_subtype(
        Subtype({name.namespace_(), name.type(), name.subtype()}));
    if (rs != nullptr) {
      try {
        std::shared_ptr<ResourceBase> rpc_client =
            rs->create_rpc_client(name.name(), channel);
        new_resource_manager.register_resource(rpc_client);
      } catch (std::exception &exc) {
        BOOST_LOG_TRIVIAL(debug) << "Error registering component "
                                 << name.subtype() << ": " << exc.what();
      }
    }
  }
  bool is_equal = current_resources.size() == resource_names_.size();
  if (is_equal) {
    for (int i = 0; i < current_resources.size(); ++i) {
      if (!ResourceNameEqual::check_equal(resource_names_.at(i),
                                          current_resources.at(i))) {
        is_equal = false;
        break;
      }
    }
  }
  if (is_equal) {
    return;
  }

  lock.lock();
  resource_names_ = current_resources;
  resource_manager = new_resource_manager;
  lock.unlock();
}

void RobotClient::refresh_every() {
  while (should_refresh.load()) {
    try {
      std::this_thread::sleep_for(std::chrono::seconds(refresh_interval));
      refresh();

    } catch (std::exception &exc) {
      break;
    }
  }
};

RobotClient::RobotClient(ViamChannel vc) : viam_channel(vc) {
  should_close_channel = false;
  stub_ = std::move(RobotService::NewStub(vc.channel));
  channel = vc.channel;
}

std::vector<ResourceName> *RobotClient::resource_names() {
  lock.lock();
  std::vector<ResourceName> *resources = &resource_names_;
  lock.unlock();
  return resources;
}

std::shared_ptr<RobotClient> RobotClient::with_channel(ViamChannel channel,
                                                       Options options) {
  std::shared_ptr<RobotClient> robot = std::make_shared<RobotClient>(channel);
  robot->refresh_interval = options.refresh_interval;
  robot->should_refresh = (robot->refresh_interval > 0);
  if (robot->should_refresh) {
    std::shared_ptr<std::thread> t =
        std::make_shared<std::thread>(&RobotClient::refresh_every, robot);
    // TODO(RSDK-1743): this was leaking, confirm that adding thread catching in
    // close/destructor lets us shutdown gracefully. See also address sanitizer,
    // UB sanitizer
    t->detach();
    robot->threads.push_back(t);
  };

  robot->refresh();
  return robot;
};

std::shared_ptr<RobotClient> RobotClient::at_address(std::string address,
                                                     Options options) {
  const char *uri = address.c_str();
  ViamChannel channel = ViamChannel::dial(uri, options.dial_options);
  std::shared_ptr<RobotClient> robot =
      RobotClient::with_channel(channel, options);
  robot->should_close_channel = true;

  return robot;
};

std::shared_ptr<RobotClient> RobotClient::at_local_socket(std::string address,
                                                          Options options) {
  address = "unix://" + address;
  const char *uri = address.c_str();
  std::shared_ptr<grpc::Channel> channel =
      grpc::CreateChannel(uri, grpc::InsecureChannelCredentials());
  std::unique_ptr<viam::robot::v1::RobotService::Stub> st =
      viam::robot::v1::RobotService::NewStub(channel);
  ViamChannel viam_channel = ViamChannel(channel, uri, nullptr);
  std::shared_ptr<RobotClient> robot =
      RobotClient::with_channel(viam_channel, options);
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
  for (Transform transform : additional_transforms) {
    *req_transforms->Add() = transform;
  }

  grpc::Status response = stub_->FrameSystemConfig(&ctx, req, &resp);
  if (is_error_response(response)) {
    BOOST_LOG_TRIVIAL(error)
        << "Error getting frame system config: " << response.error_message();
  }

  RepeatedPtrField<FrameSystemConfig> configs = resp.frame_system_configs();

  std::vector<FrameSystemConfig> fs_configs = std::vector<FrameSystemConfig>();

  for (FrameSystemConfig fs : configs) {
    fs_configs.push_back(fs);
  }

  return fs_configs;
}
PoseInFrame
RobotClient::transform_pose(PoseInFrame query, std::string destination,
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
  if (is_error_response(response)) {
    BOOST_LOG_TRIVIAL(error)
        << "Error getting PoseInFrame: " << response.error_message();
  }

  return resp.pose();
}

std::vector<Discovery>
RobotClient::discover_components(std::vector<DiscoveryQuery> queries) {
  viam::robot::v1::DiscoverComponentsRequest req;
  viam::robot::v1::DiscoverComponentsResponse resp;
  ClientContext ctx;

  RepeatedPtrField<DiscoveryQuery> *req_queries = req.mutable_queries();

  for (DiscoveryQuery query : queries) {
    *req_queries->Add() = query;
  }

  grpc::Status response = stub_->DiscoverComponents(&ctx, req, &resp);
  if (is_error_response(response)) {
    BOOST_LOG_TRIVIAL(error)
        << "Error discovering components: " << response.error_message();
  }

  std::vector<Discovery> components = std::vector<Discovery>();

  for (Discovery d : resp.discovery()) {
    components.push_back(d);
  }

  return components;
}

std::shared_ptr<ResourceBase> RobotClient::resource_by_name(ResourceName name) {
  return resource_manager.get_resource(name.name(),
                                       ResourceType("ResourceBase"));
}

void RobotClient::stop_all() {
  std::unordered_map<
      ResourceName, std::unordered_map<std::string, std::shared_ptr<ProtoType>>,
      ResourceNameHasher, ResourceNameEqual>
      map;
  for (ResourceName name : *resource_names()) {
    std::unordered_map<std::string, std::shared_ptr<ProtoType>> val;
    map.emplace(name, val);
  }
  stop_all(map);
}

void RobotClient::stop_all(
    std::unordered_map<
        ResourceName,
        std::unordered_map<std::string, std::shared_ptr<ProtoType>>,
        ResourceNameHasher, ResourceNameEqual>
        extra) {
  viam::robot::v1::StopAllRequest req;
  viam::robot::v1::StopAllResponse resp;
  ClientContext ctx;
  OrientationConfig o;

  RepeatedPtrField<viam::robot::v1::StopExtraParameters> *ep =
      req.mutable_extra();
  for (auto &xtra : extra) {
    ResourceName name = xtra.first;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
        params = std::make_shared<
            std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(
            xtra.second);
    google::protobuf::Struct s = map_to_struct(params);
    viam::robot::v1::StopExtraParameters stop;
    *stop.mutable_name() = name;
    *stop.mutable_params() = s;
    *ep->Add() = stop;
  }
  grpc::Status response = stub_->StopAll(&ctx, req, &resp);
  if (is_error_response(response)) {
    BOOST_LOG_TRIVIAL(error)
        << "Error stopping all: " << response.error_message()
        << response.error_details();
  }
}
