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
Resource::Resource(std::string name) : name_(std::move(name)) {}

std::string Resource::name() const {
    return name_;
}

void Resource::reconfigure(const Dependencies& deps, const ResourceConfig& cfg){};

ResourceName Resource::get_resource_name(std::string name) const {
    ResourceName r;
    *r.mutable_namespace_() = kRDK;
    *r.mutable_type() = kResource;
    *r.mutable_subtype() = this->api().resource_subtype();
    *r.mutable_name() = std::move(name);

    return r;
}

}  // namespace sdk
}  // namespace viam
