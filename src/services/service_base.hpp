#pragma once

#include <string>

#include <common/v1/common.pb.h>

#include <resource/resource_base.hpp>

class ServiceBase : public ResourceBase {
   public:
    virtual viam::common::v1::ResourceName get_resource_name(std::string name);
    ServiceBase();
};
