#include "component/generic/v1/generic.grpc.pb.h"
#include "google/protobuf/descriptor.h"
#define BOOST_LOG_DYN_LINK 1
#include <app/v1/robot.pb.h>
#include <google/protobuf/reflection.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
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
#include <components/generic.hpp>
#include <components/reconfigurable_component.hpp>
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
#include <services/reconfigurable_service.hpp>
#include <string>
#include <subtype/subtype.hpp>

Dependencies ModuleService_::get_dependencies(
    google::protobuf::RepeatedPtrField<std::string> proto) {
    // CR erodkin: delete
    std::cout << "SOME CALL  " << std::endl;
    Dependencies deps;
    for (auto& dep : proto) {
        Name name(dep);
        ResourceBase resource = get_parent_resource(name);
        deps.emplace(name, resource);
    }
    return deps;
}

ResourceBase ModuleService_::get_parent_resource(Name name) {
    // CR erodkin: delete
    std::cout << "SOME CALL 2 " << std::endl;
    if (parent == nullptr) {
        // CR erodkin: figure out what best option defaults are
        //*parent = RobotClient::at_address(parent_addr, {0, boost::none});
        *parent = RobotClient::at_address("unix://" + parent_addr, {0, boost::none});
    }

    boost::optional<ResourceBase> resource = parent->get()->resource_by_name(name.to_proto());
    // CR erodkin: fix, make it just return an option? also simplify
    if (resource == boost::none) {
        std::cout << "Ugh it's a none" << std::endl;
        return ResourceBase();
    }
    std::cout << " WE GOT A PARENT RESOURCE!! ";
    return resource.get();
}

::grpc::Status ModuleService_::AddResource(::grpc::ServerContext* context,
                                           const ::viam::module::v1::AddResourceRequest* request,
                                           ::viam::module::v1::AddResourceResponse* response) {
    // CR erodkin: delete
    std::cout << "SOME CALL 3 " << std::endl;
    viam::app::v1::ComponentConfig proto = request->config();
    Component cfg = Component::from_proto(proto);
    std::shared_ptr<Module> module = this->module;
    module->lock.lock();

    std::shared_ptr<ResourceBase> res;
    // CR erodkin: get rid of these std::couts. also instead of a try, match on the none case
    if (cfg.api.is_component_type()) {
        ComponentRegistration reg;
        try {
            // CR erodkin: seems like this is sometimes being called before a component is
            // registered?
            reg = Registry::lookup_component(cfg.name).get();
            module->dial();
            res = reg.create_rpc_client(module->name, module->channel);
        } catch (std::string err) {
            BOOST_LOG_TRIVIAL(error) << err;
        }
    } else if (cfg.api.is_service_type()) {
        ServiceRegistration reg;
        // CR erodkin: instead of doing `get` calls here, we should just match on is_none
        try {
            reg = Registry::lookup_service(cfg.name).get();
            module->dial();
            res = reg.create_rpc_client(module->name, module->channel);
        } catch (std::string err) {
            BOOST_LOG_TRIVIAL(error) << err;
        }
    } else {
        return grpc::Status(grpc::UNKNOWN, "unknown resource type " + cfg.api.resource_type);
    }

    if (module->services.find(cfg.api) == module->services.end()) {
        return grpc::Status(grpc::UNKNOWN, "module cannot service api " + cfg.api.to_string());
    }

    std::shared_ptr<SubtypeService> sub_svc = module->services.at(cfg.api);
    if (cfg.api.is_component_type() && !(cfg.api == Generic::subtype())) {
        if (module->services.find(Generic::subtype()) == module->services.end()) {
            return grpc::Status(grpc::UNKNOWN, "module cannot service the generic api");
        }
        std::shared_ptr<SubtypeService> generic_service = module->services.at(Generic::subtype());
        generic_service->add(cfg.resource_name(), res);
    }
    std::cout << " LOOK HERE !7" << std::endl;
    sub_svc->add(cfg.resource_name(), res);
    std::cout << " LOOK HERE !8" << std::endl;

    module->lock.unlock();
    return grpc::Status();
};

