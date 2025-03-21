#include <viam/sdk/resource/resource.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

Resource::~Resource() = default;
Resource::Resource(std::string name) : name_(std::move(name)) {}

std::string Resource::name() const {
    return name_;
}

Name Resource::get_resource_name(const std::string& type) const {
    auto name_parts = long_name_to_remote_and_short(name_);
    return {API(api().type_namespace(), type, api().resource_subtype()),
            name_parts.first,
            name_parts.second};
}

Name Resource::get_resource_name() const {
    return get_resource_name(kResource);
}

// default behavior is to just return
void Resource::close() {}

}  // namespace sdk
}  // namespace viam
