#pragma once

#include <module/v1/module.grpc.pb.h>

#include <config/resource.hpp>
#include <module/handler_map.hpp>
// #include <robot/service.hpp>
#include <components/service_base.hpp>
#include <subtype/subtype.hpp>

class Module {
   public:
    // std::shared_ptr<RobotService_>* parent;
    std::mutex lock;
    std::string name;
    std::string exe;
    std::string addr;
    bool ready;
    HandlerMap handles;
    std::shared_ptr<grpc::Channel> channel;
    std::unordered_map<Subtype, SubtypeService> services;
    void dial();
    ResourceBase get_parent_resource(Name name);
    void set_ready();
    Module();
    // Module(std::shared_ptr<RobotService_>* parent);
};

class ModuleService_ : public ComponentServiceBase,
                       public viam::module::v1::ModuleService::Service {
   public:
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
};

