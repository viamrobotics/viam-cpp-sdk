#pragma once

#include <component/generic/v1/generic.grpc.pb.h>
#include <module/v1/module.grpc.pb.h>

#include <components/service_base.hpp>
#include <module/module.hpp>
#include <resource/resource_base.hpp>

class ModuleService_ : public ComponentServiceBase,
                       public viam::module::v1::ModuleService::Service {
   public:
    void start();
    void close();
    std::shared_ptr<ResourceBase> get_parent_resource(Name name);
    void add_api_from_registry(Subtype api);
    void add_model_from_registry(Subtype api, Model model);
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

    std::shared_ptr<Module> module;

    ModuleService_(std::string addr);
    ~ModuleService_();

   private:
    std::shared_ptr<RobotClient> parent;
    std::string parent_addr;
};

