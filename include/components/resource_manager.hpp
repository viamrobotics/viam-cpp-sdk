#pragma once

#include <components/component_base.hpp>
#include <services/service_base.hpp>
#include <string>
#include <unordered_map>

class ResourceManager {
   public:
    static std::unordered_map<std::string, std::shared_ptr<ComponentBase>> components;
    static std::unordered_map<std::string, ServiceBase> services;
    void register_component(std::shared_ptr<ComponentBase> component);
    void register_service(ServiceBase service);

    /// returns a component from the registry.
    /// Args:
    /// 	std::string name: the name of the component
    ///
    /// Raises:
    /// 	If the name is not within the ResourceManager or the registered
    /// component's type is not the expected type, then register_component
    /// will throw an error.
    std::shared_ptr<ComponentBase> get_component(std::string name, ComponentType of_type);
    ServiceBase get_service(std::string name, ServiceType of_type);
    ResourceManager(std::vector<std::shared_ptr<ComponentBase>> components);
    ResourceManager();
};

