#include <google/protobuf/struct.pb.h>
#include <grpcpp/channel.h>
#include <robot/v1/robot.pb.h>

#include <components/component_base.hpp>
#include <components/service_base.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <services/service_base.hpp>
#include <string>
#include <unordered_map>

using viam::robot::v1::Status;

class ComponentRegistration {
   public:
    ComponentRegistration();
    ComponentType component_type;
    std::string name;
    ResourceManager resource_manager;
    std::function<ComponentBase(std::string, std::shared_ptr<grpc::Channel>)> create_rpc_client;
    Status create_status(ComponentBase component);
};

class ServiceRegistration {
   public:
    ServiceRegistration();
    std::string resource_type;
    std::string name;
    ResourceManager resource_manager;
    std::function<ServiceBase(std::string, std::shared_ptr<grpc::Channel>)> create_rpc_client;

    Status create_status(ServiceBase service);
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
    static std::unordered_map<Subtype, ComponentRegistration> registered_components();

   private:
    static std::unordered_map<Subtype, ComponentRegistration> components;
    static std::unordered_map<Subtype, ServiceRegistration> services;
};

std::unordered_map<Subtype, ServiceRegistration> services;
std::unordered_map<Subtype, ComponentRegistration> Registry::components;

void Registry::register_component(ComponentRegistration component) {
    if (components.find(component.name) != components.end()) {
        std::string err =
            "Cannot add component with name " + component.name + "as it already exists";
        throw std::runtime_error(err);
    }

    components.emplace(component.name, component);
}

ServiceRegistration Registry::lookup_service(std::string name) {
    if (services.find(name) == services.end()) {
        std::string err = "Service " + name + "not found.";
        err += name;
        throw std::runtime_error(err);
    }

    return services.at(name);
}

ServiceRegistration Registry::lookup_service(Subtype subtype, Model model) {
    std::string name = subtype.to_string() + model.to_string();
    return lookup_service(name);
}

ComponentRegistration Registry::lookup_component(std::string name) {
    if (components.find(name) == components.end()) {
        std::string err = "Component " + name + "not found.";
        err += name;
        throw std::runtime_error(err);
    }

    return components.at(name);
}

ComponentRegistration Registry::lookup_component(Subtype subtype, Model model) {
    std::string name = subtype.to_string() + model.to_string();
    return lookup_component(name);
}

std::unordered_map<Subtype, ServiceRegistration> Registry::registered_services() {
    std::unordered_map<Subtype, ServiceRegistration> registry;
    for (auto& service : services) {
        registry.insert(service);
    }
    return registry;
}

std::unordered_map<Subtype, ComponentRegistration> Registry::registered_components() {
    std::unordered_map<Subtype, ComponentRegistration> registry;
    for (auto& component : components) {
        registry.insert(component);
    }
    return registry;
}

Status ComponentRegistration::create_status(ComponentBase component) {
    Status status;
    google::protobuf::Struct struct_;
    *status.mutable_name() = component.get_resource_name(component.name);
    *status.mutable_status() = struct_;
    return status;
}

Status ServiceRegistration::create_status(ServiceBase service) {
    Status status;
    google::protobuf::Struct struct_;
    *status.mutable_name() = service.get_resource_name(service.name);
    *status.mutable_status() = struct_;
    return status;
}
