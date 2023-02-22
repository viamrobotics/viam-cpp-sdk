#pragma once

#include <common/v1/common.pb.h>
#include <google/protobuf/struct.pb.h>

#include <components/component_type.hpp>
#include <resource/resource_base.hpp>
#include <string>
class ComponentBase : public ResourceBase {
   public:
    std::string name;
    ComponentType type;
    std::vector<ComponentType> component_hierarchy;

    viam::common::v1::ResourceName get_resource_name(std::string name);
    ComponentBase();
};

