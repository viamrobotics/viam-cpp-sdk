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
    Dependencies deps;
    for (auto& dep : proto) {
        Name name(dep);
        ResourceBase resource = get_parent_resource(name);
        deps.emplace(name, resource);
    }
    return deps;
}

ResourceBase ModuleService_::get_parent_resource(Name name) {
    std::cout << "CALLING get_parent_resource";
    if (parent == nullptr) {
        // CR erodkin: figure out what best option defaults are
        //*parent = RobotClient::at_address(parent_addr, {0, boost::none});
        *parent = RobotClient::at_address("unix://" + parent_addr, {0, boost::none});
    }

    // CR erodkin: we're forcing this to not be an option with the final `get` call. fix
    // return parent->get()->resource_by_name(name.to_proto()).get();
    //
    // fix this
    ResourceBase ret = parent->get()->resource_by_name(name.to_proto()).get();
    std::cout << " WE GOT A PARENT RESOURCE!! ";
    return ret;
}

::grpc::Status ModuleService_::AddResource(::grpc::ServerContext* context,
                                           const ::viam::module::v1::AddResourceRequest* request,
                                           ::viam::module::v1::AddResourceResponse* response) {
    viam::app::v1::ComponentConfig proto = request->config();
    Component cfg = Component::from_proto(proto);
    std::shared_ptr<Module> module = this->module;
    module->lock.lock();

    std::shared_ptr<ResourceBase> res;
    if (cfg.api.is_component_type()) {
        ComponentRegistration reg;
        try {
            reg = Registry::lookup_component(cfg.api, cfg.model).get();
            module->dial();
            res = reg.create_rpc_client(module->name, module->channel);
        } catch (std::string err) {
            BOOST_LOG_TRIVIAL(error) << err;
        }
    } else if (cfg.api.is_service_type()) {
        ServiceRegistration reg;
        try {
            reg = Registry::lookup_service(cfg.api, cfg.model).get();
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

    SubtypeService& sub_svc = module->services.at(cfg.api);
    if (cfg.api.is_component_type() && !(cfg.api == GENERIC_SUBTYPE)) {
        if (module->services.find(GENERIC_SUBTYPE) == module->services.end()) {
            return grpc::Status(grpc::UNKNOWN, "module cannot service the generic api");
        }
        SubtypeService& generic_service = module->services.at(GENERIC_SUBTYPE);
        generic_service.add(cfg.resource_name(), res);
    }
    sub_svc.add(cfg.resource_name(), res);

    module->lock.unlock();
    return grpc::Status();
};

::grpc::Status ModuleService_::ReconfigureResource(
    ::grpc::ServerContext* context,
    const ::viam::module::v1::ReconfigureResourceRequest* request,
    ::viam::module::v1::ReconfigureResourceResponse* response) {
    viam::app::v1::ComponentConfig proto = request->config();
    Component cfg = Component::from_proto(proto);
    std::shared_ptr<Module> module = this->module;

    Dependencies deps = get_dependencies(request->dependencies());

    if (module->services.find(cfg.api) == module->services.end()) {
        return grpc::Status(grpc::UNKNOWN, "no rpc servgice for config: " + cfg.api.to_string());
    }
    SubtypeService& sub_svc = module->services.at(cfg.api);

    // see if our resource is reconfigurable. if it is, reconfigure
    std::shared_ptr<ResourceBase> res = sub_svc.resource(cfg.resource_name().name);
    if (res == nullptr) {
        return grpc::Status(
            grpc::UNKNOWN,
            "unable to reconfigure resource " + cfg.resource_name().name + " as it doesn't exist.");
    }
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
            if (!(cfg.api == GENERIC_SUBTYPE)) {
                if (module->services.find(GENERIC_SUBTYPE) == module->services.end()) {
                    return grpc::Status(grpc::UNKNOWN, "no generic service");
                }
                SubtypeService& generic_service = module->services.at(GENERIC_SUBTYPE);
                generic_service.replace_one(cfg.resource_name(), res);
            }
            sub_svc.replace_one(cfg.resource_name(), comp);
        } catch (std::string error) {
        }
    } else if (cfg.api.is_service_type()) {
        ServiceRegistration reg;
        try {
            reg = Registry::lookup_service(cfg.api, cfg.model).get();
            std::shared_ptr<ServiceBase> service =
                reg.create_rpc_client(module->name, module->channel);
            sub_svc.replace_one(cfg.resource_name(), service);
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
    std::shared_ptr<Module> m = this->module;
    Name name(request->name());
    const Subtype* subtype = name.to_subtype();
    if (m->services.find(*subtype) == m->services.end()) {
        return grpc::Status(grpc::UNKNOWN, "no grpc service for " + subtype->to_string());
    }
    SubtypeService& svc = m->services.at(*name.to_subtype());
    std::shared_ptr<ResourceBase> res = svc.resource(name.name);
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

    if (name.resource_type == COMPONENT && !(name.to_subtype() == &GENERIC_SUBTYPE)) {
        if (module->services.find(GENERIC_SUBTYPE) == module->services.end()) {
            return grpc::Status(grpc::UNKNOWN, "no generic service");
        }
        SubtypeService& generic_service = module->services.at(GENERIC_SUBTYPE);
        generic_service.remove(name);
    }
    svc.remove(name);
    return grpc::Status();
};

::grpc::Status ModuleService_::Ready(::grpc::ServerContext* context,
                                     const ::viam::module::v1::ReadyRequest* request,
                                     ::viam::module::v1::ReadyResponse* response) {
    module->lock.lock();
    module->set_ready();
    std::cout << "check ready from c++!!\n";
    BOOST_LOG_TRIVIAL(error) << "checking ready!";
    if (this->module->ready) {
        BOOST_LOG_TRIVIAL(error) << "we are ready!";
    }
    response->set_ready(module->ready);
    viam::module::v1::HandlerMap hm = this->module->handles.to_proto();
    if (response->ready()) {
        std::cout << "WE ARE READY!!\n\n";
    }
    *response->mutable_handlermap() = hm;
    parent_addr = request->parent_address();
    BOOST_LOG_TRIVIAL(error) << "we are ready woo";
    module->lock.unlock();
    return grpc::Status();
};

ModuleService_::ModuleService_(std::shared_ptr<Module> module) : module(module) {}
ModuleService_::ModuleService_(std::string addr) {
    module = std::make_shared<Module>(addr);
}

void ModuleService_::start() {
    module->lock.lock();
    mode_t old_mask = umask(0077);
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    listen(sockfd, 3);
    //  CR erodkin: necessary?
    //  sockaddr addr;
    umask(old_mask);

    //// CR erodkin: fix
    std::string address = "unix://" + module->addr;
    // std::string address = module->addr;
    grpc::ServerBuilder builder;
    builder.RegisterService(this);
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    std::unique_ptr<grpc::reflection::ProtoServerReflectionPlugin> reflection =
        std::make_unique<grpc::reflection::ProtoServerReflectionPlugin>();
    // builder.InternalAddPluginFactory(reflection);
    //
    // grpc::reflection::ProtoServerReflectionPlugin refl;
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    reflection->UpdateServerBuilder(&builder);
    server = std::move(builder.BuildAndStart());
    module->set_ready();
    module->lock.unlock();
}

void ModuleService_::close() {
    module->lock.lock();
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
    module->lock.lock();
    if (module->services.find(api) != module->services.end()) {
        return;
    }
    SubtypeService new_svc;

    boost::optional<ResourceSubtype> rs = Registry::lookup_subtype(api);
    module->services.emplace(api, new_svc);
    if (rs != boost::none) {
        rs.get().register_subtype_rpc_service(server.get(), new_svc);
    }
}

void ModuleService_::add_model_from_registry(Subtype api, Model model) {
    std::cout << "ADDING MODEL FROM REGISTRY\n";
    if (module->services.find(api) == module->services.end()) {
        add_api_from_registry(api);
    }
    std::cout << "ADDING MODEL FROM REGISTRY2\n";

    bool generic_registered = (module->services.find(GENERIC_SUBTYPE) != module->services.end());

    if (api.is_component_type() && generic_registered && !(api == GENERIC_SUBTYPE)) {
        add_api_from_registry(GENERIC_SUBTYPE);
    }
    std::cout << "ADDING MODEL FROM REGISTRY3\n";

    boost::optional<ResourceSubtype> creator = Registry::lookup_subtype(api);
    std::cout << "ADDING MODEL FROM REGISTRY4\n";
    std::string name;
    google::protobuf::ServiceDescriptor* sd;
    if (creator != boost::none) {
        name = creator->service_descriptor->name();
        sd = creator->service_descriptor;
    }
    RPCSubtype rpc_subtype(api, name, *sd);
    std::vector<Model> models;
    // CR erodkin: this is kinda gross we should probably hide it inside the HandlerMap_ code
    if (module->handles.handles.find(rpc_subtype) != module->handles.handles.end()) {
        models = module->handles.handles.at(rpc_subtype);
    }
    std::cout << "ADDING MODEL FROM REGISTRY4\n";
    models.push_back(model);
    module->handles.handles.emplace(rpc_subtype, models);
    std::cout << "ADDING MODEL FROM REGISTRY5\n";
};

