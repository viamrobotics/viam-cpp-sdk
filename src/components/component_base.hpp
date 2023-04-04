#pragma once

#include <string>

#include <google/protobuf/struct.pb.h>

#include <common/v1/common.pb.h>

#include <resource/resource_base.hpp>

class ComponentBase : public ResourceBase {
   public:
    viam::common::v1::ResourceName get_resource_name(std::string name);
    ComponentBase();
};
