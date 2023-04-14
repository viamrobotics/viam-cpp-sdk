#include <components/component_base.hpp>

#include <string>

#include <google/protobuf/struct.pb.h>

#include <common/v1/common.pb.h>

#include <common/utils.hpp>
#include <resource/resource_base.hpp>
#include <resource/resource_type.hpp>

namespace viam {
namespace cppsdk {

using viam::common::v1::ResourceName;

ResourceName ComponentBase::get_resource_name(std::string name) {
    // TODO (RSDK-1631): test, confirm whether we need to split on
    // "viam.components" here
    ResourceName r;
    *r.mutable_namespace_() = RDK;
    *r.mutable_type() = COMPONENT;
    *r.mutable_subtype() = this->type().to_string();
    *r.mutable_name() = std::move(name);

    return r;
}

ResourceType ComponentBase::type() {
    return {COMPONENT};
}

}  // namespace cppsdk
}  // namespace viam
