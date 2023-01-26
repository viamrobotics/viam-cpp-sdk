#ifndef COMPONENT_BASE_H
#define COMPONENT_BASE_H

#include <common/v1/common.pb.h>
#include <google/protobuf/struct.pb.h>

#include <resource/resource_base.hpp>
#include <string>

class ComponentType {
   public:
    std::string name;
    ComponentType(){};
    ComponentType(std::string name) {
        name = name;
    }
    friend bool operator==(ComponentType& lhs, ComponentType& rhs);
};

class ComponentBase : public ResourceBase {
   public:
    std::string name;
    ComponentType type;
    // CR erodkin: is component hierarchy necessary?
    std::vector<ComponentType> component_hierarchy;

    viam::common::v1::ResourceName get_resource_name(std::string name);
};

#endif
