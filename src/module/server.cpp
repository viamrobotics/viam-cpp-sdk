#include <app/v1/robot.pb.h>
#include <components/service_base.h>
#include <config/resource.h>
#include <grpcpp/grpcpp.h>
#include <module/v1/module.grpc.pb.h>
#include <resource/resource.h>

#include <string>

#include "robot/client.h"

struct Resource {
	std::vector<std::string> dependencies;
	Component cfg;

	Resource(std::vector<std::string> dependencies, Component cfg) {
		dependencies = dependencies;
		cfg = cfg;
	}
};

class Module {
	// CR erodkin: do we need parent?
	RobotService *parent;
	std::string name;
	std::string exe;
	std::string addr;
	std::unordered_map<RPCSubtype, std::vector<Model>> handles;
	std::unordered_map<std::string, Resource> resources;
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
};

// CR erodkin: you need to get a better high-level sense of what it is we're
// trying to do here. Ask around.
::grpc::Status ModuleServer::AddResource(
    ::grpc::ServerContext *context,
    const ::viam::module::v1::AddResourceRequest *request,
    ::viam::module::v1::AddResourceResponse *response) {
	std::vector<std::string> deps;
	// CR erodkin: should dependencies be a map? I think it is in
	// module/module.go
	for (auto dep : request->dependencies()) {
		deps.push_back(dep);
	}
	viam::app::v1::ComponentConfig proto = request->config();
	Component cfg = Component::from_proto(proto);
	Resource resource(deps, cfg);
	resources.emplace(cfg.name, resource);
	return ::grpc::Status();
};

::grpc::Status ReconfigureResource(
    ::grpc::ServerContext *context,
    const ::viam::module::v1::ReconfigureResourceRequest *request,
    ::viam::module::v1::ReconfigureResourceResponse *response){};

::grpc::Status RemoveResource(
    ::grpc::ServerContext *context,
    const ::viam::module::v1::RemoveResourceRequest *request,
    ::viam::module::v1::RemoveResourceResponse *response){};

::grpc::Status Ready(::grpc::ServerContext *context,
		     const ::viam::module::v1::ReadyRequest *request,
		     ::viam::module::v1::ReadyResponse *response){};
