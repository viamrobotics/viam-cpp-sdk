#include <exception>
#include <viam/sdk/robot/client.hpp>

#include <chrono>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <boost/log/core/core.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>

#include <viam/api/app/packages/v1/packages.pb.h>
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
#include <viam/sdk/tracing/private/tracer.hpp>

namespace viam {
namespace sdk {

using google::protobuf::RepeatedPtrField;
using viam::app::packages::v1::PackageType;
using viam::common::v1::Transform;
using viam::robot::v1::ConfigStatus;
using viam::robot::v1::FrameSystemConfig;
using viam::robot::v1::GetMachineStatusResponse;
using viam::robot::v1::JobStatus;
using viam::robot::v1::ModuleStatus;
using viam::robot::v1::Operation;
using viam::robot::v1::PackageStatus;
using viam::robot::v1::ResourceStatus;
using viam::robot::v1::RobotService;

// gRPC responses are frequently coming back with a spurious `Stream removed`
// error, leading to unhelpful and misleading logging. We should figure out why
// and fix that in `rust-utils`, but in the meantime this cleans up the logging
// error on the C++ side.
// NOLINTNEXTLINE
const std::string kStreamRemoved("Stream removed");

namespace proto_convert_details {

RobotClient::frame_system_config from_proto_impl<FrameSystemConfig>::operator()(
    const FrameSystemConfig* proto) const {
    RobotClient::frame_system_config fsconfig;
    fsconfig.frame = from_proto(proto->frame());
    if (proto->has_kinematics()) {
        fsconfig.kinematics = from_proto(proto->kinematics());
    }
    return fsconfig;
}

RobotClient::operation from_proto_impl<Operation>::operator()(const Operation* proto) const {
    RobotClient::operation op;
    op.id = proto->id();
    op.method = proto->method();
    if (proto->has_session_id()) {
        op.session_id = proto->session_id();
    }
    if (proto->has_arguments()) {
        op.arguments = from_proto(proto->arguments());
    }
    if (proto->has_started()) {
        op.started = from_proto(proto->started());
    }
    return op;
}

RobotClient::resource_status from_proto_impl<ResourceStatus>::operator()(
    const ResourceStatus* proto) const {
    RobotClient::resource_status rs;
    rs.name = from_proto(proto->name());
    switch (proto->state()) {
        case robot::v1::ResourceStatus_State_STATE_UNCONFIGURED:
            rs.state = RobotClient::resource_status::resource_state::k_unconfigured;
            break;
        case robot::v1::ResourceStatus_State_STATE_CONFIGURING:
            rs.state = RobotClient::resource_status::resource_state::k_configuring;
            break;
        case robot::v1::ResourceStatus_State_STATE_READY:
            rs.state = RobotClient::resource_status::resource_state::k_ready;
            break;
        case robot::v1::ResourceStatus_State_STATE_REMOVING:
            rs.state = RobotClient::resource_status::resource_state::k_removing;
            break;
        case robot::v1::ResourceStatus_State_STATE_UNHEALTHY:
            rs.state = RobotClient::resource_status::resource_state::k_unhealthy;
            break;
        case robot::v1::ResourceStatus_State_STATE_UNSPECIFIED:
        default:
            rs.state = RobotClient::resource_status::resource_state::k_unspecified;
    }
    if (proto->has_last_updated()) {
        rs.last_updated = from_proto(proto->last_updated());
    }
    rs.revision = proto->revision();
    rs.error = proto->error();
    return rs;
}

RobotClient::config_status from_proto_impl<ConfigStatus>::operator()(
    const ConfigStatus* proto) const {
    RobotClient::config_status cs;
    cs.revision = proto->revision();
    if (proto->has_last_updated()) {
        cs.last_updated = from_proto(proto->last_updated());
    }
    return cs;
}

RobotClient::module_status from_proto_impl<ModuleStatus>::operator()(
    const ModuleStatus* proto) const {
    RobotClient::module_status ms;
    ms.module_name = proto->module_name();
    switch (proto->state()) {
        case robot::v1::ModuleStatus_State_STATE_PENDING:
            ms.state = RobotClient::module_status::module_state::k_pending;
            break;
        case robot::v1::ModuleStatus_State_STATE_STARTING:
            ms.state = RobotClient::module_status::module_state::k_starting;
            break;
        case robot::v1::ModuleStatus_State_STATE_READY:
            ms.state = RobotClient::module_status::module_state::k_ready;
            break;
        case robot::v1::ModuleStatus_State_STATE_UNHEALTHY:
            ms.state = RobotClient::module_status::module_state::k_unhealthy;
            break;
        case robot::v1::ModuleStatus_State_STATE_CLOSING:
            ms.state = RobotClient::module_status::module_state::k_closing;
            break;
        case robot::v1::ModuleStatus_State_STATE_UNSPECIFIED:
        default:
            ms.state = RobotClient::module_status::module_state::k_unspecified;
    }
    if (proto->has_last_updated()) {
        ms.last_updated = from_proto(proto->last_updated());
    }
    ms.error = proto->error();
    ms.consecutive_failures = proto->consecutive_failures();
    return ms;
}

RobotClient::package_status from_proto_impl<PackageStatus>::operator()(
    const PackageStatus* proto) const {
    RobotClient::package_status ps;
    ps.name = proto->name();
    switch (proto->type()) {
        case PackageType::PACKAGE_TYPE_ARCHIVE:
            ps.type = RobotClient::package_status::package_type::k_archive;
            break;
        case PackageType::PACKAGE_TYPE_ML_MODEL:
            ps.type = RobotClient::package_status::package_type::k_ml_model;
            break;
        case PackageType::PACKAGE_TYPE_MODULE:
            ps.type = RobotClient::package_status::package_type::k_module;
            break;
        case PackageType::PACKAGE_TYPE_SLAM_MAP:
            ps.type = RobotClient::package_status::package_type::k_slam_map;
            break;
        case PackageType::PACKAGE_TYPE_ML_TRAINING:
            ps.type = RobotClient::package_status::package_type::k_ml_training;
            break;
        case PackageType::PACKAGE_TYPE_UNSPECIFIED:
        default:
            ps.type = RobotClient::package_status::package_type::k_unspecified;
    }
    switch (proto->state()) {
        case robot::v1::PackageStatus_State_STATE_DOWNLOADING:
            ps.state = RobotClient::package_status::package_state::k_downloading;
            break;
        case robot::v1::PackageStatus_State_STATE_LOADING:
            ps.state = RobotClient::package_status::package_state::k_loading;
            break;
        case robot::v1::PackageStatus_State_STATE_FIRST_RUN:
            ps.state = RobotClient::package_status::package_state::k_first_run;
            break;
        case robot::v1::PackageStatus_State_STATE_READY:
            ps.state = RobotClient::package_status::package_state::k_ready;
            break;
        case robot::v1::PackageStatus_State_STATE_FAILED:
            ps.state = RobotClient::package_status::package_state::k_failed;
            break;
        case robot::v1::PackageStatus_State_STATE_UNSPECIFIED:
        default:
            ps.state = RobotClient::package_status::package_state::k_unspecified;
    }
    ps.error = proto->error();
    if (proto->has_last_updated()) {
        ps.last_updated = from_proto(proto->last_updated());
    }
    ps.version = proto->version();
    ps.bytes_downloaded = proto->bytes_downloaded();
    ps.total_bytes = proto->total_bytes();
    return ps;
}

RobotClient::job_status from_proto_impl<JobStatus>::operator()(const JobStatus* proto) const {
    RobotClient::job_status js;
    js.job_name = proto->job_name();
    js.recent_successful_runs = impl::from_repeated_field(proto->recent_successful_runs());
    js.recent_failed_runs = impl::from_repeated_field(proto->recent_failed_runs());
    return js;
}

RobotClient::machine_status from_proto_impl<GetMachineStatusResponse>::operator()(
    const GetMachineStatusResponse* proto) const {
    RobotClient::machine_status ms;
    switch (proto->state()) {
        case robot::v1::GetMachineStatusResponse_State_STATE_INITIALIZING:
            ms.state = RobotClient::status::k_initializing;
            break;
        case robot::v1::GetMachineStatusResponse_State_STATE_RUNNING:
            ms.state = RobotClient::status::k_running;
            break;
        case robot::v1::GetMachineStatusResponse_State_STATE_UNSPECIFIED:
        default:
            ms.state = RobotClient::status::k_unspecified;
    }
    ms.resources = impl::from_repeated_field(proto->resources());
    ms.config = from_proto(proto->config());
    ms.modules = impl::from_repeated_field(proto->modules());
    ms.packages = impl::from_repeated_field(proto->packages());
    ms.job_statuses = impl::from_repeated_field(proto->job_statuses());
    return ms;
}

}  // namespace proto_convert_details

bool operator==(const RobotClient::frame_system_config& lhs,
                const RobotClient::frame_system_config& rhs) {
    return lhs.frame == rhs.frame && to_proto(lhs.kinematics).SerializeAsString() ==
                                         to_proto(rhs.kinematics).SerializeAsString();
}

bool operator==(const RobotClient::operation& lhs, const RobotClient::operation& rhs) {
    return lhs.id == rhs.id && lhs.method == rhs.method && lhs.session_id == rhs.session_id &&
           lhs.arguments == rhs.arguments && lhs.started == rhs.started;
}

bool operator==(const RobotClient::resource_status& lhs, const RobotClient::resource_status& rhs) {
    return lhs.name == rhs.name && lhs.state == rhs.state && lhs.last_updated == rhs.last_updated &&
           lhs.revision == rhs.revision && lhs.error == rhs.error;
}

bool operator==(const RobotClient::config_status& lhs, const RobotClient::config_status& rhs) {
    return lhs.revision == rhs.revision && lhs.last_updated == rhs.last_updated;
}

bool operator==(const RobotClient::module_status& lhs, const RobotClient::module_status& rhs) {
    return lhs.module_name == rhs.module_name && lhs.state == rhs.state &&
           lhs.last_updated == rhs.last_updated && lhs.error == rhs.error &&
           lhs.consecutive_failures == rhs.consecutive_failures;
}

bool operator==(const RobotClient::package_status& lhs, const RobotClient::package_status& rhs) {
    return lhs.name == rhs.name && lhs.type == rhs.type && lhs.state == rhs.state &&
           lhs.error == rhs.error && lhs.last_updated == rhs.last_updated &&
           lhs.version == rhs.version && lhs.bytes_downloaded == rhs.bytes_downloaded &&
           lhs.total_bytes == rhs.total_bytes;
}

bool operator==(const RobotClient::job_status& lhs, const RobotClient::job_status& rhs) {
    return lhs.job_name == rhs.job_name &&
           lhs.recent_successful_runs == rhs.recent_successful_runs &&
           lhs.recent_failed_runs == rhs.recent_failed_runs;
}

bool operator==(const RobotClient::machine_status& lhs, const RobotClient::machine_status& rhs) {
    return lhs.state == rhs.state && lhs.resources == rhs.resources && lhs.config == rhs.config &&
           lhs.modules == rhs.modules && lhs.packages == rhs.packages &&
           lhs.job_statuses == rhs.job_statuses;
}

struct RobotClient::impl {
    impl(std::unique_ptr<RobotService::Stub> stub, ViamChannel& channel)
        : stub(std::move(stub)), channel_(&channel) {}

