#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>

#include "../components/service_base.h"
#include "grpcpp/channel.h"
#include "robot/v1/robot.pb.h"

class ComponentRegistration {
   public:
    ComponentRegistration();
    ComponentType component_type;
    std::string name;
    std::function<ComponentBase(std::string, std::shared_ptr<grpc::Channel>)> create_rpc_client;
    virtual viam::robot::v1::Status create_status(ComponentBase component);
};

class Registry {
   public:
    void register_component(ComponentRegistration component);
    static ComponentRegistration lookup(std::string name);
    static std::unordered_map<std::string, ComponentRegistration> registered_components();
};

#endif
