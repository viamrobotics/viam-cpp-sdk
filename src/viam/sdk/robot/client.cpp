#include <viam/sdk/robot/client.hpp>

#include <algorithm>
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
#include <type_traits>
#include <unistd.h>
#include <vector>

#include <boost/log/trivial.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

// TODO(RSDK-4573) Having all these proto types exposed here in the APIs is sad. Let's fix that.

using google::protobuf::RepeatedPtrField;
using viam::common::v1::ResourceName;
using viam::robot::v1::Operation;
using viam::robot::v1::RobotService;
using viam::robot::v1::Status;

// gRPC responses are frequently coming back with a spurious `Stream removed`
// error, leading to unhelpful and misleading logging. We should figure out why
// and fix that in `rust-utils`, but in the meantime this cleans up the logging
// error on the C++ side.
// NOLINTNEXTLINE
const std::string kStreamRemoved("Stream removed");

viam::robot::v1::DiscoveryQuery discoveryQuery::to_proto() const {
    viam::robot::v1::DiscoveryQuery proto;
    *proto.mutable_subtype() = subtype;
    *proto.mutable_model() = std::move(model);
    return proto;
}

discoveryQuery discoveryQuery::from_proto(const viam::robot::v1::DiscoveryQuery& proto) {
    discoveryQuery query;
    query.subtype = proto.subtype();
    query.model = proto.model();
    return query;
}

viam::robot::v1::Discovery discovery::to_proto() const {
    viam::robot::v1::Discovery proto;
    *proto.mutable_query() = query.to_proto();
    *proto.mutable_results() = map_to_struct(results);
    return proto;
}

discovery discovery::from_proto(const viam::robot::v1::Discovery& proto) {
    discovery discovery;
    discovery.query = discoveryQuery::from_proto(proto.query());
    discovery.results = struct_to_map(proto.results());
    return discovery;
}

viam::robot::v1::FrameSystemConfig frameSystemConfig::to_proto() const {
    viam::robot::v1::FrameSystemConfig proto;
    *proto.mutable_frame() = frame.to_proto();
    if (kinematics) {
        *proto.mutable_kinematics() = map_to_struct(kinematics);
    }
    return proto;
}

frameSystemConfig frameSystemConfig::from_proto(const viam::robot::v1::FrameSystemConfig& proto) {
    frameSystemConfig fsconfig =
        frameSystemConfig(WorldState::transform::from_proto(proto.frame()));
    if (proto.has_kinematics()) {
        fsconfig.kinematics = struct_to_map(proto.kinematics());
    }
    return fsconfig;
}

RobotClient::~RobotClient() {
    if (should_close_channel_) {
        try {
            this->close();
        } catch (const std::exception& e) {
            BOOST_LOG_TRIVIAL(error) << "Received err while closing RobotClient: " << e.what();
        } catch (...) {
            BOOST_LOG_TRIVIAL(error) << "Received unknown err while closing RobotClient";
        }
    }
}

void RobotClient::close() {
    should_refresh_.store(false);
    for (const std::shared_ptr<std::thread>& t : threads_) {
        t->~thread();
    }
    stop_all();
    viam_channel_->close();
}

bool is_error_response(grpc::Status response) {
    return !response.ok() && (response.error_message() != kStreamRemoved);
}
std::vector<Status> RobotClient::get_status() {
    auto* resources = resource_names();
    return get_status(*resources);
}
// gets Statuses of components associated with robot. If a specific component
// vector is provided, only statuses for the given ResourceNames will be
// returned
std::vector<Status> RobotClient::get_status(std::vector<ResourceName>& components) {
    viam::robot::v1::GetStatusRequest req;
    viam::robot::v1::GetStatusResponse resp;
    ClientContext ctx;
    for (const ResourceName& name : components) {
        *req.mutable_resource_names()->Add() = name;
    }

    const grpc::Status response = stub_->GetStatus(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error getting status: " << response.error_message()
                                 << response.error_details();
    }

    const RepeatedPtrField<Status> status = resp.status();

    std::vector<Status> statuses = std::vector<Status>();

    for (const Status& s : status) {
        statuses.push_back(s);
    }

    return statuses;
}

std::vector<Operation> RobotClient::get_operations() {
    const viam::robot::v1::GetOperationsRequest req;
    viam::robot::v1::GetOperationsResponse resp;
    ClientContext ctx;

    std::vector<Operation> operations;

    grpc::Status const response = stub_->GetOperations(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error getting operations: " << response.error_message();
    }

    for (int i = 0; i < resp.operations().size(); ++i) {
        // NOLINTNEXTLINE
        operations.push_back(resp.operations().at(i));
    }
    return operations;
}

