#include <viam/sdk/common/utils.hpp>

#include <tuple>
#include <unordered_map>
#include <vector>

#include <boost/blank.hpp>
#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/components/component_base.hpp>
#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

using viam::common::v1::ResourceName;

std::vector<ResourceName> resource_names_for_resource(std::shared_ptr<Resource> resource) {
    std::string resource_type;
    std::vector<ResourceName> resource_names;
    for (auto& a : Registry::registered_resources()) {
        std::shared_ptr<ModelRegistration> reg = a.second;
        if (reg->subtype().to_string() == resource->dynamic_subtype().to_string()) {
            resource_type = reg->subtype().resource_subtype();
        } else {
            continue;
        }

        if (resource_type.empty()) {
            resource_type = resource->name();
        }

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

std::chrono::duration<double> from_proto(const google::protobuf::Duration& proto) {
    return std::chrono::duration<double, std::ratio<1>>(proto.seconds() +
                                                        proto.nanos() / (double)1e9);
}

google::protobuf::Duration to_proto(const std::chrono::duration<double>& duration) {
    google::protobuf::Duration proto;
    // TODO do this
    auto tp_sec = std::chrono::time_point_cast<std::chrono::seconds>(duration);
    proto.set_nanos(0);
    proto.set_seconds(0);
    return proto;
}

}  // namespace sdk
}  // namespace viam
