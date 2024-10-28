#include <chrono>
#include <viam/sdk/module/service.hpp>

#include <exception>
#include <memory>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>

#include <boost/core/null_deleter.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/none.hpp>
#include <google/protobuf/descriptor.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>

#include <viam/api/app/v1/robot.pb.h>
#include <viam/api/module/v1/module.grpc.pb.h>
#include <viam/api/module/v1/module.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/logger.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/handler_map.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/stoppable.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

namespace logging = boost::log;

Dependencies ModuleService::get_dependencies_(
    google::protobuf::RepeatedPtrField<std::string> const& proto,
    std::string const& resource_name) {
    Dependencies deps;
    for (const auto& dep : proto) {
        auto dep_name = Name::from_string(dep);
        const std::shared_ptr<Resource> dep_resource = get_parent_resource_(dep_name);
        if (!dep_resource) {
            std::ostringstream buffer;
            buffer << resource_name << ": Dependency "
                   << "`" << dep_name << "` was not found during (re)configuration";
            throw Exception(ErrorCondition::k_resource_not_found, buffer.str());
        }
        deps.emplace(dep_name, dep_resource);
    }
    return deps;
}

std::shared_ptr<Resource> ModuleService::get_parent_resource_(const Name& name) {
    if (!parent_) {
        parent_ = RobotClient::at_local_socket(parent_addr_, {0, boost::none});
    }

    return parent_->resource_by_name(name);
}

// TODO(RSDK-6528) - to the extent possible, switch to using `server_helper`
::grpc::Status ModuleService::AddResource(::grpc::ServerContext*,
                                          const ::viam::module::v1::AddResourceRequest* request,
                                          ::viam::module::v1::AddResourceResponse*) {
    const viam::app::v1::ComponentConfig& proto = request->config();
    const ResourceConfig cfg = ResourceConfig::from_proto(proto);
    const std::lock_guard<std::mutex> lock(lock_);

    std::shared_ptr<Resource> res;
    const Dependencies deps = get_dependencies_(request->dependencies(), cfg.name());
    const std::shared_ptr<const ModelRegistration> reg =
        Registry::lookup_model(cfg.api(), cfg.model());
    if (reg) {
        try {
            res = reg->construct_resource(deps, cfg);
        } catch (const std::exception& exc) {
            return grpc::Status(::grpc::INTERNAL, exc.what());
        }
    };
    try {
        server_->add_resource(res);
    } catch (const std::exception& exc) {
        return grpc::Status(::grpc::INTERNAL, exc.what());
    }

    return grpc::Status();
};

::grpc::Status ModuleService::ReconfigureResource(
    ::grpc::ServerContext*,
    const ::viam::module::v1::ReconfigureResourceRequest* request,
    ::viam::module::v1::ReconfigureResourceResponse*) {
    const viam::app::v1::ComponentConfig& proto = request->config();
    ResourceConfig cfg = ResourceConfig::from_proto(proto);

    const Dependencies deps = get_dependencies_(request->dependencies(), cfg.name());

    auto resource_server = server_->lookup_resource_server(cfg.api());
    if (!resource_server) {
        return grpc::Status(grpc::UNKNOWN, "no rpc service for config: " + cfg.api().to_string());
    }
    auto manager = resource_server->resource_manager();

    // see if our resource is reconfigurable. if it is, reconfigure
    const std::shared_ptr<Resource> res = manager->resource(cfg.resource_name().name());
    if (!res) {
        return grpc::Status(grpc::UNKNOWN,
                            "unable to reconfigure resource " + cfg.resource_name().name() +
                                " as it doesn't exist.");
    }
    try {
        Reconfigurable::reconfigure_if_reconfigurable(res, deps, cfg);
        return grpc::Status();
    } catch (const std::exception& exc) {
        return grpc::Status(::grpc::INTERNAL, exc.what());
    }

    // if the type isn't reconfigurable by default, replace it
    try {
        Stoppable::stop_if_stoppable(res);
    } catch (const std::exception& err) {
        BOOST_LOG_TRIVIAL(error) << "unable to stop resource: " << err.what();
    }

    const std::shared_ptr<const ModelRegistration> reg = Registry::lookup_model(cfg.name());
    if (reg) {
        try {
            const std::shared_ptr<Resource> res = reg->construct_resource(deps, cfg);
            manager->replace_one(cfg.resource_name(), res);
        } catch (const std::exception& exc) {
            return grpc::Status(::grpc::INTERNAL, exc.what());
        }
    }

    return grpc::Status();
};

