// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/api/field_behavior.proto

#include "google/api/field_behavior.pb.h"

#include <algorithm>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
#include "google/protobuf/generated_message_tctable_impl.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;
namespace google {
namespace api {
}  // namespace api
}  // namespace google
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_google_2fapi_2ffield_5fbehavior_2eproto[1];
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_google_2fapi_2ffield_5fbehavior_2eproto = nullptr;
const ::uint32_t TableStruct_google_2fapi_2ffield_5fbehavior_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;
const char descriptor_table_protodef_google_2fapi_2ffield_5fbehavior_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
    "\n\037google/api/field_behavior.proto\022\ngoogl"
    "e.api\032 google/protobuf/descriptor.proto*"
    "\266\001\n\rFieldBehavior\022\036\n\032FIELD_BEHAVIOR_UNSP"
    "ECIFIED\020\000\022\014\n\010OPTIONAL\020\001\022\014\n\010REQUIRED\020\002\022\017\n"
    "\013OUTPUT_ONLY\020\003\022\016\n\nINPUT_ONLY\020\004\022\r\n\tIMMUTA"
    "BLE\020\005\022\022\n\016UNORDERED_LIST\020\006\022\025\n\021NON_EMPTY_D"
    "EFAULT\020\007\022\016\n\nIDENTIFIER\020\010:d\n\016field_behavi"
    "or\022\035.google.protobuf.FieldOptions\030\234\010 \003(\016"
    "2\031.google.api.FieldBehaviorB\002\020\000R\rfieldBe"
    "haviorB\260\001\n\016com.google.apiB\022FieldBehavior"
    "ProtoP\001ZAgoogle.golang.org/genproto/goog"
    "leapis/api/annotations;annotations\242\002\003GAX"
    "\252\002\nGoogle.Api\312\002\nGoogle\\Api\342\002\026Google\\Api\\"
    "GPBMetadata\352\002\013Google::Apib\006proto3"
};
static const ::_pbi::DescriptorTable* const descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto_deps[1] =
    {
        &::descriptor_table_google_2fprotobuf_2fdescriptor_2eproto,
};
static ::absl::once_flag descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto = {
    false,
    false,
    553,
    descriptor_table_protodef_google_2fapi_2ffield_5fbehavior_2eproto,
    "google/api/field_behavior.proto",
    &descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto_once,
    descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto_deps,
    1,
    0,
    schemas,
    file_default_instances,
    TableStruct_google_2fapi_2ffield_5fbehavior_2eproto::offsets,
    nullptr,
    file_level_enum_descriptors_google_2fapi_2ffield_5fbehavior_2eproto,
    file_level_service_descriptors_google_2fapi_2ffield_5fbehavior_2eproto,
};

// This function exists to be marked as weak.
// It can significantly speed up compilation by breaking up LLVM's SCC
// in the .pb.cc translation units. Large translation units see a
// reduction of more than 35% of walltime for optimized builds. Without
// the weak attribute all the messages in the file, including all the
// vtables and everything they use become part of the same SCC through
// a cycle like:
// GetMetadata -> descriptor table -> default instances ->
//   vtables -> GetMetadata
// By adding a weak function here we break the connection from the
// individual vtables back into the descriptor table.
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto_getter() {
  return &descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto;
}
// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_google_2fapi_2ffield_5fbehavior_2eproto(&descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto);
namespace google {
namespace api {
const ::google::protobuf::EnumDescriptor* FieldBehavior_descriptor() {
  ::google::protobuf::internal::AssignDescriptors(&descriptor_table_google_2fapi_2ffield_5fbehavior_2eproto);
  return file_level_enum_descriptors_google_2fapi_2ffield_5fbehavior_2eproto[0];
}
PROTOBUF_CONSTINIT const uint32_t FieldBehavior_internal_data_[] = {
    589824u, 0u, };
bool FieldBehavior_IsValid(int value) {
  return 0 <= value && value <= 8;
}
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::FieldOptions,
    ::google::protobuf::internal::RepeatedEnumTypeTraits< ::google::api::FieldBehavior, ::google::api::FieldBehavior_IsValid>, 14, false>
  field_behavior(kFieldBehaviorFieldNumber, static_cast< ::google::api::FieldBehavior >(0), nullptr);
// @@protoc_insertion_point(namespace_scope)
}  // namespace api
}  // namespace google
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"
