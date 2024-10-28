#include <cctype>
#include <viam/sdk/robot/client.hpp>

#include <chrono>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

#include <boost/log/trivial.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/rpc/private/viam_grpc_channel.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

using google::protobuf::RepeatedPtrField;
using viam::common::v1::Transform;
using viam::robot::v1::Discovery;
using viam::robot::v1::DiscoveryQuery;
using viam::robot::v1::FrameSystemConfig;
using viam::robot::v1::Operation;
using viam::robot::v1::RobotService;
using viam::robot::v1::Status;

// gRPC responses are frequently coming back with a spurious `Stream removed`
// error, leading to unhelpful and misleading logging. We should figure out why
// and fix that in `rust-utils`, but in the meantime this cleans up the logging
// error on the C++ side.
// NOLINTNEXTLINE
const std::string kStreamRemoved("Stream removed");

namespace {
// TODO: add a traits class for proto to type and back conversion
DiscoveryQuery to_proto(const RobotClient::discovery_query& query) {
    DiscoveryQuery proto;
    *proto.mutable_subtype() = query.subtype;
    *proto.mutable_model() = query.model;
    return proto;
}

RobotClient::discovery_query from_proto(const DiscoveryQuery& proto) {
    RobotClient::discovery_query query;
    query.subtype = proto.subtype();
    query.model = proto.model();
    return query;
}

RobotClient::discovery from_proto(const Discovery& proto) {
    RobotClient::discovery discovery;
    discovery.query = from_proto(proto.query());
    discovery.results = struct_to_map(proto.results());
    return discovery;
}

RobotClient::frame_system_config from_proto(const FrameSystemConfig& proto) {
    RobotClient::frame_system_config fsconfig;
    fsconfig.frame = WorldState::transform::from_proto(proto.frame());
    if (proto.has_kinematics()) {
        fsconfig.kinematics = struct_to_map(proto.kinematics());
    }
    return fsconfig;
}

RobotClient::status from_proto(const Status& proto) {
    RobotClient::status status;
    if (proto.has_name()) {
        status.name = Name::from_proto(proto.name());
    }
    if (proto.has_status()) {
        status.status_map = struct_to_map(proto.status());
    }
    if (proto.has_last_reconfigured()) {
        status.last_reconfigured = timestamp_to_time_pt(proto.last_reconfigured());
    }
    return status;
}

RobotClient::operation from_proto(const Operation& proto) {
    RobotClient::operation op;
    op.id = proto.id();
    op.method = proto.method();
    if (proto.has_session_id()) {
        op.session_id = proto.session_id();
    }
    if (proto.has_arguments()) {
        op.arguments = struct_to_map(proto.arguments());
    }
    if (proto.has_started()) {
        op.started = timestamp_to_time_pt(proto.started());
    }
    return op;
}
}  // namespace

bool operator==(const RobotClient::discovery_query& lhs, const RobotClient::discovery_query& rhs) {
    return lhs.subtype == rhs.subtype && lhs.model == rhs.model;
}

bool operator==(const RobotClient::discovery& lhs, const RobotClient::discovery& rhs) {
    return lhs.query == rhs.query && map_to_struct(lhs.results).SerializeAsString() ==
                                         map_to_struct(rhs.results).SerializeAsString();
}

bool operator==(const RobotClient::frame_system_config& lhs,
                const RobotClient::frame_system_config& rhs) {
    return lhs.frame == rhs.frame && map_to_struct(lhs.kinematics).SerializeAsString() ==
                                         map_to_struct(rhs.kinematics).SerializeAsString();
}

bool operator==(const RobotClient::status& lhs, const RobotClient::status& rhs) {
    return lhs.name == rhs.name &&
           map_to_struct(lhs.status_map).SerializeAsString() ==
               map_to_struct(rhs.status_map).SerializeAsString() &&
           lhs.last_reconfigured == rhs.last_reconfigured;
}

bool operator==(const RobotClient::operation& lhs, const RobotClient::operation& rhs) {
    return lhs.id == rhs.id && lhs.method == rhs.method && lhs.session_id == rhs.session_id &&
           lhs.arguments == rhs.arguments && lhs.started == rhs.started;
}

struct RobotClient::impl {
    impl(std::unique_ptr<RobotService::Stub> stub) : stub_(std::move(stub)) {}
    std::unique_ptr<RobotService::Stub> stub_;
};

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

bool is_error_response(const grpc::Status& response) {
    return !response.ok() && (response.error_message() != kStreamRemoved);
}
std::vector<RobotClient::status> RobotClient::get_status() {
    auto resources = resource_names();
    return get_status(resources);
}

