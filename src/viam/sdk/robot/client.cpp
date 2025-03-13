#include <viam/sdk/robot/client.hpp>

#include <chrono>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

#include <boost/log/core/core.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/log/private/log_backend.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/rpc/private/viam_grpc_channel.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

using google::protobuf::RepeatedPtrField;
using viam::common::v1::Transform;
using viam::robot::v1::FrameSystemConfig;
using viam::robot::v1::Operation;
using viam::robot::v1::RobotService;

// gRPC responses are frequently coming back with a spurious `Stream removed`
// error, leading to unhelpful and misleading logging. We should figure out why
// and fix that in `rust-utils`, but in the meantime this cleans up the logging
// error on the C++ side.
// NOLINTNEXTLINE
const std::string kStreamRemoved("Stream removed");

RobotClient::frame_system_config from_proto(const FrameSystemConfig& proto) {
    RobotClient::frame_system_config fsconfig;
    fsconfig.frame = from_proto(proto.frame());
    if (proto.has_kinematics()) {
        fsconfig.kinematics = from_proto(proto.kinematics());
    }
    return fsconfig;
}

RobotClient::operation from_proto(const Operation& proto) {
    RobotClient::operation op;
    op.id = proto.id();
    op.method = proto.method();
    if (proto.has_session_id()) {
        op.session_id = proto.session_id();
    }
    if (proto.has_arguments()) {
        op.arguments = from_proto(proto.arguments());
    }
    if (proto.has_started()) {
        op.started = from_proto(proto.started());
    }
    return op;
}

bool operator==(const RobotClient::frame_system_config& lhs,
                const RobotClient::frame_system_config& rhs) {
    return lhs.frame == rhs.frame && to_proto(lhs.kinematics).SerializeAsString() ==
                                         to_proto(rhs.kinematics).SerializeAsString();
}

bool operator==(const RobotClient::operation& lhs, const RobotClient::operation& rhs) {
    return lhs.id == rhs.id && lhs.method == rhs.method && lhs.session_id == rhs.session_id &&
           lhs.arguments == rhs.arguments && lhs.started == rhs.started;
}

struct RobotClient::impl {
    impl(std::unique_ptr<RobotService::Stub> stub) : stub_(std::move(stub)) {}

    ~impl() {
        if (log_sink) {
            boost::log::core::get()->remove_sink(log_sink);
        }
    }

    std::unique_ptr<RobotService::Stub> stub_;

    boost::shared_ptr<viam::sdk::impl::SinkType> log_sink;
};

void RobotClient::connect_logging() {
    auto& sink = impl_->log_sink;
    if (!sink) {
        sink = sdk::impl::LogBackend::create(this);
        sink->set_filter(Logger::Filter{&Logger::get()});

        Logger::get().disable_console_logging();
        boost::log::core::get()->add_sink(sink);
    }
}

