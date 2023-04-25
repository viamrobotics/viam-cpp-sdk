#include <viam/sdk/module/service.hpp>

#include <csignal>
#include <iostream>
#include <memory>
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
#include <viam/sdk/components/component_base.hpp>
#include <viam/sdk/components/service_base.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/handler_map.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_base.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/subtype/subtype.hpp>

namespace viam {
namespace sdk {

namespace {
Dependencies get_dependencies(ModuleService_* m,
                              google::protobuf::RepeatedPtrField<std::string> proto) {
    Dependencies deps;
    for (auto& dep : proto) {
        auto name = Name::from_string(dep);
        std::shared_ptr<ResourceBase> resource = m->get_parent_resource(name);
        deps.emplace(name, resource);
    }
    return deps;
}
}  // namespace
   //
std::shared_ptr<ResourceBase> ModuleService_::get_parent_resource(Name name) {
    if (!parent_) {
        parent_ = RobotClient::at_local_socket(parent_addr_, {0, boost::none});
    }

    return parent_->resource_by_name(name.to_proto());
}

::grpc::Status ModuleService_::AddResource(::grpc::ServerContext* context,
                                           const ::viam::module::v1::AddResourceRequest* request,
                                           ::viam::module::v1::AddResourceResponse* response) {
    viam::app::v1::ComponentConfig proto = request->config();
    Resource cfg = Resource::from_proto(proto);
    std::shared_ptr<Module> module = this->module_;
    module->lock().lock();

    std::shared_ptr<ResourceBase> res;
    Dependencies deps = get_dependencies(this, request->dependencies());
    std::shared_ptr<ModelRegistration> reg = Registry::lookup_resource(cfg.api(), cfg.model());
    if (reg) {
        res = reg->construct_resource(deps, cfg);
    };
    if (module->services().find(cfg.api()) == module->services().end()) {
        module->lock().unlock();
        return grpc::Status(grpc::UNKNOWN, "module cannot service api " + cfg.api().to_string());
    }

    std::shared_ptr<SubtypeService> sub_svc = module->services().at(cfg.api());
    sub_svc->add(cfg.resource_name(), res);

    module->lock().unlock();
    return grpc::Status();
};

::grpc::Status ModuleService_::ReconfigureResource(
    ::grpc::ServerContext* context,
    const ::viam::module::v1::ReconfigureResourceRequest* request,
    ::viam::module::v1::ReconfigureResourceResponse* response) {
    viam::app::v1::ComponentConfig proto = request->config();
    Resource cfg = Resource::from_proto(proto);
    std::shared_ptr<Module> module = this->module_;

    Dependencies deps = get_dependencies(this, request->dependencies());

    if (module->services().find(cfg.api()) == module->services().end()) {
        return grpc::Status(grpc::UNKNOWN, "no rpc service for config: " + cfg.api().to_string());
    }
    std::shared_ptr<SubtypeService> sub_svc = module->services().at(cfg.api());

    // see if our resource is reconfigurable. if it is, reconfigure
    std::shared_ptr<ResourceBase> res = sub_svc->resource(cfg.resource_name().name());
    if (!res) {
        return grpc::Status(grpc::UNKNOWN,
                            "unable to reconfigure resource " + cfg.resource_name().name() +
                                " as it doesn't exist.");
    }
    try {
        res->reconfigure(deps, cfg);
        return grpc::Status();
    } catch (std::exception& exc) {
    }

    // if the type isn't reconfigurable by default, replace it
    try {
        res->stop();
    } catch (std::string err) {
        BOOST_LOG_TRIVIAL(error) << "unable to stop resource: " << err;
    }

    std::shared_ptr<ModelRegistration> reg = Registry::lookup_resource(cfg.name());
    if (reg) {
        std::shared_ptr<ResourceBase> res = reg->construct_resource(deps, cfg);
        sub_svc->replace_one(cfg.resource_name(), res);
    }

    return grpc::Status();
};

::grpc::Status ModuleService_::ValidateConfig(
    ::grpc::ServerContext* context,
    const ::viam::module::v1::ValidateConfigRequest* request,
    ::viam::module::v1::ValidateConfigResponse* response) {
    viam::app::v1::ComponentConfig proto = request->config();
    Resource cfg = Resource::from_proto(proto);

    std::shared_ptr<ModelRegistration> reg = Registry::lookup_resource(cfg.api(), cfg.model());
    if (!reg) {
        return grpc::Status(grpc::UNKNOWN,
                            "unable to validate resource " + cfg.resource_name().name() +
                                " as it hasn't been registered.");
    }
    try {
        std::vector<std::string> implicit_deps = reg->validate(cfg);
        for (auto& dep : implicit_deps) {
            response->add_dependencies(dep);
        }
    } catch (std::string err) {
        return grpc::Status(grpc::UNKNOWN,
                            "validation failure in resource " + cfg.name() + ": " + err);
    }
    return grpc::Status();
};

::grpc::Status ModuleService_::RemoveResource(
    ::grpc::ServerContext* context,
    const ::viam::module::v1::RemoveResourceRequest* request,
    ::viam::module::v1::RemoveResourceResponse* response) {
    std::shared_ptr<Module> m = this->module_;
    auto name = Name::from_string(request->name());
    const Subtype* subtype = name.to_subtype();
    if (m->services().find(*subtype) == m->services().end()) {
        return grpc::Status(grpc::UNKNOWN, "no grpc service for " + subtype->to_string());
    }
    std::shared_ptr<SubtypeService> svc = m->services().at(*name.to_subtype());
    std::shared_ptr<ResourceBase> res = svc->resource(name.name());
    if (!res) {
        return grpc::Status(
            grpc::UNKNOWN,
            "unable to remove resource " + name.to_string() + " as it doesn't exist.");
    }

    try {
        res->stop();
    } catch (std::string err) {
        BOOST_LOG_TRIVIAL(error) << "unable to stop resource: " << err;
    }

    svc->remove(name);
    return grpc::Status();
};

::grpc::Status ModuleService_::Ready(::grpc::ServerContext* context,
                                     const ::viam::module::v1::ReadyRequest* request,
                                     ::viam::module::v1::ReadyResponse* response) {
    module_->lock().lock();
    viam::module::v1::HandlerMap hm = this->module_->handles().to_proto();
    *response->mutable_handlermap() = hm;
    parent_addr_ = request->parent_address();
    module_->lock().unlock();
    response->set_ready(module_->ready());
    return grpc::Status();
};

ModuleService_::ModuleService_(std::string addr) {
    module_ = std::make_shared<Module>(addr);
}

void ModuleService_::start(std::shared_ptr<Server> server) {
    module_->lock().lock();
    mode_t old_mask = umask(0077);
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    listen(sockfd, 10);
    umask(old_mask);

    // TODO(RSDK-1742) see if we can/want to do this in an init instead
    server->register_service(this);
    std::string address = "unix://" + module_->addr();
    server->add_listening_port(address);

    module_->lock().unlock();
    module_->set_ready();
}

ModuleService_::~ModuleService_() {
    this->close();
}

void ModuleService_::close() {
    BOOST_LOG_TRIVIAL(info) << "Shutting down gracefully.";

    if (parent_) {
        try {
            parent_->close();
        } catch (std::exception& exc) {
            BOOST_LOG_TRIVIAL(error) << exc.what();
        }
    }
}

void ModuleService_::add_api_from_registry(std::shared_ptr<Server> server, Subtype api) {
    if (module_->services().find(api) != module_->services().end()) {
        return;
    }
    module_->lock().lock();
    std::shared_ptr<SubtypeService> new_svc = std::make_shared<SubtypeService>();

    std::shared_ptr<ResourceSubtype> rs = Registry::lookup_subtype(api);
    std::shared_ptr<ResourceServerBase> resource_server = rs->create_resource_server(new_svc);
    resource_server->register_server(server);
    module_->services().emplace(api, new_svc);
    module_->servers().push_back(resource_server);
    module_->lock().unlock();
}

void ModuleService_::add_model_from_registry(std::shared_ptr<Server> server,
                                             Subtype api,
                                             Model model) {
    if (module_->services().find(api) == module_->services().end()) {
        add_api_from_registry(server, api);
    }

    std::shared_ptr<ResourceSubtype> creator = Registry::lookup_subtype(api);
    std::string name;
    const google::protobuf::ServiceDescriptor* sd = nullptr;
    if (creator && creator->service_descriptor()) {
        name = creator->service_descriptor()->full_name();
        sd = creator->service_descriptor();
    }
    RPCSubtype rpc_subtype(api, name, *sd);
    module_->handles().add_model(model, rpc_subtype);
};

}  // namespace sdk
}  // namespace viam
