// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: service/datamanager/v1/data_manager.proto

#include "service/datamanager/v1/data_manager.pb.h"

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
namespace viam {
namespace service {
namespace datamanager {
namespace v1 {
      template <typename>
PROTOBUF_CONSTEXPR SyncResponse::SyncResponse(::_pbi::ConstantInitialized) {}
struct SyncResponseDefaultTypeInternal {
  PROTOBUF_CONSTEXPR SyncResponseDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~SyncResponseDefaultTypeInternal() {}
  union {
    SyncResponse _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 SyncResponseDefaultTypeInternal _SyncResponse_default_instance_;

inline constexpr SyncRequest::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        name_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        extra_{nullptr} {}

template <typename>
PROTOBUF_CONSTEXPR SyncRequest::SyncRequest(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct SyncRequestDefaultTypeInternal {
  PROTOBUF_CONSTEXPR SyncRequestDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~SyncRequestDefaultTypeInternal() {}
  union {
    SyncRequest _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 SyncRequestDefaultTypeInternal _SyncRequest_default_instance_;
}  // namespace v1
}  // namespace datamanager
}  // namespace service
}  // namespace viam
static ::_pb::Metadata file_level_metadata_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto[2];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto = nullptr;
const ::uint32_t TableStruct_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(
    protodesc_cold) = {
    PROTOBUF_FIELD_OFFSET(::viam::service::datamanager::v1::SyncRequest, _impl_._has_bits_),
    PROTOBUF_FIELD_OFFSET(::viam::service::datamanager::v1::SyncRequest, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::viam::service::datamanager::v1::SyncRequest, _impl_.name_),
    PROTOBUF_FIELD_OFFSET(::viam::service::datamanager::v1::SyncRequest, _impl_.extra_),
    ~0u,
    0,
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::viam::service::datamanager::v1::SyncResponse, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
};

static const ::_pbi::MigrationSchema
    schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
        {0, 10, -1, sizeof(::viam::service::datamanager::v1::SyncRequest)},
        {12, -1, -1, sizeof(::viam::service::datamanager::v1::SyncResponse)},
};

static const ::_pb::Message* const file_default_instances[] = {
    &::viam::service::datamanager::v1::_SyncRequest_default_instance_._instance,
    &::viam::service::datamanager::v1::_SyncResponse_default_instance_._instance,
};
const char descriptor_table_protodef_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
    "\n)service/datamanager/v1/data_manager.pr"
    "oto\022\033viam.service.datamanager.v1\032\026common"
    "/v1/common.proto\032\034google/api/annotations"
    ".proto\032\034google/protobuf/struct.proto\"P\n\013"
    "SyncRequest\022\022\n\004name\030\001 \001(\tR\004name\022-\n\005extra"
    "\030c \001(\0132\027.google.protobuf.StructR\005extra\"\016"
    "\n\014SyncResponse2\273\002\n\022DataManagerService\022\225\001"
    "\n\004Sync\022(.viam.service.datamanager.v1.Syn"
    "cRequest\032).viam.service.datamanager.v1.S"
    "yncResponse\"8\202\323\344\223\0022\"0/viam/api/v1/servic"
    "e/datamanager/{name}/datasync\022\214\001\n\tDoComm"
    "and\022 .viam.common.v1.DoCommandRequest\032!."
    "viam.common.v1.DoCommandResponse\":\202\323\344\223\0024"
    "\"2/viam/api/v1/service/datamanager/{name"
    "}/do_commandB\352\001\n\037com.viam.service.datama"
    "nager.v1B\020DataManagerProtoP\001Z&go.viam.co"
    "m/api/service/datamanager/v1\242\002\003VSD\252\002\033Via"
    "m.Service.Datamanager.V1\312\002\033Viam\\Service\\"
    "Datamanager\\V1\342\002\'Viam\\Service\\Datamanage"
    "r\\V1\\GPBMetadata\352\002\036Viam::Service::Datama"
    "nager::V1b\006proto3"
};
static const ::_pbi::DescriptorTable* const descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_deps[3] =
    {
        &::descriptor_table_common_2fv1_2fcommon_2eproto,
        &::descriptor_table_google_2fapi_2fannotations_2eproto,
        &::descriptor_table_google_2fprotobuf_2fstruct_2eproto,
};
static ::absl::once_flag descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto = {
    false,
    false,
    817,
    descriptor_table_protodef_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto,
    "service/datamanager/v1/data_manager.proto",
    &descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_once,
    descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_deps,
    3,
    2,
    schemas,
    file_default_instances,
    TableStruct_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto::offsets,
    file_level_metadata_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto,
    file_level_enum_descriptors_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto,
    file_level_service_descriptors_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto,
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
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_getter() {
  return &descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto;
}
// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto(&descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto);
namespace viam {
namespace service {
namespace datamanager {
namespace v1 {
// ===================================================================

class SyncRequest::_Internal {
 public:
  using HasBits = decltype(std::declval<SyncRequest>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
    8 * PROTOBUF_FIELD_OFFSET(SyncRequest, _impl_._has_bits_);
  static const ::google::protobuf::Struct& extra(const SyncRequest* msg);
  static void set_has_extra(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

const ::google::protobuf::Struct& SyncRequest::_Internal::extra(const SyncRequest* msg) {
  return *msg->_impl_.extra_;
}
void SyncRequest::clear_extra() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  if (_impl_.extra_ != nullptr) _impl_.extra_->Clear();
  _impl_._has_bits_[0] &= ~0x00000001u;
}
SyncRequest::SyncRequest(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:viam.service.datamanager.v1.SyncRequest)
}
inline PROTOBUF_NDEBUG_INLINE SyncRequest::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : _has_bits_{from._has_bits_},
        _cached_size_{0},
        name_(arena, from.name_) {}

SyncRequest::SyncRequest(
    ::google::protobuf::Arena* arena,
    const SyncRequest& from)
    : ::google::protobuf::Message(arena) {
  SyncRequest* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);
  ::uint32_t cached_has_bits = _impl_._has_bits_[0];
  _impl_.extra_ = (cached_has_bits & 0x00000001u)
                ? CreateMaybeMessage<::google::protobuf::Struct>(arena, *from._impl_.extra_)
                : nullptr;

  // @@protoc_insertion_point(copy_constructor:viam.service.datamanager.v1.SyncRequest)
}
inline PROTOBUF_NDEBUG_INLINE SyncRequest::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0},
        name_(arena) {}

inline void SyncRequest::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  _impl_.extra_ = {};
}
SyncRequest::~SyncRequest() {
  // @@protoc_insertion_point(destructor:viam.service.datamanager.v1.SyncRequest)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void SyncRequest::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.name_.Destroy();
  delete _impl_.extra_;
  _impl_.~Impl_();
}

PROTOBUF_NOINLINE void SyncRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:viam.service.datamanager.v1.SyncRequest)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.name_.ClearToEmpty();
  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    ABSL_DCHECK(_impl_.extra_ != nullptr);
    _impl_.extra_->Clear();
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* SyncRequest::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<2, 2, 1, 52, 7> SyncRequest::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(SyncRequest, _impl_._has_bits_),
    0, // no _extensions_
    99, 24,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    2,  // num_field_entries
    1,  // num_aux_entries
    offsetof(decltype(_table_), aux_entries),
    &_SyncRequest_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
  }, {{
    {::_pbi::TcParser::MiniParse, {}},
    // string name = 1 [json_name = "name"];
    {::_pbi::TcParser::FastUS1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(SyncRequest, _impl_.name_)}},
    {::_pbi::TcParser::MiniParse, {}},
    // .google.protobuf.Struct extra = 99 [json_name = "extra"];
    {::_pbi::TcParser::FastMtS2,
     {1690, 0, 0, PROTOBUF_FIELD_OFFSET(SyncRequest, _impl_.extra_)}},
  }}, {{
    99, 0, 1,
    65534, 1,
    65535, 65535
  }}, {{
    // string name = 1 [json_name = "name"];
    {PROTOBUF_FIELD_OFFSET(SyncRequest, _impl_.name_), -1, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // .google.protobuf.Struct extra = 99 [json_name = "extra"];
    {PROTOBUF_FIELD_OFFSET(SyncRequest, _impl_.extra_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kMessage | ::_fl::kTvTable)},
  }}, {{
    {::_pbi::TcParser::GetTable<::google::protobuf::Struct>()},
  }}, {{
    "\47\4\0\0\0\0\0\0"
    "viam.service.datamanager.v1.SyncRequest"
    "name"
  }},
};

::uint8_t* SyncRequest::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:viam.service.datamanager.v1.SyncRequest)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // string name = 1 [json_name = "name"];
  if (!this->_internal_name().empty()) {
    const std::string& _s = this->_internal_name();
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "viam.service.datamanager.v1.SyncRequest.name");
    target = stream->WriteStringMaybeAliased(1, _s, target);
  }

