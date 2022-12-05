// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/api/expr/v1alpha1/explain.proto

#include "google/api/expr/v1alpha1/explain.pb.h"

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
namespace expr {
namespace v1alpha1 {
PROTOBUF_CONSTEXPR Explain_ExprStep::Explain_ExprStep(
    ::_pbi::ConstantInitialized)
  : id_(int64_t{0})
  , value_index_(0){}
struct Explain_ExprStepDefaultTypeInternal {
  PROTOBUF_CONSTEXPR Explain_ExprStepDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~Explain_ExprStepDefaultTypeInternal() {}
  union {
    Explain_ExprStep _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 Explain_ExprStepDefaultTypeInternal _Explain_ExprStep_default_instance_;
PROTOBUF_CONSTEXPR Explain::Explain(
    ::_pbi::ConstantInitialized)
  : values_()
  , expr_steps_(){}
struct ExplainDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ExplainDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~ExplainDefaultTypeInternal() {}
  union {
    Explain _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ExplainDefaultTypeInternal _Explain_default_instance_;
}  // namespace v1alpha1
}  // namespace expr
}  // namespace api
}  // namespace google
static ::_pb::Metadata file_level_metadata_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto[2];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto = nullptr;

const uint32_t TableStruct_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::google::api::expr::v1alpha1::Explain_ExprStep, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::google::api::expr::v1alpha1::Explain_ExprStep, id_),
  PROTOBUF_FIELD_OFFSET(::google::api::expr::v1alpha1::Explain_ExprStep, value_index_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::google::api::expr::v1alpha1::Explain, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::google::api::expr::v1alpha1::Explain, values_),
  PROTOBUF_FIELD_OFFSET(::google::api::expr::v1alpha1::Explain, expr_steps_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::google::api::expr::v1alpha1::Explain_ExprStep)},
  { 8, -1, -1, sizeof(::google::api::expr::v1alpha1::Explain)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::google::api::expr::v1alpha1::_Explain_ExprStep_default_instance_._instance,
  &::google::api::expr::v1alpha1::_Explain_default_instance_._instance,
};

const char descriptor_table_protodef_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n&google/api/expr/v1alpha1/explain.proto"
  "\022\030google.api.expr.v1alpha1\032$google/api/e"
  "xpr/v1alpha1/value.proto\"\316\001\n\007Explain\0227\n\006"
  "values\030\001 \003(\0132\037.google.api.expr.v1alpha1."
  "ValueR\006values\022I\n\nexpr_steps\030\002 \003(\0132*.goog"
  "le.api.expr.v1alpha1.Explain.ExprStepR\te"
  "xprSteps\032;\n\010ExprStep\022\016\n\002id\030\001 \001(\003R\002id\022\037\n\013"
  "value_index\030\002 \001(\005R\nvalueIndex:\002\030\001Bo\n\034com"
  ".google.api.expr.v1alpha1B\014ExplainProtoP"
  "\001Z<google.golang.org/genproto/googleapis"
  "/api/expr/v1alpha1;expr\370\001\001b\006proto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto_deps[1] = {
  &::descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fvalue_2eproto,
};
static ::_pbi::once_flag descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto = {
    false, false, 434, descriptor_table_protodef_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto,
    "google/api/expr/v1alpha1/explain.proto",
    &descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto_once, descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto_deps, 1, 2,
    schemas, file_default_instances, TableStruct_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto::offsets,
    file_level_metadata_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto, file_level_enum_descriptors_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto,
    file_level_service_descriptors_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto_getter() {
  return &descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto(&descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto);
namespace google {
namespace api {
namespace expr {
namespace v1alpha1 {

// ===================================================================

class Explain_ExprStep::_Internal {
 public:
};

Explain_ExprStep::Explain_ExprStep(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  // @@protoc_insertion_point(arena_constructor:google.api.expr.v1alpha1.Explain.ExprStep)
}
Explain_ExprStep::Explain_ExprStep(const Explain_ExprStep& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&id_, &from.id_,
    static_cast<size_t>(reinterpret_cast<char*>(&value_index_) -
    reinterpret_cast<char*>(&id_)) + sizeof(value_index_));
  // @@protoc_insertion_point(copy_constructor:google.api.expr.v1alpha1.Explain.ExprStep)
}

inline void Explain_ExprStep::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&id_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&value_index_) -
    reinterpret_cast<char*>(&id_)) + sizeof(value_index_));
}

Explain_ExprStep::~Explain_ExprStep() {
  // @@protoc_insertion_point(destructor:google.api.expr.v1alpha1.Explain.ExprStep)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Explain_ExprStep::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void Explain_ExprStep::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Explain_ExprStep::Clear() {
// @@protoc_insertion_point(message_clear_start:google.api.expr.v1alpha1.Explain.ExprStep)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&value_index_) -
      reinterpret_cast<char*>(&id_)) + sizeof(value_index_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Explain_ExprStep::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int64 id = 1 [json_name = "id"];
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int32 value_index = 2 [json_name = "valueIndex"];
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          value_index_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Explain_ExprStep::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:google.api.expr.v1alpha1.Explain.ExprStep)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 id = 1 [json_name = "id"];
  if (this->_internal_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(1, this->_internal_id(), target);
  }

  // int32 value_index = 2 [json_name = "valueIndex"];
  if (this->_internal_value_index() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(2, this->_internal_value_index(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:google.api.expr.v1alpha1.Explain.ExprStep)
  return target;
}

size_t Explain_ExprStep::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:google.api.expr.v1alpha1.Explain.ExprStep)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int64 id = 1 [json_name = "id"];
  if (this->_internal_id() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_id());
  }

  // int32 value_index = 2 [json_name = "valueIndex"];
  if (this->_internal_value_index() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_value_index());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Explain_ExprStep::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    Explain_ExprStep::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Explain_ExprStep::GetClassData() const { return &_class_data_; }

