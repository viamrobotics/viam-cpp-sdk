// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/api/field_behavior.proto

#include "google/api/field_behavior.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace google {
namespace api {
}  // namespace api
}  // namespace google
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_google_2fapi_2ffield_5fbehavior_2eproto[1];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_google_2fapi_2ffield_5fbehavior_2eproto = nullptr;
const uint32_t TableStruct_google_2fapi_2ffield_5fbehavior_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_google_2fapi_2ffield_5fbehavior_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\037google/api/field_behavior.proto\022\ngoogl"
  "e.api\032 google/protobuf/descriptor.proto*"
  "\246\001\n\rFieldBehavior\022\036\n\032FIELD_BEHAVIOR_UNSP"
  "ECIFIED\020\000\022\014\n\010OPTIONAL\020\001\022\014\n\010REQUIRED\020\002\022\017\n"
  "\013OUTPUT_ONLY\020\003\022\016\n\nINPUT_ONLY\020\004\022\r\n\tIMMUTA"
  "BLE\020\005\022\022\n\016UNORDERED_LIST\020\006\022\025\n\021NON_EMPTY_D"
  "EFAULT\020\007:`\n\016field_behavior\022\035.google.prot"
  "obuf.FieldOptions\030\234\010 \003(\0162\031.google.api.Fi"
  "eldBehaviorR\rfieldBehaviorBp\n\016com.google"
  ".apiB\022FieldBehaviorProtoP\001ZAgoogle.golan"
  "g.org/genproto/googleapis/api/annotation"
  "s;annotations\242\002\004GAPIb\006proto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto_deps[1] = {
  &::descriptor_table_google_2fprotobuf_2fdescriptor_2eproto,
};
static ::_pbi::once_flag descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto = {
    false, false, 468, descriptor_table_protodef_google_2fapi_2ffield_5fbehavior_2eproto,
    "google/api/field_behavior.proto",
    &descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto_once, descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto_deps, 1, 0,
    schemas, file_default_instances, TableStruct_google_2fapi_2ffield_5fbehavior_2eproto::offsets,
    nullptr, file_level_enum_descriptors_google_2fapi_2ffield_5fbehavior_2eproto,
    file_level_service_descriptors_google_2fapi_2ffield_5fbehavior_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto_getter() {
  return &descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_google_2fapi_2ffield_5fbehavior_2eproto(&descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto);
namespace google {
namespace api {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* FieldBehavior_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto);
  return file_level_enum_descriptors_google_2fapi_2ffield_5fbehavior_2eproto[0];
}
bool FieldBehavior_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      return true;
    default:
      return false;
  }
}

PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::PROTOBUF_NAMESPACE_ID::FieldOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::RepeatedEnumTypeTraits< ::google::api::FieldBehavior, ::google::api::FieldBehavior_IsValid>, 14, true>
  field_behavior(kFieldBehaviorFieldNumber, static_cast< ::google::api::FieldBehavior >(0), nullptr);

// @@protoc_insertion_point(namespace_scope)
}  // namespace api
}  // namespace google
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
