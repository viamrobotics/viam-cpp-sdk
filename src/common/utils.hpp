#ifndef UTILS_H
#define UTILS_H

#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <unordered_map>

#include "../components/component_base.hpp"
#include "common/v1/common.pb.h"
#include "proto_type.hpp"

using viam::common::v1::ResourceName;

std::vector<viam::common::v1::ResourceName> resource_names_for_component(ComponentBase component);

class ResourceNameHasher {
   public:
    size_t operator()(ResourceName const& key) const {
        return std::hash<std::string>()(key.SerializeAsString());
    }
};
class ResourceNameEqual {
   public:
    bool operator()(ResourceName const& t1, ResourceName const& t2) const {
        return !(t1.SerializeAsString().compare(t2.SerializeAsString()));
    }
    static bool check_equal(const ResourceName r1, const ResourceName r2) {
        return r1.SerializeAsString().compare(r2.SerializeAsString());
    }
};

#endif