    ~impl() {
        if (log_sink) {
            boost::log::core::get()->remove_sink(log_sink);
            LogManager::get().enable_console_logging();
        }
        sdk::impl::Tracer::get().shutdown_provider();
    }

    template <typename Method>
    static auto client_helper(const std::unique_ptr<impl>& self, Method m) {
        if (!self) {
            throw std::runtime_error(
                "Tried to call RobotClient method while not connected to robot");
        }
        return make_client_helper(self.get(), *self->stub, m);
    }

    const ViamChannel& channel() const {
        return *channel_;
    }

    std::unique_ptr<RobotService::Stub> stub;
    const ViamChannel* channel_;

    // See doc comment for RobotClient::connect_logging. This pointer is non-null and installed as a
    // sink only for apps being run by viam-server as a module.
    boost::shared_ptr<viam::sdk::impl::SinkType> log_sink;
};

void RobotClient::connect_logging() {
    auto& sink = impl_->log_sink;
    if (!sink) {
        sink = sdk::impl::LogBackend::create(this);
        sink->set_filter([filter = LogManager::Filter{&LogManager::get()}](
                             const boost::log::attribute_value_set& attrs) {
            auto force = attrs[sdk::impl::attr_console_force_type{}];
            if (force && *force) {
                return false;
            }

            return filter(attrs);
        });

        LogManager::get().disable_console_logging();
        boost::log::core::get()->add_sink(sink);
    }
}

RobotClient::~RobotClient() {
    try {
        this->close();
    } catch (const std::exception& e) {
        VIAM_SDK_LOG(error) << "Received err while closing RobotClient: " << e.what();
    } catch (...) {
        VIAM_SDK_LOG(error) << "Received unknown err while closing RobotClient";
    }
}

void RobotClient::close() {
    should_refresh_.store(false);
    should_check_connection_.store(false);

    if (refresh_thread_.joinable()) {
        refresh_thread_.join();
    }

    if (check_connection_thread_.joinable()) {
        check_connection_thread_.join();
    }

    stop_all();

    viam_channel_.close();

    impl_.reset();
}

bool is_error_response(const grpc::Status& response) {
    return !response.ok() && (response.error_message() != kStreamRemoved);
}

std::vector<RobotClient::operation> RobotClient::get_operations() {
    return impl::client_helper(impl_, &RobotService::Stub::GetOperations)
        .invoke(
            [](auto& response) { return sdk::impl::from_repeated_field(response.operations()); });
}

void RobotClient::cancel_operation(std::string id) {
    return impl::client_helper(impl_, &RobotService::Stub::CancelOperation)
        .with([&id](auto& req) { req.set_id(std::move(id)); })
        .invoke();
}

void RobotClient::block_for_operation(std::string id) {
    return impl::client_helper(impl_, &RobotService::Stub::BlockForOperation)
        .with([&id](auto& req) { req.set_id(std::move(id)); })
        .invoke();
}

void RobotClient::refresh() {
    auto resources =
        impl::client_helper(impl_, &RobotService::Stub::ResourceNames).invoke([](auto& response) {
            return response.resources();
        });

    std::unordered_map<Name, std::shared_ptr<Resource>> new_resources;
    std::vector<Name> current_resources;
    for (const auto& name : resources) {
        current_resources.push_back(from_proto(name));
        if (name.subtype() == "remote") {
            continue;
        }

        const std::shared_ptr<const ResourceClientRegistration> rs =
            Registry::get().lookup_resource_client(
                {name.namespace_(), name.type(), name.subtype()});
        if (rs) {
            try {
                const std::shared_ptr<Resource> rpc_client =
                    rs->create_rpc_client(name.name(), viam_channel_);
                const Name name_({name.namespace_(), name.type(), name.subtype()}, "", name.name());
                new_resources.emplace(name_, rpc_client);
            } catch (const std::exception& exc) {
                VIAM_SDK_LOG(debug)
                    << "Error registering component " << name.subtype() << ": " << exc.what();
            }
        }
    }
    if (current_resources == resource_names_) {
        return;
    }

    const std::lock_guard<std::mutex> lock(lock_);
    resource_names_ = std::move(current_resources);
    this->resource_manager_.replace_all(new_resources);
}

void RobotClient::refresh_every() {
    while (should_refresh_.load()) {
        try {
            std::this_thread::sleep_for(refresh_interval_);
            refresh();

        } catch (std::exception&) {
            break;
        }
    }
};

void RobotClient::check_connection() {
    auto check_every = check_every_interval_;
    auto reconnect_every = reconnect_every_interval_;
    if (check_every == std::chrono::seconds{0}) {
        check_every = reconnect_every;
    }
    if (check_every == std::chrono::seconds{0} && reconnect_every == std::chrono::seconds{0}) {
        should_check_connection_.store(false);
    }
    bool connected(true);
    while (should_check_connection_) {
        std::exception_ptr connection_error;
        std::string what;
        for (int i = 0; i < 3; ++i) {
            try {
                std::this_thread::sleep_for(check_every);
                impl::client_helper(impl_, &RobotService::Stub::ResourceNames).invoke([](auto&) {
                    return;
                });
                connected = true;
                break;
            } catch (const std::exception& e) {
                connected = false;
                connection_error = std::current_exception();
                what = e.what();
                std::this_thread::sleep_for(std::chrono::milliseconds{100});
            }
        }
        if (connected) {
            continue;
        }
        const auto* uri = viam_channel_.get_channel_addr();
        VIAM_SDK_LOG(error) << "Lost connection to machine at address " << uri << " with error "
                            << what << ". Attempting to reconnect every " << reconnect_every.count()
                            << "second(s)";

        viam_channel_.close();

        for (int i = 0; i < 3; ++i) {
            try {
                viam_channel_ = ViamChannel::dial(uri, {});
                impl_ = std::make_unique<RobotClient::impl>(
                    RobotService::NewStub(viam_channel_.channel()), viam_channel_);
                refresh();
                connected = true;
            } catch (const std::exception& e) {
                viam_channel_.close();
                std::this_thread::sleep_for(reconnect_every);
            }
        }
        if (!connected) {
            close();
        }
    }
}

RobotClient::RobotClient(ViamChannel channel)
    : viam_channel_(std::move(channel)),
      impl_(std::make_unique<impl>(RobotService::NewStub(viam_channel_.channel()), viam_channel_)) {
}

std::vector<Name> RobotClient::resource_names() const {
    const std::lock_guard<std::mutex> lock(lock_);
    return resource_names_;
}

void RobotClient::log(const std::string& name,
                      const std::string& level,
                      const std::string& message,
                      time_pt time,
                      const std::string& file,
                      unsigned int line) {
    if (!impl_) {
        throw std::runtime_error("Tried to send logs to robot when it was not connected");
    }

    // Do client request/response setup manually so we can override the usual exception handling
    robot::v1::LogRequest req;
    common::v1::LogEntry log;

    *log.mutable_logger_name() = name;
    log.set_level(level);
    *log.mutable_message() = message;
    *log.mutable_time() = to_proto(time);

    auto& fields = *log.mutable_caller()->mutable_fields();
    fields["Defined"].set_bool_value(true);
    fields["File"].set_string_value(file);
    fields["Line"].set_number_value(line);

    req.mutable_logs()->Add(std::move(log));

    robot::v1::LogResponse resp;
    ClientContext ctx;
    const auto response = impl_->stub->Log(ctx, req, &resp);
    if (is_error_response(response)) {
        // Manually override to force this to get logged to console so we don't set off an
        // infinite loop
        VIAM_SDK_LOG(error) << boost::log::add_value(sdk::impl::attr_console_force_type{}, true)
                            << "Error sending log message over grpc: " << response.error_message()
                            << response.error_details();
    }
}

bool RobotClient::send_traces(const robot::v1::SendTracesRequest* req) {
    if (!impl_) {
        return false;
    }
    robot::v1::SendTracesResponse resp;
    ClientContext ctx;
    return impl_->stub->SendTraces(ctx, *req, &resp).ok();
}

void RobotClient::connect_tracing() {
    sdk::impl::Tracer::get().initialize_provider(this);
}

std::shared_ptr<RobotClient> RobotClient::with_channel(ViamChannel channel,
                                                       const Options& options) {
    auto robot = std::make_shared<RobotClient>(std::move(channel));
    robot->refresh_interval_ = std::chrono::seconds{options.refresh_interval()};
    robot->should_refresh_ = (robot->refresh_interval_ > std::chrono::seconds{0});
    if (robot->should_refresh_) {
        robot->refresh_thread_ = std::thread{&RobotClient::refresh_every, robot.get()};
    }

    robot->should_check_connection_ = true;

    robot->check_every_interval_ = options.check_every_interval();
    robot->reconnect_every_interval_ = options.reconnect_every_interval();

    robot->check_connection_thread_ = std::thread{&RobotClient::check_connection, robot.get()};

    robot->refresh();
    return robot;
};

std::shared_ptr<RobotClient> RobotClient::at_address(const std::string& address,
                                                     const Options& options) {
    const char* uri = address.c_str();
    auto robot = RobotClient::with_channel(
        ViamChannel::dial_initial(uri, options.channel_options()), options);

    return robot;
};

std::shared_ptr<RobotClient> RobotClient::at_local_socket(const std::string& address,
                                                          const Options& options) {
    // TODO (RSDK-10720) - refactor/replace `at_local_socket`
    auto robot = RobotClient::with_channel(ViamChannel(sdk::impl::create_viam_grpc_channel(
                                               address, grpc::InsecureChannelCredentials())),
                                           options);

    return robot;
};

std::vector<RobotClient::frame_system_config> RobotClient::get_frame_system_config(
    const std::vector<WorldState::transform>& additional_transforms) {
    return impl::client_helper(impl_, &RobotService::Stub::FrameSystemConfig)
        .with([&additional_transforms](auto& req) {
            *(req.mutable_supplemental_transforms()) =
                sdk::impl::to_repeated_field(additional_transforms);
        })
        .invoke(
            [](auto& resp) { return sdk::impl::from_repeated_field(resp.frame_system_configs()); });
}

pose_in_frame RobotClient::transform_pose(
    const pose_in_frame& query,
    std::string destination,
    const std::vector<WorldState::transform>& additional_transforms) {
    return impl::client_helper(impl_, &RobotService::Stub::TransformPose)
        .with([&](auto& req) {
            *req.mutable_source() = to_proto(query);
            *req.mutable_destination() = std::move(destination);
            *req.mutable_supplemental_transforms() =
                sdk::impl::to_repeated_field(additional_transforms);
        })
        .invoke([](const auto& resp) { return from_proto(resp.pose()); });
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
    return impl::client_helper(impl_, &RobotService::Stub::StopAll)
        .with([&](auto& req) {
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
        })
        .invoke();
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

pose_in_frame RobotClient::get_pose(const std::string& component_name) {
    return get_pose(component_name, "world", {}, {});
}

pose_in_frame RobotClient::get_pose(const std::string& component_name,
                                    const std::string& destination_frame,
                                    const std::vector<WorldState::transform>& additional_transforms,
                                    const ProtoStruct& extra) {
    return impl::client_helper(impl_, &RobotService::Stub::GetPose)
        .with([&](auto& req) {
            req.set_component_name(component_name);
            req.set_destination_frame(destination_frame);
            *req.mutable_supplemental_transforms() =
                sdk::impl::to_repeated_field(additional_transforms);
            *req.mutable_extra() = to_proto(extra);
        })
        .invoke([](const auto& resp) { return from_proto(resp.pose()); });
}

RobotClient::machine_status RobotClient::get_machine_status() const {
    return impl::client_helper(impl_, &RobotService::Stub::GetMachineStatus)
        .invoke([](const auto& resp) { return from_proto(resp); });
}

}  // namespace sdk
}  // namespace viam
