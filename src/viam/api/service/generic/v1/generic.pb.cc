// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: service/generic/v1/generic.proto

#include "service/generic/v1/generic.pb.h"

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

namespace viam {
namespace service {
namespace generic {
namespace v1 {
}  // namespace v1
}  // namespace generic
}  // namespace service
}  // namespace viam
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_service_2fgeneric_2fv1_2fgeneric_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_service_2fgeneric_2fv1_2fgeneric_2eproto = nullptr;
const uint32_t TableStruct_service_2fgeneric_2fv1_2fgeneric_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_service_2fgeneric_2fv1_2fgeneric_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n service/generic/v1/generic.proto\022\027viam"
  ".service.generic.v1\032\026common/v1/common.pr"
  "oto\032\034google/api/annotations.proto2\233\001\n\016Ge"
  "nericService\022\210\001\n\tDoCommand\022 .viam.common"
  ".v1.DoCommandRequest\032!.viam.common.v1.Do"
  "CommandResponse\"6\202\323\344\223\0020\"./viam/api/v1/se"
  "rvice/generic/{name}/do_commandBA\n\033com.v"
  "iam.service.generic.v1Z\"go.viam.com/api/"
  "service/generic/v1b\006proto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_service_2fgeneric_2fv1_2fgeneric_2eproto_deps[2] = {
  &::descriptor_table_common_2fv1_2fcommon_2eproto,
  &::descriptor_table_google_2fapi_2fannotations_2eproto,
};
static ::_pbi::once_flag descriptor_table_service_2fgeneric_2fv1_2fgeneric_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_service_2fgeneric_2fv1_2fgeneric_2eproto = {
    false, false, 346, descriptor_table_protodef_service_2fgeneric_2fv1_2fgeneric_2eproto,
    "service/generic/v1/generic.proto",
    &descriptor_table_service_2fgeneric_2fv1_2fgeneric_2eproto_once, descriptor_table_service_2fgeneric_2fv1_2fgeneric_2eproto_deps, 2, 0,
    schemas, file_default_instances, TableStruct_service_2fgeneric_2fv1_2fgeneric_2eproto::offsets,
    nullptr, file_level_enum_descriptors_service_2fgeneric_2fv1_2fgeneric_2eproto,
    file_level_service_descriptors_service_2fgeneric_2fv1_2fgeneric_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_service_2fgeneric_2fv1_2fgeneric_2eproto_getter() {
  return &descriptor_table_service_2fgeneric_2fv1_2fgeneric_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_service_2fgeneric_2fv1_2fgeneric_2eproto(&descriptor_table_service_2fgeneric_2fv1_2fgeneric_2eproto);
namespace viam {
namespace service {
namespace generic {
namespace v1 {

// @@protoc_insertion_point(namespace_scope)
}  // namespace v1
}  // namespace generic
}  // namespace service
}  // namespace viam
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