RobotClient::~RobotClient() {
    if (should_close_channel_) {
        try {
            this->close();
        } catch (const std::exception& e) {
            VIAM_LOG(error) << "Received err while closing RobotClient: " << e.what();
        } catch (...) {
            VIAM_LOG(error) << "Received unknown err while closing RobotClient";
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

std::vector<RobotClient::operation> RobotClient::get_operations() {
    const viam::robot::v1::GetOperationsRequest req;
    viam::robot::v1::GetOperationsResponse resp;
    ClientContext ctx;

    std::vector<operation> operations;

    grpc::Status const response = impl_->stub_->GetOperations(ctx, req, &resp);
    if (is_error_response(response)) {
        VIAM_LOG(error) << "Error getting operations: " << response.error_message();
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
        VIAM_LOG(error) << "Error canceling operation with id " << id;
    }
}

void RobotClient::block_for_operation(std::string id) {
    viam::robot::v1::BlockForOperationRequest req;
    viam::robot::v1::BlockForOperationResponse resp;
    ClientContext ctx;

    req.set_id(id);

    const grpc::Status response = impl_->stub_->BlockForOperation(ctx, req, &resp);
    if (is_error_response(response)) {
        VIAM_LOG(error) << "Error blocking for operation with id " << id;
    }
}

void RobotClient::refresh() {
    const viam::robot::v1::ResourceNamesRequest req;
    viam::robot::v1::ResourceNamesResponse resp;
    ClientContext ctx;

    const grpc::Status response = impl_->stub_->ResourceNames(ctx, req, &resp);
    if (is_error_response(response)) {
        VIAM_LOG(error) << "Error getting resource names: " << response.error_message();
    }

    std::unordered_map<Name, std::shared_ptr<Resource>> new_resources;
    std::vector<Name> current_resources;
    for (const auto& name : resp.resources()) {
        current_resources.push_back(from_proto(name));
        if (name.subtype() == "remote") {
            continue;
        }

        // TODO(RSDK-2066): as we create wrappers, make sure components in wrappers
        // are being properly registered from name.subtype(), or update what we're
        // using for lookup
        const std::shared_ptr<const ResourceClientRegistration> rs =
            Registry::get().lookup_resource_client(
                {name.namespace_(), name.type(), name.subtype()});
        if (rs) {
            try {
                const std::shared_ptr<Resource> rpc_client =
                    rs->create_rpc_client(name.name(), channel_);
                const Name name_({name.namespace_(), name.type(), name.subtype()}, "", name.name());
                new_resources.emplace(name_, rpc_client);
            } catch (const std::exception& exc) {
                VIAM_LOG(debug) << "Error registering component " << name.subtype() << ": "
                                << exc.what();
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
      impl_(std::make_unique<impl>(RobotService::NewStub(channel_))) {}

std::vector<Name> RobotClient::resource_names() const {
    const std::lock_guard<std::mutex> lock(lock_);
    return resource_names_;
}

void RobotClient::log(const std::string& name,
                      const std::string& level,
                      const std::string& message,
                      time_pt time) {
    robot::v1::LogRequest req;
    common::v1::LogEntry log;

    *log.mutable_logger_name() = name;
    log.set_level(level);
    *log.mutable_message() = message;
    *log.mutable_time() = to_proto(time);
    req.mutable_logs()->Add(std::move(log));

    robot::v1::LogResponse resp;
    ClientContext ctx;
    const auto response = impl_->stub_->Log(ctx, req, &resp);
    (void)response;
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
    auto channel = ViamChannel::dial_initial(uri, options.dial_options());
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

    *(req.mutable_supplemental_transforms()) = sdk::impl::to_repeated_field(additional_transforms);

    const grpc::Status response = impl_->stub_->FrameSystemConfig(ctx, req, &resp);
    if (is_error_response(response)) {
        VIAM_LOG(error) << "Error getting frame system config: " << response.error_message();
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

    *req.mutable_source() = to_proto(query);
    *req.mutable_destination() = std::move(destination);
    *req.mutable_supplemental_transforms() = sdk::impl::to_repeated_field(additional_transforms);

    const grpc::Status response = impl_->stub_->TransformPose(ctx, req, &resp);
    if (is_error_response(response)) {
        VIAM_LOG(error) << "Error getting PoseInFrame: " << response.error_message();
    }

    return from_proto(resp.pose());
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
        const google::protobuf::Struct s = to_proto(params);
        viam::robot::v1::StopExtraParameters stop;
        *stop.mutable_name() = to_proto(name);
        *stop.mutable_params() = s;
        *ep->Add() = stop;
    }
    const grpc::Status response = impl_->stub_->StopAll(ctx, req, &resp);
    if (is_error_response(response)) {
        VIAM_LOG(error) << "Error stopping all: " << response.error_message()
                        << response.error_details();
    }
}

std::ostream& operator<<(std::ostream& os, const RobotClient::status& v) {
    std::string status;
    switch (v) {
        case RobotClient::status::k_running:
            status = "running";
            break;
        case RobotClient::status::k_initializing:
            status = "initializing";
            break;
        case RobotClient::status::k_unspecified:
        default:
            status = "unspecified";
    }
    os << status;
    return os;
}

RobotClient::status RobotClient::get_machine_status() const {
    const robot::v1::GetMachineStatusRequest req;
    robot::v1::GetMachineStatusResponse resp;
    ClientContext ctx;

    const grpc::Status response = impl_->stub_->GetMachineStatus(ctx, req, &resp);
    if (is_error_response(response)) {
        VIAM_LOG(error) << "Error getting machine status: " << response.error_message()
                        << response.error_details();
    }
    switch (resp.state()) {
        case robot::v1::GetMachineStatusResponse_State_STATE_INITIALIZING:
            return RobotClient::status::k_initializing;
        case robot::v1::GetMachineStatusResponse_State_STATE_RUNNING:
            return RobotClient::status::k_running;
        case robot::v1::GetMachineStatusResponse_State_STATE_UNSPECIFIED:
        default:
            return RobotClient::status::k_unspecified;
    }
}

}  // namespace sdk
}  // namespace viam
