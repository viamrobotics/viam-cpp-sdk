#include <viam/sdk/components/component_base.hpp>

#include <string>

#include <google/protobuf/struct.pb.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource_base.hpp>
#include <viam/sdk/resource/resource_type.hpp>

namespace viam {
namespace sdk {

using viam::common::v1::ResourceName;

ResourceName ComponentBase::get_resource_name(std::string name) {
    auto r = this->ResourceBase::get_resource_name(name);
    *r.mutable_type() = COMPONENT;
    return r;
}

ResourceType ComponentBase::type() const {
    return {COMPONENT};
}

}  // namespace sdk
}  // namespace viam
