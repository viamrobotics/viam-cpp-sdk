#include <common/v1/common.pb.h>
#include <google/protobuf/struct.pb.h>

#include <common/utils.hpp>
#include <components/component_base.hpp>
#include <resource/resource_base.hpp>
#include <string>

using viam::common::v1::ResourceName;

ResourceName ComponentBase::get_resource_name(std::string name_) {
    // TODO (RSDK-1631): test, confirm whether we need to split on
    // "viam.components" here
    ResourceName r;
    *r.mutable_namespace_() = RDK;
    *r.mutable_type() = COMPONENT;
    *r.mutable_subtype() = name;
    *r.mutable_name() = name_;

    return r;
}

ComponentBase::ComponentBase() : ResourceBase({"component"}){};
