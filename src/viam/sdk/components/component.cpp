#include <viam/sdk/components/component.hpp>

#include <string>

#include <google/protobuf/struct.pb.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

using viam::common::v1::ResourceName;

Component::Component() : Resource("component"){};

Component::Component(std::string name) : Resource(std::move(name)){};

ResourceName Component::get_resource_name(std::string name) const {
    auto r = this->Resource::get_resource_name(name);
    *r.mutable_type() = kComponent;
    return r;
}

}  // namespace sdk
}  // namespace viam
