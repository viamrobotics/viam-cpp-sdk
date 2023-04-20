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
    auto r = this->ResourceBase::get_resource_name(name);
    *r.mutable_type() = COMPONENT;
    return r;
}

ResourceType ComponentBase::type() {
    return {COMPONENT};
}

}  // namespace cppsdk
}  // namespace viam
