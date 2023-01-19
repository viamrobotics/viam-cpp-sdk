#ifndef COMPONENT_BASE_H
#define COMPONENT_BASE_H

#include <grpcpp/support/status.h>

#include <string>

#include "../common/proto_type.h"
#include "common/v1/common.pb.h"
#include "google/protobuf/struct.pb.h"

class ComponentType {
   public:
    std::string name;
    ComponentType(){};
    ComponentType(std::string name) {
        name = name;
    }
    friend bool operator==(ComponentType& lhs, ComponentType& rhs);
};

class ComponentBase {
   public:
    std::string name;
    ComponentType type;
    std::vector<ComponentType> component_hierarchy;

    viam::common::v1::ResourceName get_resource_name(std::string name);
    virtual grpc::StatusCode stop(std::unordered_map<std::string, ProtoType> extra);
    virtual grpc::StatusCode stop();
};

#endif
