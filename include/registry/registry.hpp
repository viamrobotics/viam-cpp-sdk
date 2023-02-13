#pragma once

#include <grpcpp/channel.h>
#include <grpcpp/server.h>
#include <robot/v1/robot.pb.h>

#include <components/service_base.hpp>
#include <config/resource.hpp>
#include <resource/reconfigurable_resource.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <services/reconfigurable_service.hpp>
#include <string>

#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"
#include "subtype/subtype.hpp"

class ResourceSubtype {
   public:
    ResourceSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : service_descriptor(service_descriptor){};
    ResourceSubtype(){};
    std::function<ReconfigurableResource(ResourceBase, Name)> create_reconfigurable;
    std::function<ProtoType(ResourceBase)> create_status;
    // CR erodkin: do we actually need this? confirm
    std::function<void(grpc::Server*, std::shared_ptr<SubtypeService>)>
        register_subtype_rpc_service;
    const google::protobuf::ServiceDescriptor* service_descriptor;
    // CR erodkin: this might not be necessary even if the rest is?
    std::shared_ptr<google::protobuf::Reflection> reflection_descriptor;
    std::function<ResourceBase(std::string, std::shared_ptr<grpc::Channel>)> create_rpc_client;
};

typedef std::unordered_map<Name, std::shared_ptr<ResourceBase>> Dependencies;

class ComponentRegistration {
   public:
    ComponentRegistration();
    ComponentRegistration(
        ComponentType ct,
        std::string name,
        std::function<std::unique_ptr<ComponentBase>(std::string, std::shared_ptr<grpc::Channel>)>
            create_rpc_client,
        // CR erodkin: I don't think we need create_component at all, create_rpc_client should be
        // sufficient
        std::function<std::unique_ptr<ComponentBase>(Dependencies, Component)> create_component)
        : component_type(std::move(ct)),
          name(std::move(name)),
          create_rpc_client(std::move(create_rpc_client)),
          create_component(std::move(create_component)){};
    ComponentType component_type;
    std::string name;
    std::function<std::shared_ptr<ComponentBase>(std::string, std::shared_ptr<grpc::Channel>)>
        create_rpc_client;
    std::function<std::unique_ptr<ComponentBase>(Dependencies, Component)> create_component;
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
    void register_component(std::shared_ptr<ComponentRegistration> component);
    static std::shared_ptr<ServiceRegistration> lookup_service(std::string name);
    static std::shared_ptr<ServiceRegistration> lookup_service(Subtype subtype, Model model);
    static std::shared_ptr<ComponentRegistration> lookup_component(std::string name);
    static std::shared_ptr<ComponentRegistration> lookup_component(Subtype subtype, Model model);
    static std::shared_ptr<ResourceSubtype> lookup_subtype(Subtype subtype);
    static std::unordered_map<Subtype, std::shared_ptr<ServiceRegistration>> registered_services();

    static std::unordered_map<std::string, std::shared_ptr<ComponentRegistration>>
    registered_components();

   private:
    static std::unordered_map<std::string, std::shared_ptr<ComponentRegistration>> components;
    static std::unordered_map<Subtype, std::shared_ptr<ResourceSubtype>> subtypes;
    static std::unordered_map<std::string, std::shared_ptr<ServiceRegistration>> services;
};