void RobotClient::log(
    std::string name,
    std::string level,
    std::string message,
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>& time,
    std::string stack) {
    robot::v1::LogRequest req;
    common::v1::LogEntry log;
    *log.mutable_logger_name() = std::move(name);
    std::transform(level.begin(), level.end(), level.begin(), ::toupper);
    log.set_level(level);
    *log.mutable_message() = std::move(message);
    *log.mutable_time() = time_pt_to_timestamp(time);
    *log.mutable_stack() = std::move(stack);
    *req.mutable_logs()->Add() = std::move(log);
    robot::v1::LogResponse resp;
    ClientContext ctx;
    const auto response = impl_->stub_->Log(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error sending log: " << response.error_message()
                                 << response.error_details();
    }
}

// gets statuses of components associated with robot. If a specific component
// vector is provided, only statuses for the given Names will be
// returned
std::vector<RobotClient::status> RobotClient::get_status(std::vector<Name>& components) {
    viam::robot::v1::GetStatusRequest req;
    viam::robot::v1::GetStatusResponse resp;
    ClientContext ctx;
    for (const Name& name : components) {
        *req.mutable_resource_names()->Add() = name.to_proto();
    }

    const grpc::Status response = impl_->stub_->GetStatus(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error getting status: " << response.error_message()
                                 << response.error_details();
    }

    const RepeatedPtrField<Status> resp_status = resp.status();

    std::vector<status> statuses = std::vector<status>();

    for (const Status& s : resp_status) {
        statuses.push_back(from_proto(s));
    }

    return statuses;
}

std::vector<RobotClient::operation> RobotClient::get_operations() {
    const viam::robot::v1::GetOperationsRequest req;
    viam::robot::v1::GetOperationsResponse resp;
    ClientContext ctx;

    std::vector<operation> operations;

    grpc::Status const response = impl_->stub_->GetOperations(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error getting operations: " << response.error_message();
    }

    for (int i = 0; i < resp.operations().size(); ++i) {
        // NOLINTNEXTLINE
        operations.push_back(from_proto(resp.operations().at(i)));
    }
    return operations;
}

void RobotClient::cancel_operation(std::string id) {
    viam::robot::v1::CancelOperationRequest req;
    viam::robot::v1::CancelOperationResponse resp;
    ClientContext ctx;

    req.set_id(id);
    const grpc::Status response = impl_->stub_->CancelOperation(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error canceling operation with id " << id;
    }
}

void RobotClient::block_for_operation(std::string id) {
    viam::robot::v1::BlockForOperationRequest req;
    viam::robot::v1::BlockForOperationResponse resp;
    ClientContext ctx;

    req.set_id(id);

    const grpc::Status response = impl_->stub_->BlockForOperation(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error blocking for operation with id " << id;
    }
}

