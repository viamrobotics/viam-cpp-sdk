#include <components/component_base.hpp>
#include <components/resource_manager.hpp>
#include <services/service_base.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

ResourceManager::ResourceManager() {}

/// Register a new component with the registry.
/// Components may not have the same name.
ResourceManager::ResourceManager(std::vector<ComponentBase> components) {
    for (auto& component : components) {
        register_component(component);
    }
}

void ResourceManager::register_service(ServiceBase service) {
    if (services.find(service.name) != services.end()) {
        std::string err = "Cannot add service with name " + service.name + " as it already exists.";
        throw std::runtime_error(err);
    }

    services[service.name] = service;
}

void ResourceManager::register_component(ComponentBase component) {
    if (components.find(component.name) != components.end()) {
        std::string err =
            "Cannot add component with name " + component.name + " as it already exists.";
        throw std::runtime_error(err);
    }

    components[component.name] = component;
}

std::unordered_map<std::string, ComponentBase> ResourceManager::components;

ServiceBase ResourceManager::get_service(std::string name, ServiceType of_type) {
    if (services.find(name) == services.end()) {
        throw "Service name " + name + " doesn't exist!";
    }

    ServiceBase service = services.at(name);
    if (service.type == of_type) {
        return service;
    }

    ServiceType base = ServiceType("ServiceBase");
    if (of_type == base) {
        return service;
    }
    throw "Service name " + name +
        " was found, but it has the wrong type! Expected type: " + of_type.name +
        ". Actual type: " + service.type.name;
}

ComponentBase ResourceManager::get_component(std::string name, ComponentType of_type) {
    if (components.find(name) == components.end()) {
        throw "Component name " + name + " doesn't exist!";
    }

    ComponentBase component = components.at(name);
    if (component.type == of_type) {
        return component;
    }

    ComponentType base = ComponentType("ComponentBase");
    if (of_type == base) {
        return component;
    }
    throw "Component name " + name +
        " was found, but it has the wrong type! Expected type: " + of_type.name +
        ". Actual type: " + component.type.name;
}
