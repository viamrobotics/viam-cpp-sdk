#pragma once

#include <viam/api/component/generic/v1/generic.grpc.pb.h>
#include <viam/api/module/v1/module.grpc.pb.h>

#include <viam/sdk/components/service_base.hpp>
#include <viam/sdk/module/module.hpp>
#include <viam/sdk/resource/resource_base.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

class ModuleService_ : public ComponentServiceBase,
                       public viam::module::v1::ModuleService::Service {
   public:
    void start(std::shared_ptr<Server> server);
    void close();
    std::shared_ptr<ResourceBase> get_parent_resource(Name name);

    void add_api_from_registry(std::shared_ptr<Server> server, Subtype api);
    void add_model_from_registry(std::shared_ptr<Server> server, Subtype api, Model model);
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

    ::grpc::Status ValidateConfig(::grpc::ServerContext* context,
                                  const ::viam::module::v1::ValidateConfigRequest* request,
                                  ::viam::module::v1::ValidateConfigResponse* response) override;

    std::shared_ptr<Module> module;

    ModuleService_(std::string addr);
    ~ModuleService_();

   private:
    std::shared_ptr<RobotClient> parent;
    std::string parent_addr;
};

}  // namespace sdk
}  // namespace viam
