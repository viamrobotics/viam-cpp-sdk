#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>

#include "component_base.hpp"
class ResourceManager {
   public:
    // registers provided components
    ResourceManager(std::vector<std::shared_ptr<ComponentBase>> components);
    ResourceManager();
    std::unordered_map<std::string, std::shared_ptr<ComponentBase>> components;
    void register_component(std::shared_ptr<ComponentBase> component);
    std::shared_ptr<ComponentBase> get_component(std::string name, ComponentType of_type);
};

#endif
