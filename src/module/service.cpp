#define BOOST_LOG_DYN_LINK 1
#include <app/v1/robot.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>
#include <module/v1/module.grpc.pb.h>
#include <module/v1/module.pb.h>

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

Module::Module(){};

Module::Module(std::shared_ptr<RobotService_>* parent) : parent(parent) {
    Subtype generic(RDK, COMPONENT, GENERIC);
    SubtypeService sub_svc;
    this->services.emplace(generic, sub_svc);
}

void Module::set_ready() {
    this->lock.lock();
    this->ready = true;
    this->lock.unlock();
}

void Module::dial() {
    if (this->channel != nullptr) {
        BOOST_LOG_TRIVIAL(info) << "attempted to dial with module " + this->name +
                                       " but it was already connected.";
        return;
    }

    std::string address("unix://");
    address += this->addr;
    this->channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
}

//// CR erodkin: fix
ResourceBase Module::get_parent_resource(Name name) {
    return this->parent->get()->resource_by_name(name);
}

std::unordered_map<Name, ResourceBase> get_dependencies(
    google::protobuf::RepeatedPtrField<std::string> proto, std::shared_ptr<Module> module) {
    Dependencies deps;
    for (auto& dep : proto) {
        Name name(dep);
        ResourceBase resource = module->get_parent_resource(name);
        deps.emplace(name, resource);
    }
    return deps;
}

::grpc::Status ModuleService_::AddResource(::grpc::ServerContext* context,
                                           const ::viam::module::v1::AddResourceRequest* request,
                                           ::viam::module::v1::AddResourceResponse* response) {
    viam::app::v1::ComponentConfig proto = request->config();
    Component cfg = Component::from_proto(proto);
    std::shared_ptr<Module> module = this->module;
    module->lock.lock();

    Dependencies deps = get_dependencies(request->dependencies(), module);

    ResourceBase res;
    if (cfg.api.is_component_type()) {
        ComponentRegistration reg;
        try {
            reg = Registry::lookup_component(cfg.api, cfg.model);
            module->dial();
            res = reg.create_rpc_client(module->name, module->channel);
        } catch (std::string err) {
            BOOST_LOG_TRIVIAL(error) << err;
        }
    } else if (cfg.api.is_service_type()) {
        ServiceRegistration reg;
        try {
            reg = Registry::lookup_service(cfg.api, cfg.model);
            module->dial();
            res = reg.create_rpc_client(module->name, module->channel);
        } catch (std::string err) {
            BOOST_LOG_TRIVIAL(error) << err;
        }
    } else {
        throw "unknown resource type " + cfg.api.resource_type;
    }

    if (module->services.find(cfg.api) == module->services.end()) {
        throw "module cannot service api " + cfg.api.to_string();
    }

    SubtypeService& sub_svc = module->services.at(cfg.api);
    if (cfg.api.is_component_type() && !(cfg.api == GENERIC_SUBTYPE)) {
        if (module->services.find(GENERIC_SUBTYPE) == module->services.end()) {
            throw "module cannot service the generic API";
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

    Dependencies deps = get_dependencies(request->dependencies(), module);

    if (module->services.find(cfg.api) == module->services.end()) {
        throw "no rpc service for config: " + cfg.api.to_string();
    }
    SubtypeService& sub_svc = module->services.at(cfg.api);

    // see if our resource is reconfigurable. if it is, reconfigure
    boost::optional<ResourceBase&> res_opt = sub_svc.resource(cfg.resource_name().name);
    ResourceBase res;
    try {
        res = res_opt.get();
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "unable to reconfigure resource " << cfg.resource_name().name
                                 << " as it doesn't exist.";
        return grpc::Status();
    }
    try {
        ReconfigurableComponent* rc = static_cast<ReconfigurableComponent*>(&res);
        rc->reconfigure(cfg, deps);
        return grpc::Status();
    } catch (std::exception& exc) {
    }

    try {
        ReconfigurableService* rs = static_cast<ReconfigurableService*>(&res);
        rs->reconfigure(::Service::from_component_config(cfg), deps);
        return grpc::Status();
    } catch (std::exception& exc) {
    }

    // if the type isn't reconfigurable by default, replace it
    try {
        res.stop();
    } catch (std::string err) {
        BOOST_LOG_TRIVIAL(error) << "unable to stop resource: " << err;
    }

    if (cfg.api.is_component_type()) {
        ComponentRegistration reg;
        try {
            reg = Registry::lookup_component(cfg.api, cfg.model);
            ComponentBase comp = reg.create_rpc_client(module->name, module->channel);
            if (!(cfg.api == GENERIC_SUBTYPE)) {
                if (module->services.find(GENERIC_SUBTYPE) == module->services.end()) {
                    throw "no generic service";
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
            reg = Registry::lookup_service(cfg.api, cfg.model);
            ServiceBase service = reg.create_rpc_client(module->name, module->channel);
            sub_svc.replace_one(cfg.resource_name(), service);
        } catch (std::string error) {
        }
    } else {
        throw "unknown resource type" + cfg.api.resource_type;
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
        throw "no grpc service for " + subtype->to_string();
    }
    SubtypeService& svc = m->services.at(*name.to_subtype());
    boost::optional<ResourceBase&> res_opt = svc.resource(name.name);
    ResourceBase res;
    try {
        res = res_opt.get();
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "unable to remove resource " << name.to_string()
                                 << " as it doesn't exist.";
        return grpc::Status();
    }

    try {
        res.stop();
    } catch (std::string err) {
        BOOST_LOG_TRIVIAL(error) << "unable to stop resource: " << err;
    }

    if (name.resource_type == COMPONENT && !(name.to_subtype() == &GENERIC_SUBTYPE)) {
        if (module->services.find(GENERIC_SUBTYPE) == module->services.end()) {
            throw "no generic service";
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
    response->set_ready(this->module->ready);
    viam::module::v1::HandlerMap hm = this->module->handles.to_proto();
    response->set_allocated_handlermap(&hm);
    return grpc::Status();
};
