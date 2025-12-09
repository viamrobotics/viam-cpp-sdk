#pragma once

#include <chrono>
#include <memory>

#include <boost/optional/optional.hpp>

#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace google {
namespace protobuf {

class Duration;
class Timestamp;

}  // namespace protobuf
}  // namespace google

namespace viam {
namespace common {
namespace v1 {

class ResponseMetadata;

}
}  // namespace common
}  // namespace viam

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
};

bool operator==(const response_metadata& lhs, const response_metadata& rhs);

namespace proto_convert_details {

template <>
struct to_proto_impl<time_pt> {
    void operator()(time_pt, google::protobuf::Timestamp*) const;
};

template <>
struct from_proto_impl<google::protobuf::Timestamp> {
    time_pt operator()(const google::protobuf::Timestamp*) const;
};

template <>
struct to_proto_impl<std::chrono::microseconds> {
    void operator()(std::chrono::microseconds, google::protobuf::Duration*) const;
};

template <>
struct from_proto_impl<google::protobuf::Duration> {
    std::chrono::microseconds operator()(const google::protobuf::Duration*) const;
};

template <>
struct to_proto_impl<response_metadata> {
    void operator()(const response_metadata&, common::v1::ResponseMetadata*) const;
};

template <>
struct from_proto_impl<common::v1::ResponseMetadata> {
    response_metadata operator()(const common::v1::ResponseMetadata*) const;
};

}  // namespace proto_convert_details

std::vector<unsigned char> string_to_bytes(std::string const& s);
std::string bytes_to_string(std::vector<unsigned char> const& b);

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

/// @brief Used in modular filter components to get the 'fromDataManagement' value from an extra
/// ProtoStruct.
/// @param extra The extra ProtoStruct.
bool from_dm_from_extra(const ProtoStruct& extra);

std::string get_env(const char* var);

}  // namespace sdk
}  // namespace viam
