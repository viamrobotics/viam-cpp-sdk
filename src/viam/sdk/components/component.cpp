#include <viam/sdk/components/component.hpp>

#include <string>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

Component::Component() : Resource("component") {}

Component::Component(std::string name) : Resource(std::move(name)) {}

Name Component::get_resource_name() const {
    return Resource::get_resource_name(kComponent);
}

}  // namespace sdk
}  // namespace viam
