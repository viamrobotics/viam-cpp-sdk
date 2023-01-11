#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <components/component_base.h>

#include <string>
#include <unordered_map>
class ResourceManager {
   public:
    // registers provided components
    ResourceManager(std::vector<ComponentBase> components);
    ResourceManager();
    static std::unordered_map<std::string, ComponentBase> components;
    void register_component(ComponentBase component);
    ComponentBase get_component(std::string name, ComponentType of_type);
};

#endif