::grpc::Status ModuleService_::ReconfigureResource(
    ::grpc::ServerContext* context,
    const ::viam::module::v1::ReconfigureResourceRequest* request,
    ::viam::module::v1::ReconfigureResourceResponse* response) {
    // CR erodkin: delete
    std::cout << "SOME CALL 4 " << std::endl;
    viam::app::v1::ComponentConfig proto = request->config();
    Component cfg = Component::from_proto(proto);
    std::shared_ptr<Module> module = this->module;

    Dependencies deps = get_dependencies(request->dependencies());

    if (module->services.find(cfg.api) == module->services.end()) {
        return grpc::Status(grpc::UNKNOWN, "no rpc servgice for config: " + cfg.api.to_string());
    }
    std::shared_ptr<SubtypeService> sub_svc = module->services.at(cfg.api);

    // see if our resource is reconfigurable. if it is, reconfigure
    std::shared_ptr<ResourceBase> res = sub_svc->resource(cfg.resource_name().name);
    if (res == nullptr) {
        return grpc::Status(
            grpc::UNKNOWN,
            "unable to reconfigure resource " + cfg.resource_name().name + " as it doesn't exist.");
    }
    // CR erodkin: get rid of me
    std::cout << "CALLING RECONFIGURE RESOURCE" << std::endl;
    try {
        ReconfigurableComponent* rc = static_cast<ReconfigurableComponent*>(res.get());
        rc->reconfigure(cfg, deps);
        return grpc::Status();
    } catch (std::exception& exc) {
    }

    try {
        // CR erodkin: deal with ambiguous cast here
        ServiceBase* sb = static_cast<ServiceBase*>(res.get());
        ReconfigurableService* rs = static_cast<ReconfigurableService*>(sb);
        rs->reconfigure(::Service::from_component_config(cfg), deps);
        return grpc::Status();
    } catch (std::exception& exc) {
    }

    // if the type isn't reconfigurable by default, replace it
    try {
        res->stop();
    } catch (std::string err) {
        BOOST_LOG_TRIVIAL(error) << "unable to stop resource: " << err;
    }

    if (cfg.api.is_component_type()) {
        ComponentRegistration reg;
        try {
            reg = Registry::lookup_component(cfg.api, cfg.model).get();
            std::shared_ptr<ComponentBase> comp =
                reg.create_rpc_client(module->name, module->channel);
            if (!(cfg.api == Generic::subtype())) {
                if (module->services.find(Generic::subtype()) == module->services.end()) {
                    return grpc::Status(grpc::UNKNOWN, "no generic service");
                }
                std::shared_ptr<SubtypeService> generic_service =
                    module->services.at(Generic::subtype());
                generic_service->replace_one(cfg.resource_name(), res);
            }
            sub_svc->replace_one(cfg.resource_name(), comp);
        } catch (std::string error) {
        }
    } else if (cfg.api.is_service_type()) {
        ServiceRegistration reg;
        try {
            reg = Registry::lookup_service(cfg.api, cfg.model).get();
            std::shared_ptr<ServiceBase> service =
                reg.create_rpc_client(module->name, module->channel);
            sub_svc->replace_one(cfg.resource_name(), service);
        } catch (std::string error) {
        }
    } else {
        return grpc::Status(grpc::UNKNOWN, "unknown resource type " + cfg.api.resource_type);
    }

    return grpc::Status();
};

::grpc::Status ModuleService_::RemoveResource(
    ::grpc::ServerContext* context,
    const ::viam::module::v1::RemoveResourceRequest* request,
    ::viam::module::v1::RemoveResourceResponse* response) {
    // CR erodkin: delete
    std::cout << "SOME CALL 5 " << std::endl;
    std::shared_ptr<Module> m = this->module;
    Name name(request->name());
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

    if (name.resource_type == COMPONENT && !(*name.to_subtype() == Generic::subtype())) {
        if (module->services.find(Generic::subtype()) == module->services.end()) {
            return grpc::Status(grpc::UNKNOWN, "no generic service");
        }
        std::shared_ptr<SubtypeService> generic_service = module->services.at(Generic::subtype());
        generic_service->remove(name);
    }
    svc->remove(name);
    return grpc::Status();
};

::grpc::Status ModuleService_::Ready(::grpc::ServerContext* context,
                                     const ::viam::module::v1::ReadyRequest* request,
                                     ::viam::module::v1::ReadyResponse* response) {
    // CR erodkin: delete
    std::cout << "SOME CALL 6 " << std::endl;
    module->lock.lock();
    // CR erodkin: delete me
    if (!module->ready) {
        std::cout << "WE ARE NOT READY BOO" << std::endl;
    } else {
        std::cout << "WE ARE READY WOO" << std::endl;
    }
    response->set_ready(module->ready);
    viam::module::v1::HandlerMap hm = this->module->handles.to_proto();
    *response->mutable_handlermap() = hm;
    parent_addr = request->parent_address();
    module->lock.unlock();
    return grpc::Status();
};

