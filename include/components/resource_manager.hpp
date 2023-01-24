#pragma once

#include <components/component_base.h>

#include <services/service_base.hpp>
#include <string>
#include <unordered_map>

class ResourceManager {
       public:
	// registers provided components
	ResourceManager(std::vector<ComponentBase> components);
	ResourceManager();
	void register_component(ComponentBase component);
	ComponentBase get_component(std::string name, ComponentType of_type);

	void register_service(ServiceBase service);

       private:
	static std::unordered_map<std::string, ComponentBase> components;
	static std::unordered_map<std::string, ServiceBase> services;
};

