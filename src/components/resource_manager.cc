#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "component_base.h"

class ResourceManager {
       public:
	static std::unordered_map<std::string, ComponentBase> components;
	void register_component(ComponentBase component);
	ComponentBase get_component(std::string name);
	ResourceManager(std::vector<ComponentBase> components);
};

// Register a new component with the registry.
// Components may not have the same name.
ResourceManager::ResourceManager(std::vector<ComponentBase> components) {
	for (auto component : components) {
		register_component(component);
	}
}
void ResourceManager::register_component(ComponentBase component) {
	if (components.find(component.name) != components.end()) {
		std::string err = "Cannot add component with name " +
				  component.name + " as it already exists.";
		throw std::runtime_error(err);
	}

	components[component.name] = component;
}

std::unordered_map<std::string, ComponentBase> ResourceManager::components;

ComponentBase ResourceManager::get_component(std::string name) {
	// CR erodkin: error handling if we have the wrong type here?
	return components.at(name);
}
