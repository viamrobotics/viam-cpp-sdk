// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/api/field_behavior.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2ffield_5fbehavior_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2ffield_5fbehavior_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021011 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/descriptor.pb.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_google_2fapi_2ffield_5fbehavior_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_google_2fapi_2ffield_5fbehavior_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace google {
namespace api {

enum FieldBehavior : int {
  FIELD_BEHAVIOR_UNSPECIFIED = 0,
  OPTIONAL = 1,
  REQUIRED = 2,
  OUTPUT_ONLY = 3,
  INPUT_ONLY = 4,
  IMMUTABLE = 5,
  UNORDERED_LIST = 6,
  NON_EMPTY_DEFAULT = 7,
  FieldBehavior_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  FieldBehavior_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool FieldBehavior_IsValid(int value);
constexpr FieldBehavior FieldBehavior_MIN = FIELD_BEHAVIOR_UNSPECIFIED;
constexpr FieldBehavior FieldBehavior_MAX = NON_EMPTY_DEFAULT;
constexpr int FieldBehavior_ARRAYSIZE = FieldBehavior_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* FieldBehavior_descriptor();
template<typename T>
inline const std::string& FieldBehavior_Name(T enum_t_value) {
  static_assert(::std::is_same<T, FieldBehavior>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function FieldBehavior_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    FieldBehavior_descriptor(), enum_t_value);
}
inline bool FieldBehavior_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, FieldBehavior* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<FieldBehavior>(
    FieldBehavior_descriptor(), name, value);
}
// ===================================================================


// ===================================================================

static const int kFieldBehaviorFieldNumber = 1052;
extern ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::PROTOBUF_NAMESPACE_ID::FieldOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::RepeatedEnumTypeTraits< ::google::api::FieldBehavior, ::google::api::FieldBehavior_IsValid>, 14, true >
  field_behavior;

// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace api
}  // namespace google

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::google::api::FieldBehavior> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::google::api::FieldBehavior>() {
  return ::google::api::FieldBehavior_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2ffield_5fbehavior_2eproto
