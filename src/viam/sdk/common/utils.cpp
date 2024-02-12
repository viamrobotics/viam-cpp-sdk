#include <viam/sdk/common/utils.hpp>

#include <tuple>
#include <unordered_map>
#include <vector>

#include <boost/blank.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <grpcpp/client_context.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/components/component.hpp>
#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

using viam::common::v1::ResourceName;
using time_point = std::chrono::time_point<long long, std::chrono::nanoseconds>;

std::vector<ResourceName> resource_names_for_resource(const std::shared_ptr<Resource>& resource) {
    std::string resource_type;
    std::string resource_subtype;
    std::vector<ResourceName> resource_names;
    for (const auto& kv : Registry::registered_models()) {
        const std::shared_ptr<const ModelRegistration> reg = kv.second;
        if (reg->api().to_string() == resource->api().to_string()) {
            resource_type = reg->api().resource_type();
            resource_subtype = reg->api().resource_subtype();
        } else {
            continue;
        }

        if (resource_subtype.empty()) {
            resource_subtype = resource->name();
        }

        ResourceName r;
        *r.mutable_namespace_() = kRDK;
        *r.mutable_name() = resource->name();
        *r.mutable_type() = resource_type;
        *r.mutable_subtype() = resource_subtype;
        resource_names.push_back(r);
    }
    return resource_names;
}

std::vector<unsigned char> string_to_bytes(const std::string& s) {
    std::vector<unsigned char> bytes(s.begin(), s.end());
    return bytes;
};

std::string bytes_to_string(const std::vector<unsigned char>& b) {
    std::string img_string(b.begin(), b.end());
    return img_string;
};

time_point timestamp_to_time_pt(const google::protobuf::Timestamp& timestamp) {
    const std::chrono::seconds seconds(timestamp.seconds());
    const std::chrono::nanoseconds nanos(timestamp.nanos());
    return time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(seconds) +
                      nanos);
}

google::protobuf::Timestamp time_pt_to_timestamp(const time_point& time_pt) {
    const std::chrono::seconds duration_s =
        std::chrono::duration_cast<std::chrono::seconds>(time_pt.time_since_epoch());
    const std::chrono::nanoseconds duration_ns = time_pt.time_since_epoch() - duration_s;
    google::protobuf::Timestamp timestamp;
    timestamp.set_seconds(duration_s.count());
    timestamp.set_nanos(static_cast<int32_t>(duration_ns.count()));
    return timestamp;
}

response_metadata response_metadata::from_proto(const viam::common::v1::ResponseMetadata& proto) {
    response_metadata metadata;
    metadata.captured_at = timestamp_to_time_pt(proto.captured_at());
    return metadata;
}

viam::common::v1::ResponseMetadata response_metadata::to_proto(const response_metadata& metadata) {
    viam::common::v1::ResponseMetadata proto;
    google::protobuf::Timestamp ts = time_pt_to_timestamp(metadata.captured_at);
    *proto.mutable_captured_at() = std::move(ts);
    return proto;
}

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
    namespace sc = std::chrono;

    const sc::seconds seconds = sc::duration_cast<sc::seconds>(duration);
    const sc::nanoseconds nanos = duration - seconds;

    google::protobuf::Duration proto;
    proto.set_nanos(static_cast<int32_t>(nanos.count()));
    proto.set_seconds(seconds.count());
    return proto;
}

void set_logger_severity_from_args(int argc, char** argv) {
    if (argc >= 3 && strcmp(argv[2], "--log-level=debug") == 0) {
        boost::log::core::get()->set_filter(boost::log::trivial::severity >=
                                            boost::log::trivial::debug);
        return;
    }
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
}

bool operator==(const response_metadata& lhs, const response_metadata& rhs) {
    return lhs.captured_at == rhs.captured_at;
}

void ClientContext::set_client_ctx_authority_() {
    wrapped_context_.set_authority("viam-placeholder");
}

ClientContext::ClientContext() {
    set_client_ctx_authority_();
}

ClientContext::operator const grpc::ClientContext*() const {
    return &wrapped_context_;
}

ClientContext::operator grpc::ClientContext*() {
    return &wrapped_context_;
}

bool from_dm_from_extra(const AttributeMap& extra) {
    if (!extra) {
        return false;
    }
    auto pos = extra->find("fromDataManagement");
    if (pos != extra->end()) {
        ProtoType value = *(pos->second);
        const bool* boolValue = value.get<bool>();
        if (boolValue) {
            return *boolValue;
        }
    }
    return false;
}

}  // namespace sdk
}  // namespace viam
