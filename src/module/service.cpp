#include <grpcpp/server.h>

#include <csignal>
#include <iostream>

#include "google/protobuf/descriptor.h"
#define BOOST_LOG_DYN_LINK 1
#include <app/v1/robot.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>
#include <module/v1/module.grpc.pb.h>
#include <module/v1/module.pb.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <boost/log/trivial.hpp>
#include <boost/none.hpp>
#include <common/utils.hpp>
#include <components/component_base.hpp>
#include <components/service_base.hpp>
#include <config/resource.hpp>
#include <memory>
#include <module/handler_map.hpp>
#include <module/service.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <robot/client.hpp>
#include <robot/service.hpp>
#include <rpc/server.hpp>
#include <string>
#include <subtype/subtype.hpp>

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
    if (parent == nullptr) {
        parent = RobotClient::at_address("unix://" + parent_addr, {0, boost::none});
    }

    return parent->resource_by_name(name.to_proto());
}

::grpc::Status ModuleService_::AddResource(::grpc::ServerContext* context,
                                           const ::viam::module::v1::AddResourceRequest* request,
                                           ::viam::module::v1::AddResourceResponse* response) {
    viam::app::v1::ComponentConfig proto = request->config();
    Resource cfg = Resource::from_proto(proto);
    std::shared_ptr<Module> module = this->module;
    module->lock.lock();

    std::shared_ptr<ResourceBase> res;
    Dependencies deps = get_dependencies(this, request->dependencies());
    std::shared_ptr<ModelRegistration> reg = Registry::lookup_resource(cfg.api, cfg.model);
    if (reg != nullptr) {
        res = reg->construct_resource(deps, cfg);
    };

    if (module->services.find(cfg.api) == module->services.end()) {
        module->lock.unlock();
        return grpc::Status(grpc::UNKNOWN, "module cannot service api " + cfg.api.to_string());
    }

    std::shared_ptr<SubtypeService> sub_svc = module->services.at(cfg.api);
    sub_svc->add(cfg.resource_name(), res);

    module->lock.unlock();
    return grpc::Status();
};

::grpc::Status ModuleService_::ReconfigureResource(
    ::grpc::ServerContext* context,
    const ::viam::module::v1::ReconfigureResourceRequest* request,
    ::viam::module::v1::ReconfigureResourceResponse* response) {
    viam::app::v1::ComponentConfig proto = request->config();
    Resource cfg = Resource::from_proto(proto);
    std::shared_ptr<Module> module = this->module;

    Dependencies deps = get_dependencies(this, request->dependencies());

    if (module->services.find(cfg.api) == module->services.end()) {
        return grpc::Status(grpc::UNKNOWN, "no rpc service for config: " + cfg.api.to_string());
    }
    std::shared_ptr<SubtypeService> sub_svc = module->services.at(cfg.api);

    // see if our resource is reconfigurable. if it is, reconfigure
    std::shared_ptr<ResourceBase> res = sub_svc->resource(cfg.resource_name().name);
    if (res == nullptr) {
        return grpc::Status(
            grpc::UNKNOWN,
            "unable to reconfigure resource " + cfg.resource_name().name + " as it doesn't exist.");
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

    std::shared_ptr<ModelRegistration> reg = Registry::lookup_resource(cfg.name);
    if (reg != nullptr) {
        std::shared_ptr<ResourceBase> res = reg->construct_resource(deps, cfg);
        sub_svc->replace_one(cfg.resource_name(), res);
    }

    return grpc::Status();
};

::grpc::Status ModuleService_::RemoveResource(
    ::grpc::ServerContext* context,
    const ::viam::module::v1::RemoveResourceRequest* request,
    ::viam::module::v1::RemoveResourceResponse* response) {
    std::shared_ptr<Module> m = this->module;
    auto name = Name::from_string(request->name());
    const Subtype* subtype = name.to_subtype();
    if (m->services.find(*subtype) == m->services.end()) {
        return grpc::Status(grpc::UNKNOWN, "no grpc service for " + subtype->to_string());
    }
    std::shared_ptr<SubtypeService> svc = m->services.at(*name.to_subtype());
    std::shared_ptr<ResourceBase> res = svc->resource(name.name);
    if (res == nullptr) {
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
    module->lock.lock();
    viam::module::v1::HandlerMap hm = this->module->handles.to_proto();
    *response->mutable_handlermap() = hm;
    parent_addr = request->parent_address();
    module->lock.unlock();
    response->set_ready(module->ready);
    return grpc::Status();
};

ModuleService_::ModuleService_(std::string addr) {
    module = std::make_shared<Module>(addr);
}

void ModuleService_::start() {
    module->lock.lock();
    mode_t old_mask = umask(0077);
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    listen(sockfd, 10);
    umask(old_mask);

    // TODO(RSDK-1742) see if we can/want to do this in an init instead
    Server::register_service(this);
    std::string address = "unix://" + module->addr;
    Server::add_listening_port(address);

    module->lock.unlock();
    module->set_ready();
}

ModuleService_::~ModuleService_() {
    this->close();
}

void ModuleService_::close() {
    // CR erodkin: this actually doesn't shutdown gracefully at all! fix that
    BOOST_LOG_TRIVIAL(info) << "Shutting down gracefully.";

    if (parent != nullptr) {
        try {
            parent->close();
        } catch (std::exception& exc) {
            BOOST_LOG_TRIVIAL(error) << exc.what();
        }
    }
    Server::shutdown();
}

void ModuleService_::add_api_from_registry(Subtype api) {
    if (module->services.find(api) != module->services.end()) {
        return;
    }
    module->lock.lock();
    std::shared_ptr<SubtypeService> new_svc = std::make_shared<SubtypeService>();

    std::shared_ptr<ResourceSubtype> rs = Registry::lookup_subtype(api);
    std::shared_ptr<ResourceServerBase> server = rs->create_resource_server(new_svc);
    server->register_server();
    module->services.emplace(api, new_svc);
    module->servers.push_back(server);
    module->lock.unlock();
}

void ModuleService_::add_model_from_registry(Subtype api, Model model) {
    if (module->services.find(api) == module->services.end()) {
        add_api_from_registry(api);
    }

    std::shared_ptr<ResourceSubtype> creator = Registry::lookup_subtype(api);
    std::string name;
    const google::protobuf::ServiceDescriptor* sd;
    if (creator != nullptr && creator->service_descriptor != nullptr) {
        name = creator->service_descriptor->full_name();
        sd = creator->service_descriptor;
    }
    RPCSubtype rpc_subtype(api, name, *sd);
    module->handles.add_model(model, rpc_subtype);
};