  cached_has_bits = _impl_._has_bits_[0];
  // .google.protobuf.Struct extra = 99 [json_name = "extra"];
  if (cached_has_bits & 0x00000001u) {
    target = ::google::protobuf::internal::WireFormatLite::InternalWriteMessage(
        99, _Internal::extra(this),
        _Internal::extra(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:viam.service.datamanager.v1.SyncRequest)
  return target;
}

::size_t SyncRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:viam.service.datamanager.v1.SyncRequest)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string name = 1 [json_name = "name"];
  if (!this->_internal_name().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                    this->_internal_name());
  }

  // .google.protobuf.Struct extra = 99 [json_name = "extra"];
  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    total_size +=
        2 + ::google::protobuf::internal::WireFormatLite::MessageSize(*_impl_.extra_);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::google::protobuf::Message::ClassData SyncRequest::_class_data_ = {
    SyncRequest::MergeImpl,
    nullptr,  // OnDemandRegisterArenaDtor
};
const ::google::protobuf::Message::ClassData* SyncRequest::GetClassData() const {
  return &_class_data_;
}

void SyncRequest::MergeImpl(::google::protobuf::Message& to_msg, const ::google::protobuf::Message& from_msg) {
  auto* const _this = static_cast<SyncRequest*>(&to_msg);
  auto& from = static_cast<const SyncRequest&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:viam.service.datamanager.v1.SyncRequest)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_name().empty()) {
    _this->_internal_set_name(from._internal_name());
  }
  if ((from._impl_._has_bits_[0] & 0x00000001u) != 0) {
    _this->_internal_mutable_extra()->::google::protobuf::Struct::MergeFrom(
        from._internal_extra());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void SyncRequest::CopyFrom(const SyncRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:viam.service.datamanager.v1.SyncRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool SyncRequest::IsInitialized() const {
  return true;
}

::_pbi::CachedSize* SyncRequest::AccessCachedSize() const {
  return &_impl_._cached_size_;
}
void SyncRequest::InternalSwap(SyncRequest* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.name_, &other->_impl_.name_, arena);
  swap(_impl_.extra_, other->_impl_.extra_);
}

::google::protobuf::Metadata SyncRequest::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_getter, &descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_once,
      file_level_metadata_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto[0]);
}
// ===================================================================

class SyncResponse::_Internal {
 public:
};

SyncResponse::SyncResponse(::google::protobuf::Arena* arena)
    : ::google::protobuf::internal::ZeroFieldsBase(arena) {
  // @@protoc_insertion_point(arena_constructor:viam.service.datamanager.v1.SyncResponse)
}
SyncResponse::SyncResponse(
    ::google::protobuf::Arena* arena,
    const SyncResponse& from)
    : ::google::protobuf::internal::ZeroFieldsBase(arena) {
  SyncResponse* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);

  // @@protoc_insertion_point(copy_constructor:viam.service.datamanager.v1.SyncResponse)
}









::google::protobuf::Metadata SyncResponse::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_getter, &descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_once,
      file_level_metadata_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto[1]);
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace v1
}  // namespace datamanager
}  // namespace service
}  // namespace viam
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"