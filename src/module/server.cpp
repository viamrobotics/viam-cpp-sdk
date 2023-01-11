#include <app/v1/robot.pb.h>
#include <components/service_base.h>
#include <grpcpp/grpcpp.h>
#include <module/v1/module.grpc.pb.h>
#include <resource/resource.h>

struct Resource {
	std::vector<std::string> dependencies;
	viam::app::v1::ComponentConfig cfg;

	Resource(std::vector<std::string> dependencies,
		 viam::app::v1::ComponentConfig cfg) {
		dependencies = dependencies;
		cfg = cfg;
	}
};

class Module {
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

::grpc::Status ModuleServer::AddResource(
    ::grpc::ServerContext *context,
    const ::viam::module::v1::AddResourceRequest *request,
    ::viam::module::v1::AddResourceResponse *response) {
	viam::app::v1::ComponentConfig cfg = request->config();
	google::protobuf::RepeatedPtrField<std::string> dependencies =
	    request->dependencies();
	std::vector<std::string> deps;
	for (auto dep : dependencies) {
		deps.push_back(dep);
	}
	Resource resource(deps, cfg);
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
