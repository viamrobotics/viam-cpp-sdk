#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>

#include "../components/service_base.h"
class ComponentRegistration {
       public:
	std::function<ComponentBase(std::string,
				    std::shared_ptr<grpc::Channel>)>
	    create_rpc_client;
};

class Registry {
       public:
	void register_component(ComponentRegistration component);
	static ComponentRegistration lookup(std::string name);
};

#endif