::grpc::Status ModuleService::ValidateConfig(
    ::grpc::ServerContext*,
    const ::viam::module::v1::ValidateConfigRequest* request,
    ::viam::module::v1::ValidateConfigResponse* response) {
    const viam::app::v1::ComponentConfig& proto = request->config();
    ResourceConfig cfg = ResourceConfig::from_proto(proto);

    const std::shared_ptr<const ModelRegistration> reg =
        Registry::lookup_model(cfg.api(), cfg.model());
    if (!reg) {
        return grpc::Status(grpc::UNKNOWN,
                            "unable to validate resource " + cfg.resource_name().name() +
                                " as it hasn't been registered.");
    }
    try {
        const std::vector<std::string> implicit_deps = reg->validate(cfg);
        for (const auto& dep : implicit_deps) {
            response->add_dependencies(dep);
        }
    } catch (const std::exception& err) {
        return grpc::Status(grpc::UNKNOWN,
                            "validation failure in resource " + cfg.name() + ": " + err.what());
    }
    return grpc::Status();
};

::grpc::Status ModuleService::RemoveResource(
    ::grpc::ServerContext*,
    const ::viam::module::v1::RemoveResourceRequest* request,
    ::viam::module::v1::RemoveResourceResponse*) {
    auto name = Name::from_string(request->name());
    auto resource_server = server_->lookup_resource_server(name.api());
    if (!resource_server) {
        return grpc::Status(grpc::UNKNOWN, "no grpc service for " + name.api().to_string());
    }
    const std::shared_ptr<ResourceManager> manager = resource_server->resource_manager();
    const std::shared_ptr<Resource> res = manager->resource(name.name());
    if (!res) {
        return grpc::Status(
            grpc::UNKNOWN,
            "unable to remove resource " + name.to_string() + " as it doesn't exist.");
    }

    try {
        Stoppable::stop_if_stoppable(res);
    } catch (const std::exception& err) {
        BOOST_LOG_TRIVIAL(error) << "unable to stop resource: " << err.what();
    }

    manager->remove(name);
    return grpc::Status();
};

::grpc::Status ModuleService::Ready(::grpc::ServerContext*,
                                    const ::viam::module::v1::ReadyRequest* request,
                                    ::viam::module::v1::ReadyResponse* response) {
    const std::lock_guard<std::mutex> lock(lock_);
    const viam::module::v1::HandlerMap hm = this->module_->handles().to_proto();
    *response->mutable_handlermap() = hm;
    auto new_parent_addr = request->parent_address();
    if (parent_addr_ != new_parent_addr) {
        parent_addr_ = std::move(new_parent_addr);
        if (!parent_) {
            parent_ = RobotClient::at_local_socket(parent_addr_, {0, boost::none});
        }
        init_logging_();
    }
    response->set_ready(module_->ready());
    return grpc::Status();
};

namespace impl {
class custom_logging_buf : public std::stringbuf {
   public:
    custom_logging_buf(std::shared_ptr<RobotClient> parent) : parent_(std::move(parent)) {};
    int sync() override {
        if (!parent_) {
            std::cerr << "Attempted to send custom gRPC log without parent address\n";
            return -1;
        }

        std::cout << "here!";
        auto name_attr =
            boost::log::attribute_cast<boost::log::attributes::mutable_constant<std::string>>(
                boost::log::core::get()->get_thread_attributes()["LoggerName"]);

        std::string name = name_attr.get();
        auto level_attr =
            boost::log::attribute_cast<boost::log::attributes::mutable_constant<std::string>>(
                boost::log::core::get()->get_thread_attributes()["LogLevel"]);

        std::string level = level_attr.get();
        if (name == "" || level == "") {
            // logging is not being done from `Viam` logger but is still going through `Boost`.
            // Because we use `Boost` as our backend and are overwriting defaults, this log will
            // still go to `viam-server`. But, since the call is not made through our logger,
            // we don't want to rely on a previously set level or logger name. So, we fall back
            // on default values.
            name = "module_service";
            level = "info";
        }

        // reset name and level attribute to "" in case a subsequent logging call is made through
        // a boost macro directly, without using our loggers.
        name_attr.set("");
        level_attr.set("");

        auto log = this->str();
        // Boost loves to add newline chars at the end of log messages, but this causes RDK
        // logs to break out over multiple lines, which isn't great. Since we break up the structure
        // of our log messages into two parts for module logging purposes, we need to remove the
        // final character (always a newline) and then the first occurrence of a newline after that
        // if it exists.
        log.pop_back();
        auto first_newline_char = log.find('\n', 0);
        if (first_newline_char != std::string::npos) {
            log.erase(first_newline_char, 1);
        }

        parent_->log(std::move(name),
                     std::move(level),
                     std::move(log),
                     std::chrono::system_clock::now(),
                     "None");
        return 0;
    }

