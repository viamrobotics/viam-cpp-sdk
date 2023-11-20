#include <viam/sdk/resource/resource.hpp>

#include <stdexcept>
#include <unordered_map>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

Resource::~Resource() = default;

void Resource::stop(const AttributeMap& extra) {
    throw std::runtime_error("Unimplemented");
}

std::string Resource::name() const {
    return name_;
}

API Resource::static_api() {
    return {kRDK, kResource, "Resource"};
}

void Resource::reconfigure(Dependencies deps, ResourceConfig cfg){};

ResourceType Resource::type() const {
    return {kResource};
}

ResourceName Resource::get_resource_name(std::string name) {
    ResourceName r;
    *r.mutable_namespace_() = kRDK;
    *r.mutable_type() = kResource;
    *r.mutable_subtype() = this->dynamic_api().resource_subtype();
    *r.mutable_name() = std::move(name);

    return r;
}

}  // namespace sdk
}  // namespace viam