void RobotClient::cancel_operation(std::string id) {
    viam::robot::v1::CancelOperationRequest req;
    viam::robot::v1::CancelOperationResponse resp;
    ClientContext ctx;

    req.set_id(id);
    const grpc::Status response = stub_->CancelOperation(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error canceling operation with id " << id;
    }
}

void RobotClient::block_for_operation(std::string id) {
    viam::robot::v1::BlockForOperationRequest req;
    viam::robot::v1::BlockForOperationResponse resp;
    ClientContext ctx;

    req.set_id(id);

    const grpc::Status response = stub_->BlockForOperation(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error blocking for operation with id " << id;
    }
}

void RobotClient::refresh() {
    const viam::robot::v1::ResourceNamesRequest req;
    viam::robot::v1::ResourceNamesResponse resp;
    ClientContext ctx;

    const grpc::Status response = stub_->ResourceNames(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error getting resource names: " << response.error_message();
    }

    std::unordered_map<Name, std::shared_ptr<Resource>> new_resources;
    std::vector<ResourceName> current_resources;
    for (const auto& name : resp.resources()) {
        current_resources.push_back(name);
        if (name.subtype() == "remote") {
            continue;
        }

        // TODO(RSDK-2066): as we create wrappers, make sure components in wrappers
        // are being properly registered from name.subtype(), or update what we're
        // using for lookup
        const std::shared_ptr<ResourceRegistration> rs =
            Registry::lookup_resource({name.namespace_(), name.type(), name.subtype()});
        if (rs) {
            try {
                const std::shared_ptr<Resource> rpc_client =
                    rs->create_rpc_client(name.name(), channel_);
                const Name name_({name.namespace_(), name.type(), name.subtype()}, "", name.name());
                new_resources.emplace(name_, rpc_client);
            } catch (const std::exception& exc) {
                BOOST_LOG_TRIVIAL(debug)
                    << "Error registering component " << name.subtype() << ": " << exc.what();
            }
        }
    }
    bool is_equal = current_resources.size() == resource_names_.size();
    if (is_equal) {
        for (size_t i = 0; i < resource_names_.size(); ++i) {
            if (!ResourceNameEqual::check_equal(resource_names_.at(i), current_resources.at(i))) {
                is_equal = false;
                break;
            }
        }
    }
    if (is_equal) {
        return;
    }

    const std::lock_guard<std::mutex> lock(lock_);
    resource_names_ = current_resources;
    this->resource_manager_.replace_all(new_resources);
}

void RobotClient::refresh_every() {
    while (should_refresh_.load()) {
        try {
            std::this_thread::sleep_for(std::chrono::seconds(refresh_interval_));
            refresh();

        } catch (std::exception& exc) {
            break;
        }
    }
};

RobotClient::RobotClient(std::shared_ptr<ViamChannel> channel)
    : viam_channel_(channel),
      channel_(channel->channel()),
      should_close_channel_(false),
      stub_(RobotService::NewStub(channel_)) {}

std::vector<ResourceName>* RobotClient::resource_names() {
    const std::lock_guard<std::mutex> lock(lock_);
    std::vector<ResourceName>* resources = &resource_names_;
    return resources;
}

std::shared_ptr<RobotClient> RobotClient::with_channel(std::shared_ptr<ViamChannel> channel,
                                                       Options options) {
    std::shared_ptr<RobotClient> robot = std::make_shared<RobotClient>(channel);
    robot->refresh_interval_ = options.refresh_interval();
    robot->should_refresh_ = (robot->refresh_interval_ > 0);
    if (robot->should_refresh_) {
        const std::shared_ptr<std::thread> t =
            std::make_shared<std::thread>(&RobotClient::refresh_every, robot);
        // TODO(RSDK-1743): this was leaking, confirm that adding thread catching in
        // close/destructor lets us shutdown gracefully. See also address sanitizer,
        // UB sanitizer
        t->detach();
        robot->threads_.push_back(t);
    };

    robot->refresh();
    return robot;
};

std::shared_ptr<RobotClient> RobotClient::at_address(std::string address, Options options) {
    const char* uri = address.c_str();
    auto channel = ViamChannel::dial(uri, options.dial_options());
    std::shared_ptr<RobotClient> robot = RobotClient::with_channel(channel, options);
    robot->should_close_channel_ = true;

    return robot;
};

