#pragma once

#include <cmath>
#include <unordered_map>

#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/component.hpp>

namespace viam {
namespace sdk {

using viam::common::v1::ResourceName;
const std::string kComponent = "component";
const std::string kResource = "resource";
const std::string kService = "service";
const std::string kRDK = "rdk";
const std::string kBuiltin = "builtin";

std::vector<viam::common::v1::ResourceName> resource_names_for_resource(
    const std::shared_ptr<Resource>& resource);

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

std::vector<unsigned char> string_to_bytes(std::string const& s);
std::string bytes_to_string(std::vector<unsigned char> const& b);

std::chrono::microseconds from_proto(const google::protobuf::Duration& proto);
google::protobuf::Duration to_proto(const std::chrono::microseconds& duration);

/// @brief Set the boost trivial logger's severity depending on args.
/// @param argc The number of args.
/// @param argv The commandline arguments to parse.
///
/// Sets the boost trivial logger's severity to debug if "--log-level=debug" is the
/// the third argument. Sets severity to info otherwise. Useful for module
/// implementations. See LogLevel documentation in the RDK for more information on
/// how to start modules with a "log-level" commandline argument.
void set_logger_severity_from_args(int argc, char** argv);

}  // namespace sdk
}  // namespace viam