   private:
    std::shared_ptr<RobotClient> parent_;
};
}  // namespace impl

struct ModuleService::impl {
    typedef logging::sinks::synchronous_sink<logging::sinks::text_ostream_backend> text_sink;
    impl(std::shared_ptr<RobotClient> parent)
        : buf(::viam::sdk::impl::custom_logging_buf(std::move(parent))) {
        auto backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
        auto strm = boost::make_shared<std::ostream>(&buf);
        stream = strm;
        backend->add_stream(strm);
        backend->auto_flush(true);
        sink = boost::make_shared<text_sink>(backend);
    }

    ::viam::sdk::impl::custom_logging_buf buf;
    boost::shared_ptr<text_sink> sink;
    boost::shared_ptr<std::ostream> stream;
};

void ModuleService::init_logging_() {
    BOOST_LOG_TRIVIAL(error) << "error log the old way";
    impl_ = std::make_unique<impl>(parent_);
    init_logging(*impl_->stream);
    logging::core::get()->add_sink(impl_->sink);
}

ModuleService::ModuleService(std::string addr)
    : module_(std::make_unique<Module>(std::move(addr))), server_(std::make_unique<Server>()) {}

ModuleService::ModuleService(int argc,
                             char** argv,
                             const std::vector<std::shared_ptr<ModelRegistration>>& registrations) {
    if (argc < 2) {
        throw Exception(ErrorCondition::k_connection, "Need socket path as command line argument");
    }
    module_ = std::make_unique<Module>(argv[1]);
    server_ = std::make_unique<Server>();
    signal_manager_ = SignalManager();
    set_logger_severity_from_args(argc, argv);

    for (auto&& mr : registrations) {
        Registry::register_model(mr);
        add_model_from_registry(mr->api(), mr->model());
    }
}

void ModuleService::serve() {
    const mode_t old_mask = umask(0077);
    const int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    listen(sockfd, 10);
    umask(old_mask);

    server_->register_service(this);
    const std::string address = "unix://" + module_->addr();
    server_->add_listening_port(address);

    module_->set_ready();
    server_->start();
    BOOST_LOG_TRIVIAL(info) << "Module listening on " << module_->addr();
    BOOST_LOG_TRIVIAL(info) << "Module handles the following API/model pairs:\n"
                            << module_->handles();

    signal_manager_.wait();
}

ModuleService::~ModuleService() {
    // TODO(RSDK-5509): Run registered cleanup functions here.
    // CR erodkin: this is breaking because of level parsing, fix that
    // BOOST_LOG_TRIVIAL(info) << "Shutting down gracefully.";
    server_->shutdown();

    if (parent_) {
        try {
            parent_->close();
        } catch (const std::exception& exc) {
            BOOST_LOG_TRIVIAL(error) << exc.what();
        }
    }
}

void ModuleService::add_model_from_registry_inlock_(API api,
                                                    Model model,
                                                    const std::lock_guard<std::mutex>&) {
    const std::shared_ptr<const ResourceServerRegistration> creator =
        Registry::lookup_resource_server(api);
    std::string name;
    const google::protobuf::ServiceDescriptor* sd = nullptr;
    if (creator && creator->service_descriptor()) {
        name = creator->service_descriptor()->full_name();
        sd = creator->service_descriptor();
    }
    const RPCSubtype rpc_subtype(std::move(api), name, *sd);
    module_->mutable_handles().add_model(std::move(model), rpc_subtype);
};

void ModuleService::add_model_from_registry(API api, Model model) {
    const std::lock_guard<std::mutex> lock(lock_);
    return add_model_from_registry_inlock_(std::move(api), std::move(model), lock);
}

}  // namespace sdk
}  // namespace viam