std::shared_ptr<RobotClient> RobotClient::at_local_socket(std::string address, Options options) {
    const std::string addr = "unix://" + address;
    const char* uri = addr.c_str();
    const std::shared_ptr<grpc::Channel> channel =
        grpc::CreateChannel(uri, grpc::InsecureChannelCredentials());
    const std::unique_ptr<viam::robot::v1::RobotService::Stub> st =
        viam::robot::v1::RobotService::NewStub(channel);
    auto viam_channel = std::make_shared<ViamChannel>(channel, address.c_str(), nullptr);
    std::shared_ptr<RobotClient> robot = RobotClient::with_channel(viam_channel, options);
    robot->should_close_channel_ = true;

    return robot;
};

std::vector<frameSystemConfig> RobotClient::get_frame_system_config(
    std::vector<WorldState::transform> additional_transforms) {
    viam::robot::v1::FrameSystemConfigRequest req;
    viam::robot::v1::FrameSystemConfigResponse resp;
    ClientContext ctx;

    RepeatedPtrField<viam::common::v1::Transform>* req_transforms =
        req.mutable_supplemental_transforms();
    for (const WorldState::transform& transform : additional_transforms) {
        *req_transforms->Add() = transform.to_proto();
    }

    const grpc::Status response = stub_->FrameSystemConfig(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error getting frame system config: "
                                 << response.error_message();
    }

    const RepeatedPtrField<viam::robot::v1::FrameSystemConfig> configs =
        resp.frame_system_configs();

    std::vector<frameSystemConfig> fs_configs = std::vector<frameSystemConfig>();

    for (const viam::robot::v1::FrameSystemConfig& fs : configs) {
        fs_configs.push_back(frameSystemConfig::from_proto(fs));
    }

    return fs_configs;
}

pose_in_frame RobotClient::transform_pose(
    pose_in_frame query,
    std::string destination,
    std::vector<WorldState::transform> additional_transforms) {
    viam::robot::v1::TransformPoseRequest req;
    viam::robot::v1::TransformPoseResponse resp;
    ClientContext ctx;

    *req.mutable_source() = query.to_proto();
    *req.mutable_destination() = destination;
    RepeatedPtrField<viam::common::v1::Transform>* req_transforms =
        req.mutable_supplemental_transforms();

    for (const WorldState::transform& transform : additional_transforms) {
        *req_transforms->Add() = transform.to_proto();
    }

    const grpc::Status response = stub_->TransformPose(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error getting PoseInFrame: " << response.error_message();
    }

    return pose_in_frame::from_proto(resp.pose());
}

std::vector<discovery> RobotClient::discover_components(std::vector<discoveryQuery> queries) {
    viam::robot::v1::DiscoverComponentsRequest req;
    viam::robot::v1::DiscoverComponentsResponse resp;
    ClientContext ctx;

    RepeatedPtrField<viam::robot::v1::DiscoveryQuery>* req_queries = req.mutable_queries();

    for (const discoveryQuery& query : queries) {
        *req_queries->Add() = query.to_proto();
    }

    const grpc::Status response = stub_->DiscoverComponents(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error discovering components: " << response.error_message();
    }

    std::vector<discovery> components = std::vector<discovery>();

    for (const viam::robot::v1::Discovery& d : resp.discovery()) {
        components.push_back(discovery::from_proto(d));
    }

    return components;
}

std::shared_ptr<Resource> RobotClient::resource_by_name(const ResourceName& name) {
    return resource_manager_.resource(name.name());
}

void RobotClient::stop_all() {
    std::unordered_map<ResourceName,
                       std::unordered_map<std::string, std::shared_ptr<ProtoType>>,
                       ResourceNameHasher,
                       ResourceNameEqual>
        map;
    for (const ResourceName& name : *resource_names()) {
        const std::unordered_map<std::string, std::shared_ptr<ProtoType>> val;
        map.emplace(name, val);
    }
    stop_all(map);
}

void RobotClient::stop_all(
    std::unordered_map<ResourceName,
                       std::unordered_map<std::string, std::shared_ptr<ProtoType>>,
                       ResourceNameHasher,
                       ResourceNameEqual> extra) {
    viam::robot::v1::StopAllRequest req;
    viam::robot::v1::StopAllResponse resp;
    ClientContext ctx;

    RepeatedPtrField<viam::robot::v1::StopExtraParameters>* ep = req.mutable_extra();
    for (auto& xtra : extra) {
        const ResourceName name = xtra.first;
        const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> params =
            std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(
                xtra.second);
        const google::protobuf::Struct s = map_to_struct(params);
        viam::robot::v1::StopExtraParameters stop;
        *stop.mutable_name() = name;
        *stop.mutable_params() = s;
        *ep->Add() = stop;
    }
    const grpc::Status response = stub_->StopAll(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error stopping all: " << response.error_message()
                                 << response.error_details();
    }
}

}  // namespace sdk
}  // namespace viam
