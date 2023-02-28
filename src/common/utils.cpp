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

std::vector<ResourceName> resource_names_for_component(std::shared_ptr<ComponentBase> component) {
    std::string* component_type;
    std::vector<ResourceName> resource_names;
    for (auto& a : Registry::registered_components()) {
        std::shared_ptr<ComponentRegistration> reg = a.second;
        if (reg->component_type == component->type) {
            component_type = &reg->component_type.name;
        }
    }

    if (component_type == nullptr) {
        component_type = &component->name;
    }

    // TODO (RSDK-1631): make sure we split properly on "viam.components."
    // within component_type as needed

    ResourceName r;
    *r.mutable_namespace_() = RDK;
    *r.mutable_type() = COMPONENT;
    *r.mutable_name() = component->name;
    *r.mutable_subtype() = *component_type;
    resource_names.push_back(r);
    return resource_names;
}

