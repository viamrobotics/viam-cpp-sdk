#pragma once

#include <common/v1/common.pb.h>
#include <google/protobuf/struct.pb.h>

#include <resource/resource_base.hpp>
#include <string>
class ComponentType {
   public:
    std::string name;
    friend bool operator==(ComponentType& lhs, ComponentType& rhs);
    ComponentType(std::string name) {
        name = name;
    }
    ComponentType() {
        name = "ComponentBase";
    }
};

class ComponentBase : public ResourceBase {
   public:
    std::string name;
    ComponentType type;
    std::vector<ComponentType> component_hierarchy;

    viam::common::v1::ResourceName get_resource_name(std::string name);
    ComponentBase();
};