ModuleService_::ModuleService_(std::shared_ptr<Module> module) : module(module) {}
ModuleService_::ModuleService_(std::string addr) {
    module = std::make_shared<Module>(addr);
}

void ModuleService_::start() {
    // CR erodkin: lock happening here might be part of a problem?
    // CR erodkin: delete
    std::cout << "SOME CALL 7 " << std::endl;
    module->lock.lock();
    mode_t old_mask = umask(0777);
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    listen(sockfd, 10);
    // CR erodkin: necessary?
    sockaddr addr;
    umask(old_mask);

    std::string address = "unix://" + module->addr;
    // CR erodkin: delete me
    std::cout << "WHAT IS THE ADDRESS??" << address << std::endl;
    grpc::ServerBuilder builder;
    // CR erodkin: hard coding here no good! fix that up. In general we should have a better
    // location for this.
    viam::component::generic::v1::GenericService::Service gs;
    builder.RegisterService(&gs);
    builder.RegisterService(this);
    std::cout << "ADDING LISTENING PORT" << std::endl;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    std::unique_ptr<grpc::reflection::ProtoServerReflectionPlugin> reflection =
        std::make_unique<grpc::reflection::ProtoServerReflectionPlugin>();
    // builder.InternalAddPluginFactory(reflection);
    //
    // grpc::reflection::ProtoServerReflectionPlugin refl;
    // grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    reflection->UpdateServerBuilder(&builder);
    // CR erodkin: probably we don't need server here? instead we can just always call at startup
    // the server func
    server = std::move(builder.BuildAndStart());
    // CR erodkin: module has to be unlocked first because set_ready takes the lock. pretty gross!
    // fix it
    module->lock.unlock();
    module->set_ready();
}

ModuleService_::~ModuleService_() {
    this->close();
}

void ModuleService_::close() {
    // CR erodkin: delete
    std::cout << "SOME CALL 8 " << std::endl;
    module->lock.lock();
    // CR erodkin: delete me
    std::cout << "CALLING CLOSE" << std::endl;
    BOOST_LOG_TRIVIAL(info) << "Shutting down gracefully.";

    if (parent != nullptr) {
        try {
            parent->get()->close();
        } catch (std::exception& exc) {
            BOOST_LOG_TRIVIAL(error) << exc.what();
        }
    }
    server->Shutdown();
}

void ModuleService_::add_api_from_registry(Subtype api) {
    // CR erodkin: delete
    std::cout << "SOME CALL 9 " << std::endl;
    module->lock.lock();
    if (module->services.find(api) != module->services.end()) {
        return;
    }
    std::shared_ptr<SubtypeService> new_svc = std::make_shared<SubtypeService>();

    boost::optional<ResourceSubtype> rs = Registry::lookup_subtype(api);
    module->services.emplace(api, new_svc);
    if (rs != boost::none) {
        rs.get().register_subtype_rpc_service(server.get(), new_svc);
    }
}

void ModuleService_::add_model_from_registry(Subtype api, Model model) {
    // CR erodkin: delete
    std::cout << "SOME CALL 10 " << std::endl;
    if (module->services.find(api) == module->services.end()) {
        add_api_from_registry(api);
    }

    bool generic_registered = (module->services.find(Generic::subtype()) != module->services.end());

    if (api.is_component_type() && generic_registered && !(api == Generic::subtype())) {
        add_api_from_registry(Generic::subtype());
    }

    boost::optional<ResourceSubtype> creator = Registry::lookup_subtype(api);
    std::string name;
    const google::protobuf::ServiceDescriptor* sd;
    if (creator != boost::none && creator->service_descriptor != nullptr) {
        name = creator->service_descriptor->full_name();
        sd = creator->service_descriptor;
    }
    RPCSubtype rpc_subtype(api, name, *sd);
    std::vector<Model> models;
    // CR erodkin: this is kinda gross we should probably hide it inside the HandlerMap_ code
    if (module->handles.handles.find(rpc_subtype) != module->handles.handles.end()) {
        models = module->handles.handles.at(rpc_subtype);
    }
    models.push_back(model);
    module->handles.handles.emplace(rpc_subtype, models);
};

