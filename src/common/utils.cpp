#include <common/utils.hpp>

#include <tuple>
#include <unordered_map>
#include <vector>

#include <boost/blank.hpp>
#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

#include <common/v1/common.pb.h>

#include <components/component_base.hpp>
#include <registry/registry.hpp>

using viam::common::v1::ResourceName;
using namespace viam::cppsdk;

std::vector<ResourceName> resource_names_for_resource(std::shared_ptr<ResourceBase> resource) {
    std::string resource_type;
    std::vector<ResourceName> resource_names;
    for (auto& a : Registry::registered_resources()) {
        std::shared_ptr<ModelRegistration> reg = a.second;
        if (reg->resource_type.to_string() == resource->type().to_string()) {
            resource_type = reg->resource_type.to_string();
        }

        if (resource_type == "") {
            resource_type = resource->name();
        }

        // TODO (RSDK-1631): make sure we split properly on "viam.components."
        // within component_type as needed

        ResourceName r;
        *r.mutable_namespace_() = RDK;
        *r.mutable_type() = resource->type().to_string();
        *r.mutable_name() = resource->name();
        *r.mutable_subtype() = resource_type;
        resource_names.push_back(r);
    }
    return resource_names;
}

std::vector<unsigned char> string_to_bytes(std::string const& s) {
    std::vector<unsigned char> bytes(s.begin(), s.end());
    return bytes;
};

std::string bytes_to_string(std::vector<unsigned char> const& b) {
    std::string img_string(b.begin(), b.end());
    return img_string;
};
