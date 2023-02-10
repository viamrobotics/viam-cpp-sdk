#include <google/protobuf/struct.pb.h>
#include <grpcpp/channel.h>
#include <robot/v1/robot.pb.h>

#include <components/component_base.hpp>
#include <components/service_base.hpp>
#include <exception>
#include <registry/registry.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <services/service_base.hpp>
#include <string>
#include <unordered_map>

#include "components/generic.hpp"

using viam::robot::v1::Status;

void Registry::register_component(std::shared_ptr<ComponentRegistration> component) {
    // CR erodkin: delete
    if (components.find(component->name) != components.end()) {
        std::string err =
            "Cannot add component with name " + component->name + "as it already exists";
        throw std::runtime_error(err);
    }

    // CR erodkin: registered components should just be a bunch of pointers
    components.emplace(component->name, *component);
}

boost::optional<ServiceRegistration> Registry::lookup_service(std::string name) {
    if (services.find(name) == services.end()) {
        return boost::none;
    }

    return services.at(name);
}

boost::optional<ServiceRegistration> Registry::lookup_service(Subtype subtype, Model model) {
    const std::string name = subtype.to_string() + model.to_string();
    return lookup_service(name);
}

boost::optional<ComponentRegistration> Registry::lookup_component(std::string name) {
    if (components.find(name) == components.end()) {
        return boost::none;
    }

    return components.at(name);
}

boost::optional<ComponentRegistration> Registry::lookup_component(Subtype subtype, Model model) {
    const std::string name = subtype.to_string() + model.to_string();
    return lookup_component(name);
}

boost::optional<ResourceSubtype> Registry::lookup_subtype(Subtype subtype) {
    if (subtypes.find(subtype) == subtypes.end()) {
        return boost::none;
    }

    return subtypes.at(subtype);
}

std::unordered_map<Subtype, ServiceRegistration> Registry::registered_services() {
    std::unordered_map<Subtype, ServiceRegistration> registry;
    for (auto& service : services) {
        registry.insert(service);
    }
    return registry;
}

std::unordered_map<std::string, ComponentRegistration> Registry::registered_components() {
    std::unordered_map<std::string, ComponentRegistration> registry;
    for (auto& component : components) {
        registry.insert(component);
    }
    return registry;
}

Status ComponentRegistration::create_status(ComponentBase component) {
    Status status;
    *status.mutable_name() = component.get_resource_name(component.name);
    *status.mutable_status() = google::protobuf::Struct();
    return status;
}

Status ServiceRegistration::create_status(ServiceBase service) {
    Status status;
    google::protobuf::Struct struct_;
    *status.mutable_name() = service.get_resource_name(service.name);
    *status.mutable_status() = struct_;
    return status;
}

std::unordered_map<std::string, ComponentRegistration> Registry::components;
std::unordered_map<std::string, ServiceRegistration> Registry::services;
// // CR erodkin: fix
// std::unordered_map<Subtype, ResourceSubtype> Registry::subtypes;
std::unordered_map<Subtype, ResourceSubtype> Registry::subtypes = {
    {Generic::subtype(), Generic::resource_subtype()}};

ServiceRegistration::ServiceRegistration(){};
ComponentRegistration::ComponentRegistration(){};
