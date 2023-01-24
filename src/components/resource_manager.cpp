#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "component_base.hpp"
#include "resource_manager.hpp"

// class ResourceManager {
//    public:
//     static std::unordered_map<std::string, ComponentBase> components;
//     void register_component(ComponentBase component);

//     // returns a component from the registry.
//     // Args:
//     // 	std::string name: the name of the component
//     //
//     // Raises:
//     // 	If the name is not within the ResourceManager or the registered
//     // component's type is not the expected type, then register_component
//     // will throw an error.
//     ComponentBase get_component(std::string name, ComponentType of_type);
//     ResourceManager(std::vector<ComponentBase> components);
//     ResourceManager();
// };

ResourceManager::ResourceManager() {}

// Register a new component with the registry.
// Components may not have the same name.
ResourceManager::ResourceManager(std::vector<std::shared_ptr<ComponentBase>> components) {
    for (auto component : components) {
        register_component(component);
    }
}
void ResourceManager::register_component(std::shared_ptr<ComponentBase> component) {
    if (components.find(component->name) != components.end()) {
        std::string err =
            "Cannot add component with name " + component->name + " as it already exists.";
        throw std::runtime_error(err);
    }

    components[component->name] = component;
}

std::unordered_map<std::string, std::shared_ptr<ComponentBase>> ResourceManager::components;

std::shared_ptr<ComponentBase> ResourceManager::get_component(std::string name, ComponentType of_type) {
    if (components.find(name) == components.end()) {
        throw "Component name " + name + " doesn't exist!";
    }

    std::shared_ptr<ComponentBase> component = components.at(name);
    if (component->type == of_type) {
        return component;
    }

    ComponentType base = ComponentType("ComponentBase");
    if (of_type == base) {
        return component;
    }
    throw "Component name " + name +
        " was found, but it has the wrong type! Expected type: " + of_type.name +
        ". Actual type: " + component->type.name;
}
