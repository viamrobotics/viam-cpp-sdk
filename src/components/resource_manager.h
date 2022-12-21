#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>

#include "component_base.h"
class ResourceManager {
       public:
	ResourceManager(std::vector<ComponentBase> components);
	static std::unordered_map<std::string, ComponentBase> components;
	void register_component(ComponentBase component);
	ComponentBase get_component(std::string name);
};

#endif
