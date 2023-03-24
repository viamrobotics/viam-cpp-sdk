#pragma once

#include <common/v1/common.pb.h>

#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <common/proto_type.hpp>
#include <components/component_base.hpp>
#include <unordered_map>

using viam::common::v1::ResourceName;
const std::string COMPONENT = "component";
const std::string SERVICE = "service";
const std::string RDK = "rdk";
const std::string GENERIC = "generic";
const std::string BUILTIN = "builtin";

std::vector<viam::common::v1::ResourceName> resource_names_for_resource(
    std::shared_ptr<ResourceBase> resource);

class ResourceNameHasher {
 public:
  size_t operator()(ResourceName const &key) const {
    return std::hash<std::string>()(key.SerializeAsString());
  }
};
class ResourceNameEqual {
 public:
  bool operator()(ResourceName const &t1, ResourceName const &t2) const {
    return !(t1.SerializeAsString().compare(t2.SerializeAsString()));
  }
  static bool check_equal(const ResourceName r1, const ResourceName r2) {
    return r1.SerializeAsString().compare(r2.SerializeAsString());
  }
};

std::vector<unsigned char> string_to_bytes(std::string const &s);
std::string bytes_to_string(std::vector<unsigned char> const &b);
