#pragma once

#include <boost/optional/optional.hpp>
#include <grpcpp/client_context.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

const std::string kComponent = "component";
const std::string kResource = "resource";
const std::string kService = "service";
const std::string kRDK = "rdk";
const std::string kBuiltin = "builtin";

using time_pt = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;

struct response_metadata {
    time_pt captured_at;

    static response_metadata from_proto(const viam::common::v1::ResponseMetadata& proto);
    static viam::common::v1::ResponseMetadata to_proto(const response_metadata& metadata);
};

bool operator==(const response_metadata& lhs, const response_metadata& rhs);

/// @brief convert a google::protobuf::Timestamp to time_point
time_pt timestamp_to_time_pt(const google::protobuf::Timestamp& timestamp);

/// @brief convert a time_point to a google::protobuf::Timestamp.
google::protobuf::Timestamp time_pt_to_timestamp(time_pt);

std::vector<unsigned char> string_to_bytes(std::string const& s);
std::string bytes_to_string(std::vector<unsigned char> const& b);

std::chrono::microseconds from_proto(const google::protobuf::Duration& proto);
google::protobuf::Duration to_proto(std::chrono::microseconds duration);

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
    void set_debug_key(const std::string& debug_key);

   private:
    void set_client_ctx_authority_();
    void add_viam_client_version_();
    grpc::ClientContext wrapped_context_;
};

/// @brief Given a fully qualified resource name, returns remote name (or "" if no remote name
/// exists) and short name
std::pair<std::string, std::string> long_name_to_remote_and_short(const std::string& long_name);

/// @brief Returns a new `ProtoStruct` with a random key for server-side debug logging
ProtoStruct debug_map();

/// @brief Returns a new `ProtoStruct` with @param debug_key for server-side debug logging
/// @throws Exception if the debug_key contains invalid (e.g., uppercase) gRPC characters
ProtoStruct debug_map(std::string debug_key);

/// @brief Adds @param debug_key for server-side debug logging to @param map
/// @throws Exception if the debug_key contains invalid (e.g., uppercase) gRPC characters
void add_debug_entry(ProtoStruct& map, std::string debug_key);

/// @brief Adds a random key to @param map for server-side debug logging
void add_debug_entry(ProtoStruct& map);

/// @brief Adds @param debug_key for server-side debug logging to @param map
/// @throws Exception if the debug_key contains invalid (e.g., uppercase) gRPC characters
/// @returns the new ProtoStruct
ProtoStruct with_debug_entry(ProtoStruct&& map, std::string debug_key);

/// @brief Adds a random key to @param map for server-side debug logging
/// @returns the new ProtoStruct
ProtoStruct with_debug_entry(ProtoStruct&& map);

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
/// ProtoStruct.
/// @param extra The extra ProtoStruct.
bool from_dm_from_extra(const ProtoStruct& extra);

}  // namespace sdk
}  // namespace viam
