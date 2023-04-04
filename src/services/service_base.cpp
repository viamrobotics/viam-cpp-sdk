#include <services/service_base.hpp>

#include <string>

#include <common/v1/common.pb.h>

#include <common/utils.hpp>
#include <resource/resource_base.hpp>

ResourceName ServiceBase::get_resource_name(std::string name) {
    // TODO (RSDK-1631): test, confirm whether we need to split on
    // "viam.components" here
    ResourceName r;
    *r.mutable_namespace_() = RDK;
    *r.mutable_type() = SERVICE;
    *r.mutable_subtype() = this->type().to_string();
    *r.mutable_name() = std::move(name);

    return r;
}

ServiceBase::ServiceBase() : ResourceBase({"service"}){};
