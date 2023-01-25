#ifndef COMPONENT_BASE_H
#define COMPONENT_BASE_H

#include <grpcpp/support/status.h>

#include <string>

#include "../common/proto_type.hpp"
#include "common/v1/common.pb.h"
#include "google/protobuf/struct.pb.h"

class ComponentType {
    public:
        std::string name;
        ComponentType(std::string name) {
            name = name;
        }
        ComponentType() {
            name = "ComponentBase";
        }
        friend bool operator==(ComponentType& lhs, ComponentType& rhs);
};

class ComponentBase {
    public:
        ComponentBase();
        std::string name;
        static ComponentType type;
        std::vector<ComponentType> component_hierarchy;

        static viam::common::v1::ResourceName get_resource_name(std::string name);
        virtual grpc::StatusCode stop(std::unordered_map<std::string, ProtoType> extra);
        virtual grpc::StatusCode stop();
};

#endif
