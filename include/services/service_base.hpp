#pragma once

#include <common/v1/common.pb.h>

#include <resource/resource_base.hpp>
#include <string>

class ServiceType {
   public:
    friend bool operator==(ServiceType& lhs, ServiceType& rhs);
    ServiceType(std::string name) {
        name = name;
    }
    ServiceType() {
        name = "ServiceBase";
    }

   private:
    std::string name;
};

class ServiceBase : public ResourceBase {
   public:
    std::string name;
    ServiceType type;
    std::vector<ServiceType> service_hierarchy;
    virtual viam::common::v1::ResourceName get_resource_name(std::string name);
};

