#pragma once

#include <common/v1/common.pb.h>

#include <resource/resource_base.hpp>
#include <string>

#include "services/service_type.hpp"

class ServiceBase : public ResourceBase {
   public:
    ServiceType type;
    std::vector<ServiceType> service_hierarchy;
    virtual viam::common::v1::ResourceName get_resource_name(std::string name);
    ServiceBase();
};