void RobotClient::refresh() {
    const viam::robot::v1::ResourceNamesRequest req;
    viam::robot::v1::ResourceNamesResponse resp;
    ClientContext ctx;

    const grpc::Status response = impl_->stub_->ResourceNames(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error getting resource names: " << response.error_message()
                                 << response.error_details();
        return;
    }

    std::unordered_map<Name, std::shared_ptr<Resource>> new_resources;
    std::vector<Name> current_resources;
    for (const auto& name : resp.resources()) {
        current_resources.push_back(Name::from_proto(name));
        if (name.subtype() == "remote") {
            continue;
        }

        // TODO(RSDK-2066): as we create wrappers, make sure components in wrappers
        // are being properly registered from name.subtype(), or update what we're
        // using for lookup
        const std::shared_ptr<const ResourceClientRegistration> rs =
            Registry::lookup_resource_client({name.namespace_(), name.type(), name.subtype()});
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
            if (!(resource_names_.at(i) == current_resources.at(i))) {
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
    : channel_(channel->channel()),
      viam_channel_(std::move(channel)),
      should_close_channel_(false),
      impl_(std::make_unique<impl>(RobotService::NewStub(channel_))) {
    Registry::initialize();
}

std::vector<Name> RobotClient::resource_names() const {
    const std::lock_guard<std::mutex> lock(lock_);
    return resource_names_;
}

std::shared_ptr<RobotClient> RobotClient::with_channel(std::shared_ptr<ViamChannel> channel,
                                                       const Options& options) {
    std::shared_ptr<RobotClient> robot = std::make_shared<RobotClient>(std::move(channel));
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

std::shared_ptr<RobotClient> RobotClient::at_address(const std::string& address,
                                                     const Options& options) {
    const char* uri = address.c_str();
    auto channel = ViamChannel::dial(uri, options.dial_options());
    std::shared_ptr<RobotClient> robot = RobotClient::with_channel(channel, options);
    robot->should_close_channel_ = true;

    return robot;
};

std::shared_ptr<RobotClient> RobotClient::at_local_socket(const std::string& address,
                                                          const Options& options) {
    const std::string addr = "unix://" + address;
    const char* uri = addr.c_str();
    const std::shared_ptr<grpc::Channel> channel =
        sdk::impl::create_viam_channel(uri, grpc::InsecureChannelCredentials());
    auto viam_channel = std::make_shared<ViamChannel>(channel, address.c_str(), nullptr);
    std::shared_ptr<RobotClient> robot = RobotClient::with_channel(viam_channel, options);
    robot->should_close_channel_ = true;

    return robot;
};

std::vector<RobotClient::frame_system_config> RobotClient::get_frame_system_config(
    const std::vector<WorldState::transform>& additional_transforms) {
    viam::robot::v1::FrameSystemConfigRequest req;
    viam::robot::v1::FrameSystemConfigResponse resp;
    ClientContext ctx;

    RepeatedPtrField<Transform>* req_transforms = req.mutable_supplemental_transforms();
    for (const WorldState::transform& transform : additional_transforms) {
        *req_transforms->Add() = transform.to_proto();
    }

    const grpc::Status response = impl_->stub_->FrameSystemConfig(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error getting frame system config: "
                                 << response.error_message();
    }

    const RepeatedPtrField<FrameSystemConfig> configs = resp.frame_system_configs();

    std::vector<RobotClient::frame_system_config> fs_configs = std::vector<frame_system_config>();

    for (const FrameSystemConfig& fs : configs) {
        fs_configs.push_back(from_proto(fs));
    }

    return fs_configs;
}

pose_in_frame RobotClient::transform_pose(
    const pose_in_frame& query,
    std::string destination,
    const std::vector<WorldState::transform>& additional_transforms) {
    viam::robot::v1::TransformPoseRequest req;
    viam::robot::v1::TransformPoseResponse resp;
    ClientContext ctx;

    *req.mutable_source() = query.to_proto();
    *req.mutable_destination() = std::move(destination);
    RepeatedPtrField<Transform>* req_transforms = req.mutable_supplemental_transforms();

    for (const WorldState::transform& transform : additional_transforms) {
        *req_transforms->Add() = transform.to_proto();
    }

    const grpc::Status response = impl_->stub_->TransformPose(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error getting PoseInFrame: " << response.error_message();
    }

    return pose_in_frame::from_proto(resp.pose());
}

std::vector<RobotClient::discovery> RobotClient::discover_components(
    const std::vector<discovery_query>& queries) {
    viam::robot::v1::DiscoverComponentsRequest req;
    viam::robot::v1::DiscoverComponentsResponse resp;
    ClientContext ctx;

    RepeatedPtrField<DiscoveryQuery>* req_queries = req.mutable_queries();

    for (const discovery_query& query : queries) {
        *req_queries->Add() = to_proto(query);
    }

    const grpc::Status response = impl_->stub_->DiscoverComponents(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error discovering components: " << response.error_message();
    }

    std::vector<discovery> components = std::vector<discovery>();

    for (const Discovery& d : resp.discovery()) {
        components.push_back(from_proto(d));
    }

    return components;
}

std::shared_ptr<Resource> RobotClient::resource_by_name(const Name& name) {
    return resource_manager_.resource(name.name());
}

void RobotClient::stop_all() {
    std::unordered_map<Name, ProtoStruct> map;
    for (const Name& name : resource_names()) {
        map.emplace(name, ProtoStruct{});
    }
    stop_all(map);
}

void RobotClient::stop_all(const std::unordered_map<Name, ProtoStruct>& extra) {
    viam::robot::v1::StopAllRequest req;
    viam::robot::v1::StopAllResponse resp;
    ClientContext ctx;

    RepeatedPtrField<viam::robot::v1::StopExtraParameters>* ep = req.mutable_extra();
    for (const auto& xtra : extra) {
        const Name& name = xtra.first;
        const ProtoStruct& params = xtra.second;
        const google::protobuf::Struct s = map_to_struct(params);
        viam::robot::v1::StopExtraParameters stop;
        *stop.mutable_name() = name.to_proto();
        *stop.mutable_params() = s;
        *ep->Add() = stop;
    }
    const grpc::Status response = impl_->stub_->StopAll(ctx, req, &resp);
    if (is_error_response(response)) {
        BOOST_LOG_TRIVIAL(error) << "Error stopping all: " << response.error_message()
                                 << response.error_details();
    }
}

}  // namespace sdk
}  // namespace viam
