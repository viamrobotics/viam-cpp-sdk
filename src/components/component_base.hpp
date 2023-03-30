#pragma once

#include <string>

#include <google/protobuf/struct.pb.h>

#include <common/v1/common.pb.h>

#include <components/component_type.hpp>
#include <resource/resource_base.hpp>

class ComponentBase : public ResourceBase {
   public:
    ComponentType type;
    std::vector<ComponentType> component_hierarchy;

    viam::common::v1::ResourceName get_resource_name(std::string name);
    ComponentBase();
};
