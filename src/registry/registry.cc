#include <string>
#include <unordered_map>

#include "../components/component_base.h"
#include "../components/service_base.h"
#include "google/protobuf/struct.pb.h"
#include "grpcpp/channel.h"
#include "robot/v1/robot.pb.h"
using viam::robot::v1::Status;

class ComponentRegistration {
       public:
	ComponentType component_type;
	std::string name;
	ComponentServiceBase rpc_service;
	std::function<ComponentBase(std::string,
				    std::shared_ptr<grpc::Channel>)>
	    create_rpc_client;
	virtual Status create_status(ComponentBase component);
};

class Registry {
       public:
	// Registers a component with the Registry
	// Args:
	// 	component (ComponentRegistration): object containing component
	// 	registration data
	//
	// Raises:
	// 	throws error if component already exists in the registry
	void register_component(ComponentRegistration component);
	static ComponentRegistration lookup(std::string name);

	static std::unordered_map<std::string, ComponentRegistration>
	registered_components();

       private:
	static std::unordered_map<std::string, ComponentRegistration>
	    components;
};

std::unordered_map<std::string, ComponentRegistration> Registry::components;

void Registry::register_component(ComponentRegistration component) {
	if (components.find(component.name) != components.end()) {
		std::string err = "Cannot add component with name " +
				  component.name + "as it already exists";
		throw std::runtime_error(err);
	}

	components[component.name] = component;
}

ComponentRegistration Registry::lookup(std::string name) {
	if (components.find(name) == components.end()) {
		std::string err = "Component " + name + "not found.";
		err += name;
		throw std::runtime_error(err);
	}

	return components.at(name);
}

std::unordered_map<std::string, ComponentRegistration>
Registry::registered_components() {
	std::unordered_map<std::string, ComponentRegistration> registry;
	for (auto component : components) {
		registry.insert(component);
	}
	return registry;
}

Status ComponentRegistration::create_status(ComponentBase component) {
	Status status;
	google::protobuf::Struct struct_;
	*status.mutable_name() = component.get_resource_name(component.name);
	*status.mutable_status() = struct_;
	return status;
}
