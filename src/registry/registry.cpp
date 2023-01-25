#include <string>
#include <unordered_map>

#include "../components/component_base.hpp"
#include "../components/service_base.hpp"
#include "google/protobuf/struct.pb.h"
#include "grpcpp/channel.h"
#include "robot/v1/robot.pb.h"
#include "registry.hpp"
using viam::robot::v1::Status;

ComponentRegistration::ComponentRegistration() {}
ComponentRegistration::ComponentRegistration(ComponentType component_type_, std::string name_, std::function<std::shared_ptr<ComponentBase>(std::string, std::shared_ptr<grpc::Channel>)> create_rpc_client_) {
    component_type = component_type_;
    name = name_;
    create_rpc_client = create_rpc_client_;
}

std::unordered_map<std::string, ComponentRegistration> Registry::components;

void Registry::register_component(ComponentRegistration component) {
    if (components.find(component.name) != components.end()) {
        std::string err =
            "Cannot add component with name " + component.name + "as it already exists";
        throw std::runtime_error(err);
    }

    components.emplace(component.name, component);
    // components[component.name] = component;
}

ComponentRegistration Registry::lookup(std::string name) {
    if (components.find(name) == components.end()) {
        std::string err = "Component " + name + " not found.";
        err += " " + name;
        throw std::runtime_error(err);
    }

    return components.at(name);
}

std::unordered_map<std::string, ComponentRegistration> Registry::registered_components() {
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
