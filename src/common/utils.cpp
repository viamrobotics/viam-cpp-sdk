#include <common/v1/common.pb.h>

#include <boost/blank.hpp>
#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <common/utils.hpp>
#include <components/component_base.hpp>
#include <registry/registry.hpp>
#include <tuple>
#include <unordered_map>
#include <vector>

using viam::common::v1::ResourceName;

std::vector<ResourceName> resource_names_for_resource(std::shared_ptr<ResourceBase> resource) {
    std::string* resource_type;
    std::vector<ResourceName> resource_names;
    for (auto& a : Registry::registered_resources()) {
        std::shared_ptr<ResourceRegistration> reg = a.second;
        if (reg->resource_type.type == resource->type.type) {
            resource_type = &reg->resource_type.type;
        }
    }

    if (resource_type == nullptr) {
        resource_type = &resource->name;
    }

    // TODO (RSDK-1631): make sure we split properly on "viam.components."
    // within component_type as needed

    ResourceName r;
    *r.mutable_namespace_() = RDK;
    *r.mutable_type() = resource->type.type;
    *r.mutable_name() = resource->name;
    *r.mutable_subtype() = *resource_type;
    resource_names.push_back(r);
    return resource_names;
}

