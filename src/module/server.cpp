#include <app/v1/robot.pb.h>
#include <common/utils.h>
#include <components/component_base.h>
#include <components/service_base.h>
#include <config/resource.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <module/v1/module.grpc.pb.h>
#include <resource/resource.h>
#include <robot/client.h>
#include <robot/service.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/file_status.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/none.hpp>
#include <memory>
#include <resource/resource_base.hpp>
#include <string>
#include <subtype/subtype.hpp>

// CR erodkin: probably just use class everywhere? or be more precise with when
// we use struct
struct Resource : public ResourceBase {
	std::unordered_map<Name, ComponentBase> dependencies;
	Component cfg;

	Resource(std::unordered_map<Name, ComponentBase> dependencies,
		 Component cfg) {
		dependencies = dependencies;
		cfg = cfg;
	}
};

class Module {
       public:
	std::shared_ptr<RobotService_> *parent;
	std::mutex lock;
	std::string name;
	std::string exe;
	std::string addr;
	std::string parent_addr;
	std::unordered_map<RPCSubtype, std::vector<Model>> handles;
	std::unordered_map<std::string, Resource> resources;
	// CR erodkin: here!
	std::unordered_map<Subtype, SubtypeService> services;
	void connect_parent();
	ComponentBase get_parent_resource(Name name);
};

class ModuleServer : public ComponentServiceBase,
		     public viam::module::v1::ModuleService::Service {
       public:
	std::unordered_map<Name, Module> resources;
	::grpc::Status AddResource(
	    ::grpc::ServerContext *context,
	    const ::viam::module::v1::AddResourceRequest *request,
	    ::viam::module::v1::AddResourceResponse *response) override;

	::grpc::Status ReconfigureResource(
	    ::grpc::ServerContext *context,
	    const ::viam::module::v1::ReconfigureResourceRequest *request,
	    ::viam::module::v1::ReconfigureResourceResponse *response) override;

	::grpc::Status RemoveResource(
	    ::grpc::ServerContext *context,
	    const ::viam::module::v1::RemoveResourceRequest *request,
	    ::viam::module::v1::RemoveResourceResponse *response) override;

	::grpc::Status Ready(
	    ::grpc::ServerContext *context,
	    const ::viam::module::v1::ReadyRequest *request,
	    ::viam::module::v1::ReadyResponse *response) override;

       private:
	boost::optional<Module> get_module(Component cfg);
};

boost::optional<Module> ModuleServer::get_module(Component cfg) {
	// CR erodkin: this is so ugly and nested! break it up. see
	// modmanager/manager.go get_module
	for (auto &ms : resources) {
		Module &module = ms.second;
		for (auto &api : module.handles) {
			const Subtype cfg_subtype = cfg.resource_name();

			if (api.first.subtype == cfg_subtype) {
				for (auto model :
				     module.handles.at(api.first)) {
					if (cfg.model == model) {
						return module;
					}
				}
			}
		}
	}
	return boost::none;
};

// CR erodkin: START FROM HERE. define get_parent_resource, and probably
// change it to return a ResourceBase rather than a ComponentBase
void Module::connect_parent() {
	this->lock.lock();
	if (this->parent == nullptr) {
		boost::filesystem::file_status fs =
		    boost::filesystem::status(this->parent_addr);
		fs.type();
	}
}

ComponentBase Module::get_parent_resource(Name name) {
	try {
		this->connect_parent();
	} catch (std::string err) {
		throw err;
	}

	return this->parent->get()->resource_by_name(name);
}

// CR erodkin: you need to get a better high-level sense of what it is we're
// trying to do here. Ask around.
//
// update on Monday: I think what we want to do here is store the non-proto
// resource types that we created for client activity. when we call AddResource,
// we convert proto to the generic type and store it in a registry,
// RemoveResource removes it, etc. Make sure we know what ReconfigureResource
// actually does?
//
std::unordered_map<Name, ComponentBase> get_dependencies(
    google::protobuf::RepeatedPtrField<std::string> proto, Module *module) {
	std::unordered_map<Name, ComponentBase> deps;
	for (auto dep : proto) {
		Name name(dep);
		ComponentBase resource = module->get_parent_resource(name);
		deps.emplace(name, resource);
	}
	return deps;
}

::grpc::Status ModuleServer::AddResource(
    ::grpc::ServerContext *context,
    const ::viam::module::v1::AddResourceRequest *request,
    ::viam::module::v1::AddResourceResponse *response) {
	viam::app::v1::ComponentConfig proto = request->config();
	Component cfg = Component::from_proto(proto);
	boost::optional<Module> module = get_module(cfg);
	if (module == boost::none) {
		std::string err_message =
		    "no module registered to serve resource api " +
		    cfg.resource_name().to_subtype().to_string() +
		    " and model " + cfg.model.to_string();
		return grpc::Status(grpc::UNKNOWN, err_message);
	}

	module.get().lock.lock();

	std::unordered_map<Name, ComponentBase> deps =
	    get_dependencies(request->dependencies(), module.get_ptr());

	Resource resource(deps, cfg);
	module->resources.emplace(cfg.name, resource);

	module.get().lock.unlock();
	return grpc::Status();
};

::grpc::Status ModuleServer::ReconfigureResource(
    ::grpc::ServerContext *context,
    const ::viam::module::v1::ReconfigureResourceRequest *request,
    ::viam::module::v1::ReconfigureResourceResponse *response) {
	viam::app::v1::ComponentConfig proto = request->config();
	Component cfg = Component::from_proto(proto);
	boost::optional<Module> module = get_module(cfg);
	if (module == boost::none) {
		throw "no module registered to serve resource api " +
		    cfg.resource_name().to_subtype().to_string() +
		    " and model " + cfg.model.to_string();
	}

	std::unordered_map<Name, ComponentBase> deps =
	    get_dependencies(request->dependencies(), module.get_ptr());
};

::grpc::Status RemoveResource(
    ::grpc::ServerContext *context,
    const ::viam::module::v1::RemoveResourceRequest *request,
    ::viam::module::v1::RemoveResourceResponse *response){};

::grpc::Status Ready(::grpc::ServerContext *context,
		     const ::viam::module::v1::ReadyRequest *request,
		     ::viam::module::v1::ReadyResponse *response){};
