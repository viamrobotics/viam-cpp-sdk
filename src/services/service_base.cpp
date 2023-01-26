#include <common/v1/common.pb.h>

#include <common/utils.hpp>
#include <resource/resource_base.hpp>
#include <string>

class ServiceType {
    std::string name;
    friend bool operator==(ServiceType& lhs, ServiceType& rhs);
    ServiceType(std::string name) {
        name = name;
    }
    ServiceType() {
        name = "ServiceBase";
    }
};

class ServiceBase : public ResourceBase {
    // CR erodkin: do we need more? see ComponentBase for model
    ServiceType type;
    std::string name;
    ResourceName get_resource_name(std::string name);
    ServiceBase();
};

ResourceName ServiceBase::get_resource_name(std::string name_) {
    // TODO (RSDK-1631): test, confirm whether we need to split on
    // "viam.components" here
    ResourceName r;
    *r.mutable_namespace_() = "rdk";
    *r.mutable_type() = "service";
    *r.mutable_subtype() = name;
    *r.mutable_name() = name_;

    return r;
}

