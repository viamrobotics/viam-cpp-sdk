#pragma once

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <grpcpp/channel.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/server.h>
#include <robot/v1/robot.pb.h>

#include <components/service_base.hpp>
#include <config/resource.hpp>
#include <resource/reconfigurable_resource.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <services/reconfigurable_service.hpp>
#include <string>
#include <subtype/subtype.hpp>

class ResourceSubtype {
   public:
    static std::shared_ptr<ResourceSubtype> new_from_descriptor(
        const google::protobuf::ServiceDescriptor* service_descriptor);
    std::function<ReconfigurableResource(ResourceBase, Name)> create_reconfigurable;
    std::function<ProtoType(ResourceBase)> create_status;
    const google::protobuf::ServiceDescriptor* service_descriptor;
    std::function<ResourceBase(std::string, std::shared_ptr<grpc::Channel>)> create_rpc_client;

    // TODO(RSDK-1742): would love for this constructor to be private but we get compiler complaints
    // if it is. See what we can do to fix that
    ResourceSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : service_descriptor(service_descriptor){};

   private:
};

typedef std::unordered_map<Name, std::shared_ptr<ResourceBase>> Dependencies;

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