void Explain_ExprStep::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<Explain_ExprStep *>(to)->MergeFrom(
      static_cast<const Explain_ExprStep &>(from));
}


void Explain_ExprStep::MergeFrom(const Explain_ExprStep& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:google.api.expr.v1alpha1.Explain.ExprStep)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_id() != 0) {
    _internal_set_id(from._internal_id());
  }
  if (from._internal_value_index() != 0) {
    _internal_set_value_index(from._internal_value_index());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Explain_ExprStep::CopyFrom(const Explain_ExprStep& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:google.api.expr.v1alpha1.Explain.ExprStep)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Explain_ExprStep::IsInitialized() const {
  return true;
}

void Explain_ExprStep::InternalSwap(Explain_ExprStep* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Explain_ExprStep, value_index_)
      + sizeof(Explain_ExprStep::value_index_)
      - PROTOBUF_FIELD_OFFSET(Explain_ExprStep, id_)>(
          reinterpret_cast<char*>(&id_),
          reinterpret_cast<char*>(&other->id_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Explain_ExprStep::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto_getter, &descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto_once,
      file_level_metadata_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto[0]);
}

// ===================================================================

class Explain::_Internal {
 public:
};

void Explain::clear_values() {
  values_.Clear();
}
Explain::Explain(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned),
  values_(arena),
  expr_steps_(arena) {
  SharedCtor();
  // @@protoc_insertion_point(arena_constructor:google.api.expr.v1alpha1.Explain)
}
Explain::Explain(const Explain& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      values_(from.values_),
      expr_steps_(from.expr_steps_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:google.api.expr.v1alpha1.Explain)
}

inline void Explain::SharedCtor() {
}

Explain::~Explain() {
  // @@protoc_insertion_point(destructor:google.api.expr.v1alpha1.Explain)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Explain::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void Explain::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Explain::Clear() {
// @@protoc_insertion_point(message_clear_start:google.api.expr.v1alpha1.Explain)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  values_.Clear();
  expr_steps_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Explain::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // repeated .google.api.expr.v1alpha1.Value values = 1 [json_name = "values"];
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_values(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
        } else
          goto handle_unusual;
        continue;
      // repeated .google.api.expr.v1alpha1.Explain.ExprStep expr_steps = 2 [json_name = "exprSteps"];
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_expr_steps(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<18>(ptr));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Explain::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:google.api.expr.v1alpha1.Explain)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .google.api.expr.v1alpha1.Value values = 1 [json_name = "values"];
  for (unsigned i = 0,
      n = static_cast<unsigned>(this->_internal_values_size()); i < n; i++) {
    const auto& repfield = this->_internal_values(i);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
        InternalWriteMessage(1, repfield, repfield.GetCachedSize(), target, stream);
  }

  // repeated .google.api.expr.v1alpha1.Explain.ExprStep expr_steps = 2 [json_name = "exprSteps"];
  for (unsigned i = 0,
      n = static_cast<unsigned>(this->_internal_expr_steps_size()); i < n; i++) {
    const auto& repfield = this->_internal_expr_steps(i);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
        InternalWriteMessage(2, repfield, repfield.GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:google.api.expr.v1alpha1.Explain)
  return target;
}

size_t Explain::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:google.api.expr.v1alpha1.Explain)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .google.api.expr.v1alpha1.Value values = 1 [json_name = "values"];
  total_size += 1UL * this->_internal_values_size();
  for (const auto& msg : this->values_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // repeated .google.api.expr.v1alpha1.Explain.ExprStep expr_steps = 2 [json_name = "exprSteps"];
  total_size += 1UL * this->_internal_expr_steps_size();
  for (const auto& msg : this->expr_steps_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Explain::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    Explain::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Explain::GetClassData() const { return &_class_data_; }

void Explain::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<Explain *>(to)->MergeFrom(
      static_cast<const Explain &>(from));
}


void Explain::MergeFrom(const Explain& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:google.api.expr.v1alpha1.Explain)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  values_.MergeFrom(from.values_);
  expr_steps_.MergeFrom(from.expr_steps_);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Explain::CopyFrom(const Explain& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:google.api.expr.v1alpha1.Explain)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Explain::IsInitialized() const {
  return true;
}

void Explain::InternalSwap(Explain* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  values_.InternalSwap(&other->values_);
  expr_steps_.InternalSwap(&other->expr_steps_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Explain::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto_getter, &descriptor_table_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto_once,
      file_level_metadata_google_2fapi_2fexpr_2fv1alpha1_2fexplain_2eproto[1]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace v1alpha1
}  // namespace expr
}  // namespace api
}  // namespace google
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::google::api::expr::v1alpha1::Explain_ExprStep*
Arena::CreateMaybeMessage< ::google::api::expr::v1alpha1::Explain_ExprStep >(Arena* arena) {
  return Arena::CreateMessageInternal< ::google::api::expr::v1alpha1::Explain_ExprStep >(arena);
}
template<> PROTOBUF_NOINLINE ::google::api::expr::v1alpha1::Explain*
Arena::CreateMaybeMessage< ::google::api::expr::v1alpha1::Explain >(Arena* arena) {
  return Arena::CreateMessageInternal< ::google::api::expr::v1alpha1::Explain >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
