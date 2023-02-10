#pragma once

#include <component/generic/v1/generic.grpc.pb.h>
#include <module/v1/module.grpc.pb.h>

#include <components/service_base.hpp>
// #include <generic/generic.hpp>
#include <module/module.hpp>

class ModuleService_ : public ComponentServiceBase,
                       public viam::module::v1::ModuleService::Service {
   public:
    void start();
    void close();
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
    // CR erodkin: this shouldn't be living here, it's going to be used by lots of services not just
    // module
    std::unique_ptr<grpc::Server> server;
    // CR erodkin: see if this works! think we don't need this probably actually
    // std::unique_ptr<grpc::ServerBuilder> server_builder;

    std::unique_ptr<viam::component::generic::v1::GenericService::Service> gs;
    ModuleService_(std::shared_ptr<Module> module);
    ModuleService_(std::string addr);
    ~ModuleService_();
    Dependencies get_dependencies(google::protobuf::RepeatedPtrField<std::string> proto);

   private:
    ResourceBase get_parent_resource(Name name);
    std::shared_ptr<RobotClient>* parent;
    // CR erodkin: do we need parent_addr at all?
    std::string parent_addr;
};

