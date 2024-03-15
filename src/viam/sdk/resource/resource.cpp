#include <viam/sdk/resource/resource.hpp>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

using common::v1::ResourceName;

Resource::~Resource() = default;
Resource::Resource(std::string name) : name_(std::move(name)) {}

std::string Resource::name() const {
    return name_;
}

ResourceName Resource::get_resource_name(std::string name) const {
    ResourceName r;
    *r.mutable_namespace_() = kRDK;
    *r.mutable_type() = kResource;
    *r.mutable_subtype() = this->api().resource_subtype();
    *r.mutable_name() = std::move(name);

    return r;
}

std::vector<Name> Resource::resource_names() const {
    std::string resource_type;
    std::string resource_subtype;
    std::vector<Name> resource_names;
    for (const auto& kv : Registry::registered_models()) {
        const std::shared_ptr<const ModelRegistration> reg = kv.second;
        if (reg->api() == api()) {
            resource_type = reg->api().resource_type();
            resource_subtype = reg->api().resource_subtype();
        } else {
            continue;
        }

        if (resource_subtype.empty()) {
            resource_subtype = name();
        }

        resource_names.push_back({{kRDK, resource_type, resource_subtype}, "", name()});
    }
    return resource_names;
}
}  // namespace sdk
}  // namespace viam
