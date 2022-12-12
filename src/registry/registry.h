#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>

#include "../components/service_base.h"
class ComponentRegistration {
       public:
	ComponentType component_type;
	std::string name;
	std::function<ComponentBase(std::string,
				    std::shared_ptr<grpc::Channel>)>
	    create_rpc_client;
};

class Registry {
       public:
	void register_component(ComponentRegistration component);
	static ComponentRegistration lookup(std::string name);
	static std::unordered_map<std::string, ComponentRegistration>
	registered_components();
};

#endif
