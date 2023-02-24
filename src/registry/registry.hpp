#pragma once

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <grpcpp/channel.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/server.h>
#include <robot/v1/robot.pb.h>

#include <components/component_base.hpp>
#include <components/component_type.hpp>
#include <config/resource.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <resource/resource_server_base.hpp>
#include <services/service_base.hpp>
#include <services/service_type.hpp>
#include <string>
#include <subtype/subtype.hpp>

// TODO(RSDK-1742): instead of std::functions, consider making these functions virtual
// TODO(RSDK-1742): one class per header
class ResourceSubtype {
   public:
    std::function<ResourceBase(ResourceBase, Name)> create_reconfigurable;
    std::function<ProtoType(ResourceBase)> create_status;
    const google::protobuf::ServiceDescriptor* service_descriptor;
    std::function<ResourceBase(std::string, std::shared_ptr<grpc::Channel>)> create_rpc_client;
    virtual std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<SubtypeService> svc);

    ResourceSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : service_descriptor(service_descriptor){};

   private:
};

class ComponentRegistration {
   public:
    ComponentRegistration();
    ComponentRegistration(
        ComponentType ct,
        Subtype subtype,
        Model model,
        std::function<std::unique_ptr<ComponentBase>(std::string, std::shared_ptr<grpc::Channel>)>
            create_rpc_client)
        : subtype(std::move(subtype)),
          component_type(std::move(ct)),
          model(std::move(model)),
          create_rpc_client(std::move(create_rpc_client)){};
    Subtype subtype;
    Model model;
    ComponentType component_type;
    std::function<std::shared_ptr<ComponentBase>(std::string, std::shared_ptr<grpc::Channel>)>
        create_rpc_client;
    viam::robot::v1::Status create_status(std::shared_ptr<ComponentBase> component);
};

class ServiceRegistration {
   public:
    ServiceRegistration();
    ServiceType service_type;
    std::string name;
    std::function<std::shared_ptr<ServiceBase>(std::string, std::shared_ptr<grpc::Channel>)>
        create_rpc_client;

    viam::robot::v1::Status create_status(std::shared_ptr<ServiceBase> service);
};

class Registry {
   public:
    /// Registers a component with the Registry
    /// Args:
    /// 	component (ComponentRegistration): object containing component
    /// 	registration data
    ///
    /// Raises:
    /// 	throws error if component already exists in the registry
    static void register_component(std::shared_ptr<ComponentRegistration> component);
    static void register_subtype(Subtype subtype,
                                 std::shared_ptr<ResourceSubtype> resource_subtype);
    static std::shared_ptr<ServiceRegistration> lookup_service(std::string name);
    static std::shared_ptr<ServiceRegistration> lookup_service(Subtype subtype, Model model);
    static std::shared_ptr<ComponentRegistration> lookup_component(std::string name);
    static std::shared_ptr<ComponentRegistration> lookup_component(Subtype subtype, Model model);
    static std::shared_ptr<ResourceSubtype> lookup_subtype(Subtype subtype);
    static std::unordered_map<std::string, std::shared_ptr<ServiceRegistration>>
    registered_services();

    static std::unordered_map<std::string, std::shared_ptr<ComponentRegistration>>
    registered_components();

   private:
    static std::unordered_map<std::string, std::shared_ptr<ComponentRegistration>> components;
    static std::unordered_map<Subtype, std::shared_ptr<ResourceSubtype>> subtypes;
    static std::unordered_map<std::string, std::shared_ptr<ServiceRegistration>> services;
};

