#include <viam/sdk/resource/resource.hpp>

#include <stdexcept>
#include <unordered_map>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

Resource::~Resource() = default;
Resource::Resource(std::string name) : name_(std::move(name)) {}

std::vector<Name> Resource::resource_names_for_resource() {
    std::string resource_type;
    std::string resource_subtype;
    std::vector<Name> resource_names;
    for (auto& kv : Registry::registered_models()) {
        const std::shared_ptr<ModelRegistration> reg = kv.second;
        if (reg->api().to_string() == api().to_string()) {
            resource_type = reg->api().resource_type();
            resource_subtype = reg->api().resource_subtype();
        } else {
            continue;
        }

        if (resource_subtype.empty()) {
            resource_subtype = name();
        }

        std::string name_str;
        name_str.append(kRDK)
            .append(":")
            .append(resource_type)
            .append(":")
            .append(resource_subtype)
            .append("/")
            .append(name());
        resource_names.push_back(Name::from_string(name_str));
    }
    return resource_names;
}

std::string Resource::name() const {
    return name_;
}

void Resource::reconfigure(Dependencies deps, ResourceConfig cfg){};

common::v1::ResourceName Resource::get_resource_name(std::string name) {
    common::v1::ResourceName r;
    *r.mutable_namespace_() = kRDK;
    *r.mutable_type() = kResource;
    *r.mutable_subtype() = this->api().resource_subtype();
    *r.mutable_name() = std::move(name);

    return r;
}

}  // namespace sdk
}  // namespace viam
