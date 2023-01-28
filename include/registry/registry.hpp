#pragma once

#include <grpcpp/channel.h>
#include <robot/v1/robot.pb.h>

#include <components/service_base.hpp>
#include <resource/resource.hpp>
#include <string>

class ComponentRegistration {
   public:
    ComponentRegistration();
    ComponentType component_type;
    std::string name;
    std::function<ComponentBase(std::string, std::shared_ptr<grpc::Channel>)> create_rpc_client;
    viam::robot::v1::Status create_status(ComponentBase component);
};

class ServiceRegistration {
   public:
    ServiceRegistration();
    ServiceType service_type;
    std::string name;
    ResourceManager resource_manager;
    std::function<ServiceBase(std::string, std::shared_ptr<grpc::Channel>)> create_rpc_client;

    viam::robot::v1::Status create_status(ServiceBase service);
};

typedef std::unordered_map<Name, ResourceBase> Dependencies;

class Registry {
   public:
    /// Registers a component with the Registry
    /// Args:
    /// 	component (ComponentRegistration): object containing component
    /// 	registration data
    ///
    /// Raises:
    /// 	throws error if component already exists in the registry
    void register_component(ComponentRegistration component);
    static ServiceRegistration lookup_service(std::string name);
    static ServiceRegistration lookup_service(Subtype subtype, Model model);
    static ComponentRegistration lookup_component(std::string name);
    static ComponentRegistration lookup_component(Subtype subtype, Model model);
    static std::unordered_map<Subtype, ServiceRegistration> registered_services();

    static std::unordered_map<std::string, ComponentRegistration> registered_components();

   private:
    static std::unordered_map<Subtype, ComponentRegistration> components;
    static std::unordered_map<Subtype, ServiceRegistration> services;
};

