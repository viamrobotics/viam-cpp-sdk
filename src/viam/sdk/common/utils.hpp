#pragma once

#include <cmath>
#include <unordered_map>

#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <grpcpp/client_context.h>

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

struct response_metadata {
    std::chrono::time_point<long long, std::chrono::nanoseconds> captured_at;

    static response_metadata from_proto(const viam::common::v1::ResponseMetadata& proto);
    static viam::common::v1::ResponseMetadata to_proto(const response_metadata& metadata);
};

bool operator==(const response_metadata& lhs, const response_metadata& rhs);

/// @brief convert a google::protobuf::Timestamp to
/// std::chrono::time_point<long long, std::chrono::nanoseconds>
std::chrono::time_point<long long, std::chrono::nanoseconds> timestamp_to_time_pt(
    const google::protobuf::Timestamp& timestamp);

/// @brief convert a std::chrono::time_point<long long, std::chrono::nanoseconds> to
/// a google::protobuf::Timestamp.
google::protobuf::Timestamp time_pt_to_timestamp(
    const std::chrono::time_point<long long, std::chrono::nanoseconds>& time_pt);

std::vector<unsigned char> string_to_bytes(std::string const& s);
std::string bytes_to_string(std::vector<unsigned char> const& b);

std::chrono::microseconds from_proto(const google::protobuf::Duration& proto);
google::protobuf::Duration to_proto(const std::chrono::microseconds& duration);

// the authority on a grpc::ClientContext is sometimes set to an invalid uri on mac, causing
// `rust-utils` to fail to process gRPC requests. This class provides a convenience wrapper around a
// grpc ClientContext that allows us to make any necessary modifications to authority or else where
// to avoid runtime issues.
// For more details, see https://viam.atlassian.net/browse/RSDK-5194.
class ClientContext {
   public:
    ClientContext();
    operator grpc::ClientContext*();
    operator const grpc::ClientContext*() const;

   private:
    void set_client_ctx_authority_();
    grpc::ClientContext wrapped_context_;
};

/// @brief Set the boost trivial logger's severity depending on args.
/// @param argc The number of args.
/// @param argv The commandline arguments to parse.
///
/// Sets the boost trivial logger's severity to debug if "--log-level=debug" is the
/// the third argument. Sets severity to info otherwise. Useful for module
/// implementations. See LogLevel documentation in the RDK for more information on
/// how to start modules with a "log-level" commandline argument.
void set_logger_severity_from_args(int argc, char** argv);

/// @brief Used in modular filter components to get the 'fromDataManagement' value from an extra
/// AttributeMap.
/// @param extra The extra AttributeMap.
bool from_dm_from_extra(const AttributeMap& extra);

}  // namespace sdk
}  // namespace viam
