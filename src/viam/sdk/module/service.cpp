#include <viam/sdk/module/service.hpp>

#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>

#include <boost/log/trivial.hpp>
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

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/handler_map.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/stoppable.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

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
            throw std::invalid_argument(buffer.str());
        }
        deps.emplace(dep_name, dep_resource);
    }
    return deps;
}

std::shared_ptr<Resource> ModuleService::get_parent_resource_(Name name) {
    if (!parent_) {
        parent_ = RobotClient::at_local_socket(parent_addr_, {0, boost::none});
    }

    return parent_->resource_by_name(name);
}

::grpc::Status ModuleService::AddResource(::grpc::ServerContext* context,
                                          const ::viam::module::v1::AddResourceRequest* request,
                                          ::viam::module::v1::AddResourceResponse* response) {
    const viam::app::v1::ComponentConfig& proto = request->config();
    ResourceConfig cfg = ResourceConfig::from_proto(proto);
    const std::lock_guard<std::mutex> lock(lock_);

    std::shared_ptr<Resource> res;
    const Dependencies deps = get_dependencies_(request->dependencies(), cfg.name());
    const std::shared_ptr<ModelRegistration> reg = Registry::lookup_model(cfg.api(), cfg.model());
    if (reg) {
        try {
            res = reg->construct_resource(deps, cfg);
        } catch (const std::exception& exc) {
            return grpc::Status(::grpc::INTERNAL, exc.what());
        }
    };
    const std::unordered_map<API, std::shared_ptr<ResourceManager>>& services = module_->services();
    if (services.find(cfg.api()) == services.end()) {
        return grpc::Status(grpc::UNKNOWN, "module cannot service api " + cfg.api().to_string());
    }

    const std::shared_ptr<ResourceManager> manager = services.at(cfg.api());
    manager->add(cfg.resource_name(), res);

    return grpc::Status();
};

::grpc::Status ModuleService::ReconfigureResource(
    ::grpc::ServerContext* context,
    const ::viam::module::v1::ReconfigureResourceRequest* request,
    ::viam::module::v1::ReconfigureResourceResponse* response) {
    const viam::app::v1::ComponentConfig& proto = request->config();
    ResourceConfig cfg = ResourceConfig::from_proto(proto);

    const Dependencies deps = get_dependencies_(request->dependencies(), cfg.name());

    const std::unordered_map<API, std::shared_ptr<ResourceManager>>& services = module_->services();
    if (services.find(cfg.api()) == services.end()) {
        return grpc::Status(grpc::UNKNOWN, "no rpc service for config: " + cfg.api().to_string());
    }
    const std::shared_ptr<ResourceManager> manager = services.at(cfg.api());

    // see if our resource is reconfigurable. if it is, reconfigure
    const std::shared_ptr<Resource> res = manager->resource(cfg.resource_name().name());
    if (!res) {
        return grpc::Status(grpc::UNKNOWN,
                            "unable to reconfigure resource " + cfg.resource_name().name() +
                                " as it doesn't exist.");
    }
    try {
        res->reconfigure(deps, cfg);
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

    const std::shared_ptr<ModelRegistration> reg = Registry::lookup_model(cfg.name());
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
    ::grpc::ServerContext* context,
    const ::viam::module::v1::ValidateConfigRequest* request,
    ::viam::module::v1::ValidateConfigResponse* response) {
    const viam::app::v1::ComponentConfig& proto = request->config();
    ResourceConfig cfg = ResourceConfig::from_proto(proto);

    const std::shared_ptr<ModelRegistration> reg = Registry::lookup_model(cfg.api(), cfg.model());
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
    ::grpc::ServerContext* context,
    const ::viam::module::v1::RemoveResourceRequest* request,
    ::viam::module::v1::RemoveResourceResponse* response) {
    auto name = Name::from_string(request->name());
    const std::unordered_map<API, std::shared_ptr<ResourceManager>>& services = module_->services();
    if (services.find(name.api()) == services.end()) {
        return grpc::Status(grpc::UNKNOWN, "no grpc service for " + name.api().to_string());
    }
    const std::shared_ptr<ResourceManager> manager = services.at(name.api());
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

::grpc::Status ModuleService::Ready(::grpc::ServerContext* context,
                                    const ::viam::module::v1::ReadyRequest* request,
                                    ::viam::module::v1::ReadyResponse* response) {
    const std::lock_guard<std::mutex> lock(lock_);
    const viam::module::v1::HandlerMap hm = this->module_->handles().to_proto();
    *response->mutable_handlermap() = hm;
    parent_addr_ = request->parent_address();
    response->set_ready(module_->ready());
    return grpc::Status();
};

ModuleService::ModuleService(std::string addr)
    : module_(std::make_unique<Module>(std::move(addr))), server_(std::make_shared<Server>()) {}

ModuleService::ModuleService(int argc,
                             char** argv,
                             std::vector<std::shared_ptr<ModelRegistration>> registrations) {
    if (argc < 2) {
        throw std::runtime_error("Need socket path as command line argument");
    }
    module_ = std::make_unique<Module>(argv[1]);
    server_ = std::make_shared<Server>();
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
    BOOST_LOG_TRIVIAL(info) << "Module handles the following API/model pairs: " << std::endl
                            << module_->handles();

    signal_manager_.wait();
}

ModuleService::~ModuleService() {
    // TODO(RSDK-5509): Run registered cleanup functions here.
    BOOST_LOG_TRIVIAL(info) << "Shutting down gracefully.";
    server_->shutdown();

    if (parent_) {
        try {
            parent_->close();
        } catch (const std::exception& exc) {
            BOOST_LOG_TRIVIAL(error) << exc.what();
        }
    }
}

void ModuleService::add_api_from_registry_inlock_(API api, const std::lock_guard<std::mutex>&) {
    const std::unordered_map<API, std::shared_ptr<ResourceManager>>& services = module_->services();
    if (services.find(api) != services.end()) {
        return;
    }
    auto new_manager = std::make_shared<ResourceManager>();

    const std::shared_ptr<ResourceRegistration> rs = Registry::lookup_resource(api);
    const std::shared_ptr<ResourceServer> resource_server = rs->create_resource_server(new_manager);
    resource_server->register_server(server_);
    module_->mutable_services().emplace(api, new_manager);
    module_->mutable_servers().push_back(resource_server);
}

void ModuleService::add_model_from_registry_inlock_(API api,
                                                    Model model,
                                                    const std::lock_guard<std::mutex>& lock) {
    const std::unordered_map<API, std::shared_ptr<ResourceManager>>& services = module_->services();
    if (services.find(api) == services.end()) {
        add_api_from_registry_inlock_(api, lock);
    }

    const std::shared_ptr<ResourceRegistration> creator = Registry::lookup_resource(api);
    std::string name;
    const google::protobuf::ServiceDescriptor* sd = nullptr;
    if (creator && creator->service_descriptor()) {
        name = creator->service_descriptor()->full_name();
        sd = creator->service_descriptor();
    }
    const RPCSubtype rpc_subtype(api, name, *sd);
    module_->mutable_handles().add_model(model, rpc_subtype);
};

void ModuleService::add_api_from_registry(API api) {
    const std::lock_guard<std::mutex> lock(lock_);
    return add_api_from_registry_inlock_(api, lock);
}

void ModuleService::add_model_from_registry(API api, Model model) {
    const std::lock_guard<std::mutex> lock(lock_);
    return add_model_from_registry_inlock_(api, model, lock);
}

}  // namespace sdk
}  // namespace viam
