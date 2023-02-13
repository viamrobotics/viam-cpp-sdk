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
    components.emplace(component->name, component);
}

std::shared_ptr<ServiceRegistration> Registry::lookup_service(std::string name) {
    if (services.find(name) == services.end()) {
        // CR erodkin: do we need to do this if check at all? can we just return services.at?
        return nullptr;
    }

    return services.at(name);
}

std::shared_ptr<ServiceRegistration> Registry::lookup_service(Subtype subtype, Model model) {
    const std::string name = subtype.to_string() + model.to_string();
    return lookup_service(name);
}

std::shared_ptr<ComponentRegistration> Registry::lookup_component(std::string name) {
    if (components.find(name) == components.end()) {
        return nullptr;
    }

    return components.at(name);
}

std::shared_ptr<ComponentRegistration> Registry::lookup_component(Subtype subtype, Model model) {
    const std::string name = subtype.to_string() + model.to_string();
    return lookup_component(name);
}

std::shared_ptr<ResourceSubtype> Registry::lookup_subtype(Subtype subtype) {
    if (subtypes.find(subtype) == subtypes.end()) {
        return nullptr;
    }

    return subtypes.at(subtype);
}

std::unordered_map<Subtype, std::shared_ptr<ServiceRegistration>> Registry::registered_services() {
    std::unordered_map<Subtype, std::shared_ptr<ServiceRegistration>> registry;
    for (auto& service : services) {
        registry.insert(service);
    }
    return registry;
}

std::unordered_map<std::string, std::shared_ptr<ComponentRegistration>>
Registry::registered_components() {
    std::unordered_map<std::string, std::shared_ptr<ComponentRegistration>> registry;
    for (auto& component : components) {
        registry.insert(component);
    }
    return registry;
}

Status ComponentRegistration::create_status(std::shared_ptr<ComponentBase> component) {
    Status status;
    *status.mutable_name() = component->get_resource_name(component->name);
    *status.mutable_status() = google::protobuf::Struct();
    return status;
}

Status ServiceRegistration::create_status(std::shared_ptr<ServiceBase> service) {
    Status status;
    google::protobuf::Struct struct_;
    *status.mutable_name() = service->get_resource_name(service->name);
    *status.mutable_status() = struct_;
    return status;
}

std::unordered_map<std::string, std::shared_ptr<ComponentRegistration>> Registry::components;
std::unordered_map<std::string, std::shared_ptr<ServiceRegistration>> Registry::services;
// // CR erodkin: do we want generic to be automatically built in like this?
// We want it to be automatic but not-like-this.gif. Instead, have it done through an init.
std::unordered_map<Subtype, std::shared_ptr<ResourceSubtype>> Registry::subtypes = {
    {Generic::subtype(), std::make_shared<ResourceSubtype>(Generic::resource_subtype())}};

ServiceRegistration::ServiceRegistration(){};
ComponentRegistration::ComponentRegistration(){};
