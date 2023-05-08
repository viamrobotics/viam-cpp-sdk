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

std::chrono::microseconds from_proto(const google::protobuf::Duration& proto) {
    namespace sc = std::chrono;
    const sc::seconds seconds_part{proto.seconds()};
    const sc::nanoseconds nanos_part{proto.nanos()};

    const sc::microseconds from_seconds = sc::duration_cast<sc::microseconds>(seconds_part);
    sc::microseconds from_nanos = sc::duration_cast<sc::microseconds>(nanos_part);

    if ((nanos_part.count() < 0) && (from_nanos > nanos_part)) {
        from_nanos -= sc::microseconds(1);
    } else if ((nanos_part.count() > 0) && (from_nanos < nanos_part)) {
        from_nanos += sc::microseconds(1);
    }
    return from_seconds + from_nanos;
}

google::protobuf::Duration to_proto(const std::chrono::microseconds& duration) {
    google::protobuf::Duration proto;
    int64_t total_micros = duration.count();
    int64_t micros_part = total_micros % 1000000;
    int64_t seconds_part = (total_micros - micros_part) / 1000000;

    proto.set_nanos(micros_part * 1000);
    proto.set_seconds(seconds_part);
    return proto;
}

}  // namespace sdk
}  // namespace viam
