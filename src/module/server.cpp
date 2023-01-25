#include <app/v1/robot.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/support/status.h>
#include <module/v1/module.grpc.pb.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/file_status.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/log/trivial.hpp>
#include <boost/none.hpp>
#include <common/utils.hpp>
#include <components/component_base.hpp>
#include <components/service_base.hpp>
#include <config/resource.hpp>
#include <memory>
#include <registry/registry.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <robot/client.hpp>
#include <robot/service.hpp>
#include <string>
#include <subtype/subtype.hpp>

// CR erodkin: probably just use class everywhere? or be more precise with when
// we use struct
struct Resource : public ResourceBase {
    std::unordered_map<Name, ResourceBase> dependencies;
    Component cfg;

    Resource(std::unordered_map<Name, ResourceBase> dependencies, Component cfg) {
        dependencies = dependencies;
        cfg = cfg;
    }
};

class Module {
   public:
    std::shared_ptr<RobotService_>* parent;
    std::mutex lock;
    std::string name;
    std::string exe;
    std::string addr;
    // CR erodkin: I don't think we need parent_addr? parent doesn't need to
    // be dialed right?
    std::string parent_addr;
    std::unordered_map<RPCSubtype, std::vector<Model>> handles;
    // CR erodkin: do we need resources here? do we actually use it?
    std::unordered_map<std::string, Resource> resources;
    std::shared_ptr<Channel> channel;
    std::unordered_map<Subtype, SubtypeService> services;
    void dial();
    void connect_parent();
    ResourceBase get_parent_resource(Name name);
};

class ModuleServer : public ComponentServiceBase, public viam::module::v1::ModuleService::Service {
   public:
    std::unordered_map<Name, Module> resources;
    ::grpc::Status AddResource(::grpc::ServerContext* context,
                               const ::viam::module::v1::AddResourceRequest* request,
                               ::viam::module::v1::AddResourceResponse* response) override;

    ::grpc::Status ReconfigureResource(
        ::grpc::ServerContext* context,
        const ::viam::module::v1::ReconfigureResourceRequest* request,
        ::viam::module::v1::ReconfigureResourceResponse* response) override;

    ::grpc::Status RemoveResource(::grpc::ServerContext* context,
                                  const ::viam::module::v1::RemoveResourceRequest* request,
                                  ::viam::module::v1::RemoveResourceResponse* response) override;

    ::grpc::Status Ready(::grpc::ServerContext* context,
                         const ::viam::module::v1::ReadyRequest* request,
                         ::viam::module::v1::ReadyResponse* response) override;

   private:
    boost::optional<Module> get_module(Component cfg);
};

boost::optional<Module> ModuleServer::get_module(Component cfg) {
    // CR erodkin: this is so ugly and nested! break it up. see
    // modmanager/manager.go get_module
    for (auto& ms : resources) {
        Module& module = ms.second;
        for (auto& api : module.handles) {
            const Subtype cfg_subtype = cfg.resource_name();

            if (api.first.subtype == cfg_subtype) {
                for (auto model : module.handles.at(api.first)) {
                    if (cfg.model == model) {
                        return module;
                    }
                }
            }
        }
    }
    return boost::none;
};

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

// CR erodkin: Flesh this out. define get_parent_resource, and probably
// change it to return a ResourceBase rather than a ComponentBase. Actually, we
// should also just make sure we need connect_parent at all. since this is
// functioning as a service, don't we just need to have a parent, and have a
// connection from our module server to the service?
void Module::connect_parent() {
    this->lock.lock();
    if (this->parent == nullptr) {
        boost::filesystem::file_status fs = boost::filesystem::status(this->parent_addr);
        fs.type();
    }
}

ResourceBase Module::get_parent_resource(Name name) {
    try {
        this->connect_parent();
    } catch (std::string err) {
        throw err;
    }

    return this->parent->get()->resource_by_name(name);
}

std::unordered_map<Name, ResourceBase> get_dependencies(
    google::protobuf::RepeatedPtrField<std::string> proto, Module* module) {
    std::unordered_map<Name, ResourceBase> deps;
    for (auto dep : proto) {
        Name name(dep);
        ResourceBase resource = module->get_parent_resource(name);
        deps.emplace(name, resource);
    }
    return deps;
}

::grpc::Status ModuleServer::AddResource(::grpc::ServerContext* context,
                                         const ::viam::module::v1::AddResourceRequest* request,
                                         ::viam::module::v1::AddResourceResponse* response) {
    viam::app::v1::ComponentConfig proto = request->config();
    Component cfg = Component::from_proto(proto);
    boost::optional<Module> module = get_module(cfg);
    if (module == boost::none) {
        std::string err_message = "no module registered to serve resource api " +
                                  cfg.resource_name().to_subtype().to_string() + " and model " +
                                  cfg.model.to_string();
        return grpc::Status(grpc::UNKNOWN, err_message);
    }

    module.get().lock.lock();

    std::unordered_map<Name, ResourceBase> deps =
        get_dependencies(request->dependencies(), module.get_ptr());

    // CR erodkin: unclear that we actually use module->resources ever.
    // confirm, remove if not
    Resource resource(deps, cfg);
    module->resources.emplace(cfg.name, resource);

    ResourceBase res;
    if (cfg.api.is_component_type()) {
        ComponentRegistration reg;
        try {
            reg = Registry::lookup_component(cfg.api, cfg.model);
            module.get().dial();
            res = reg.create_rpc_client(module->name, module->channel);
        } catch (std::string err) {
            BOOST_LOG_TRIVIAL(error) << err;
        }
    } else if (cfg.api.is_service_type()) {
        ServiceRegistration reg;
        try {
            reg = Registry::lookup_service(cfg.api, cfg.model);
            module.get().dial();
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
    // CR erodkin: in module/module.go:277-285, there's an adding of
    // components to the generic service in certain cases. see if we need to
    // do something similar.
    sub_svc.add(cfg.resource_name(), res);

    module.get().lock.unlock();
    return grpc::Status();
};

::grpc::Status ModuleServer::ReconfigureResource(
    ::grpc::ServerContext* context,
    const ::viam::module::v1::ReconfigureResourceRequest* request,
    ::viam::module::v1::ReconfigureResourceResponse* response) {
    viam::app::v1::ComponentConfig proto = request->config();
    Component cfg = Component::from_proto(proto);
    boost::optional<Module> module = get_module(cfg);

    if (module == boost::none) {
        throw "no module registered to serve resource api " +
            cfg.resource_name().to_subtype().to_string() + " and model " + cfg.model.to_string();
    }

    std::unordered_map<Name, ResourceBase> deps =
        get_dependencies(request->dependencies(), module.get_ptr());

    if (module->services.find(cfg.api) == module->services.end()) {
        throw "no rpc service for config: " + cfg.api.to_string();
    }
    SubtypeService& sub_svc = module->services.at(cfg.api);

    ResourceBase res = sub_svc.resource(cfg.resource_name().name);
};

::grpc::Status RemoveResource(::grpc::ServerContext* context,
                              const ::viam::module::v1::RemoveResourceRequest* request,
                              ::viam::module::v1::RemoveResourceResponse* response){};

::grpc::Status Ready(::grpc::ServerContext* context,
                     const ::viam::module::v1::ReadyRequest* request,
                     ::viam::module::v1::ReadyResponse* response){};
