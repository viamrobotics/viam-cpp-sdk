#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "component_base.h"

class ResourceManager {
       public:
	static std::unordered_map<std::string, ComponentBase> components;
	void register_component(ComponentBase component);

	// returns a component from the registry.
	// Args:
	// 	std::string name: the name of the component
	//
	// Raises:
	// 	If the name is not within the ResourceManager then
	// register_component will throw an error.
	//
	// TODO(ethan): pass expected type to this function, raise if it's not
	// what we receive.
	ComponentBase get_component(std::string name);
	ResourceManager(std::vector<ComponentBase> components);
	ResourceManager();
};

ResourceManager::ResourceManager() {}

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
	if (components.find(name) == components.end()) {
		throw "Component name " + name + " doesn't exist!";
	}
	return components.at(name);
}
