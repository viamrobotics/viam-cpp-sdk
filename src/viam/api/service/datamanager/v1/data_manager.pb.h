// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: service/datamanager/v1/data_manager.proto
// Protobuf C++ Version: 4.25.1

#ifndef GOOGLE_PROTOBUF_INCLUDED_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_2epb_2eh
#define GOOGLE_PROTOBUF_INCLUDED_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_2epb_2eh

#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "google/protobuf/port_def.inc"
#if PROTOBUF_VERSION < 4025000
#error "This file was generated by a newer version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please update"
#error "your headers."
#endif  // PROTOBUF_VERSION

#if 4025001 < PROTOBUF_MIN_PROTOC_VERSION
#error "This file was generated by an older version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please"
#error "regenerate this file with a newer version of protoc."
#endif  // PROTOBUF_MIN_PROTOC_VERSION
#include "google/protobuf/port_undef.inc"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/arenastring.h"
#include "google/protobuf/generated_message_bases.h"
#include "google/protobuf/generated_message_tctable_decl.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/metadata_lite.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/message.h"
#include "google/protobuf/repeated_field.h"  // IWYU pragma: export
#include "google/protobuf/extension_set.h"  // IWYU pragma: export
#include "google/protobuf/unknown_field_set.h"
#include "common/v1/common.pb.h"
#include "google/api/annotations.pb.h"
#include "google/protobuf/struct.pb.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"

#define PROTOBUF_INTERNAL_EXPORT_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto

namespace google {
namespace protobuf {
namespace internal {
class AnyMetadata;
}  // namespace internal
}  // namespace protobuf
}  // namespace google

// Internal implementation detail -- do not use these members.
struct TableStruct_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto {
  static const ::uint32_t offsets[];
};
extern const ::google::protobuf::internal::DescriptorTable
    descriptor_table_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto;
namespace viam {
namespace service {
namespace datamanager {
namespace v1 {
class SyncRequest;
struct SyncRequestDefaultTypeInternal;
extern SyncRequestDefaultTypeInternal _SyncRequest_default_instance_;
class SyncResponse;
struct SyncResponseDefaultTypeInternal;
extern SyncResponseDefaultTypeInternal _SyncResponse_default_instance_;
}  // namespace v1
}  // namespace datamanager
}  // namespace service
}  // namespace viam
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google

namespace viam {
namespace service {
namespace datamanager {
namespace v1 {

// ===================================================================


// -------------------------------------------------------------------

class SyncResponse final :
    public ::google::protobuf::internal::ZeroFieldsBase /* @@protoc_insertion_point(class_definition:viam.service.datamanager.v1.SyncResponse) */ {
 public:
  inline SyncResponse() : SyncResponse(nullptr) {}
  template<typename = void>
  explicit PROTOBUF_CONSTEXPR SyncResponse(::google::protobuf::internal::ConstantInitialized);

  inline SyncResponse(const SyncResponse& from)
      : SyncResponse(nullptr, from) {}
  SyncResponse(SyncResponse&& from) noexcept
    : SyncResponse() {
    *this = ::std::move(from);
  }

  inline SyncResponse& operator=(const SyncResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline SyncResponse& operator=(SyncResponse&& from) noexcept {
    if (this == &from) return *this;
    if (GetArena() == from.GetArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance);
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields()
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.mutable_unknown_fields<::google::protobuf::UnknownFieldSet>();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::google::protobuf::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::google::protobuf::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const SyncResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const SyncResponse* internal_default_instance() {
    return reinterpret_cast<const SyncResponse*>(
               &_SyncResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(SyncResponse& a, SyncResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(SyncResponse* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() != nullptr &&
        GetArena() == other->GetArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() == other->GetArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::google::protobuf::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SyncResponse* other) {
    if (other == this) return;
    ABSL_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  SyncResponse* New(::google::protobuf::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<SyncResponse>(arena);
  }
  using ::google::protobuf::internal::ZeroFieldsBase::CopyFrom;
  inline void CopyFrom(const SyncResponse& from) {
    ::google::protobuf::internal::ZeroFieldsBase::CopyImpl(*this, from);
  }
  using ::google::protobuf::internal::ZeroFieldsBase::MergeFrom;
  void MergeFrom(const SyncResponse& from) {
    ::google::protobuf::internal::ZeroFieldsBase::MergeImpl(*this, from);
  }
  public:

  private:
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::absl::string_view FullMessageName() {
    return "viam.service.datamanager.v1.SyncResponse";
  }
  protected:
  explicit SyncResponse(::google::protobuf::Arena* arena);
  SyncResponse(::google::protobuf::Arena* arena, const SyncResponse& from);
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:viam.service.datamanager.v1.SyncResponse)
 private:
  class _Internal;

  friend class ::google::protobuf::MessageLite;
  friend class ::google::protobuf::Arena;
  template <typename T>
  friend class ::google::protobuf::Arena::InternalHelper;
  using InternalArenaConstructable_ = void;
  using DestructorSkippable_ = void;
  struct Impl_ {

        inline explicit constexpr Impl_(
            ::google::protobuf::internal::ConstantInitialized) noexcept;
        inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                              ::google::protobuf::Arena* arena);
        inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                              ::google::protobuf::Arena* arena, const Impl_& from);
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  friend struct ::TableStruct_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto;
};// -------------------------------------------------------------------

class SyncRequest final :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:viam.service.datamanager.v1.SyncRequest) */ {
 public:
  inline SyncRequest() : SyncRequest(nullptr) {}
  ~SyncRequest() override;
  template<typename = void>
  explicit PROTOBUF_CONSTEXPR SyncRequest(::google::protobuf::internal::ConstantInitialized);

  inline SyncRequest(const SyncRequest& from)
      : SyncRequest(nullptr, from) {}
  SyncRequest(SyncRequest&& from) noexcept
    : SyncRequest() {
    *this = ::std::move(from);
  }

  inline SyncRequest& operator=(const SyncRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline SyncRequest& operator=(SyncRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetArena() == from.GetArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance);
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields()
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.mutable_unknown_fields<::google::protobuf::UnknownFieldSet>();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::google::protobuf::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::google::protobuf::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const SyncRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const SyncRequest* internal_default_instance() {
    return reinterpret_cast<const SyncRequest*>(
               &_SyncRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SyncRequest& a, SyncRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(SyncRequest* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() != nullptr &&
        GetArena() == other->GetArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() == other->GetArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::google::protobuf::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SyncRequest* other) {
    if (other == this) return;
    ABSL_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  SyncRequest* New(::google::protobuf::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<SyncRequest>(arena);
  }
  using ::google::protobuf::Message::CopyFrom;
  void CopyFrom(const SyncRequest& from);
  using ::google::protobuf::Message::MergeFrom;
  void MergeFrom( const SyncRequest& from) {
    SyncRequest::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::google::protobuf::Message& to_msg, const ::google::protobuf::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  ::size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::google::protobuf::internal::ParseContext* ctx) final;
  ::uint8_t* _InternalSerialize(
      ::uint8_t* target, ::google::protobuf::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const { return _impl_._cached_size_.Get(); }

  private:
  ::google::protobuf::internal::CachedSize* AccessCachedSize() const final;
  void SharedCtor(::google::protobuf::Arena* arena);
  void SharedDtor();
  void InternalSwap(SyncRequest* other);

  private:
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::absl::string_view FullMessageName() {
    return "viam.service.datamanager.v1.SyncRequest";
  }
  protected:
  explicit SyncRequest(::google::protobuf::Arena* arena);
  SyncRequest(::google::protobuf::Arena* arena, const SyncRequest& from);
  public:

  static const ClassData _class_data_;
  const ::google::protobuf::Message::ClassData*GetClassData() const final;

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 1,
    kExtraFieldNumber = 99,
  };
  // string name = 1 [json_name = "name"];
  void clear_name() ;
  const std::string& name() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_name(Arg_&& arg, Args_... args);
  std::string* mutable_name();
  PROTOBUF_NODISCARD std::string* release_name();
  void set_allocated_name(std::string* value);

  private:
  const std::string& _internal_name() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_name(
      const std::string& value);
  std::string* _internal_mutable_name();

  public:
  // .google.protobuf.Struct extra = 99 [json_name = "extra"];
  bool has_extra() const;
  void clear_extra() ;
  const ::google::protobuf::Struct& extra() const;
  PROTOBUF_NODISCARD ::google::protobuf::Struct* release_extra();
  ::google::protobuf::Struct* mutable_extra();
  void set_allocated_extra(::google::protobuf::Struct* value);
  void unsafe_arena_set_allocated_extra(::google::protobuf::Struct* value);
  ::google::protobuf::Struct* unsafe_arena_release_extra();

  private:
  const ::google::protobuf::Struct& _internal_extra() const;
  ::google::protobuf::Struct* _internal_mutable_extra();

  public:
  // @@protoc_insertion_point(class_scope:viam.service.datamanager.v1.SyncRequest)
 private:
  class _Internal;

  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<
      2, 2, 1,
      52, 7>
      _table_;
  friend class ::google::protobuf::MessageLite;
  friend class ::google::protobuf::Arena;
  template <typename T>
  friend class ::google::protobuf::Arena::InternalHelper;
  using InternalArenaConstructable_ = void;
  using DestructorSkippable_ = void;
  struct Impl_ {

        inline explicit constexpr Impl_(
            ::google::protobuf::internal::ConstantInitialized) noexcept;
        inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                              ::google::protobuf::Arena* arena);
        inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                              ::google::protobuf::Arena* arena, const Impl_& from);
    ::google::protobuf::internal::HasBits<1> _has_bits_;
    mutable ::google::protobuf::internal::CachedSize _cached_size_;
    ::google::protobuf::internal::ArenaStringPtr name_;
    ::google::protobuf::Struct* extra_;
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto;
};

// ===================================================================




// ===================================================================


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// SyncRequest

// string name = 1 [json_name = "name"];
inline void SyncRequest::clear_name() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.name_.ClearToEmpty();
}
inline const std::string& SyncRequest::name() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:viam.service.datamanager.v1.SyncRequest.name)
  return _internal_name();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void SyncRequest::set_name(Arg_&& arg,
                                                     Args_... args) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.name_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:viam.service.datamanager.v1.SyncRequest.name)
}
inline std::string* SyncRequest::mutable_name() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_name();
  // @@protoc_insertion_point(field_mutable:viam.service.datamanager.v1.SyncRequest.name)
  return _s;
}
inline const std::string& SyncRequest::_internal_name() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.name_.Get();
}
inline void SyncRequest::_internal_set_name(const std::string& value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.name_.Set(value, GetArena());
}
inline std::string* SyncRequest::_internal_mutable_name() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  return _impl_.name_.Mutable( GetArena());
}
inline std::string* SyncRequest::release_name() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  // @@protoc_insertion_point(field_release:viam.service.datamanager.v1.SyncRequest.name)
  return _impl_.name_.Release();
}
inline void SyncRequest::set_allocated_name(std::string* value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.name_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.name_.IsDefault()) {
          _impl_.name_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:viam.service.datamanager.v1.SyncRequest.name)
}

// .google.protobuf.Struct extra = 99 [json_name = "extra"];
inline bool SyncRequest::has_extra() const {
  bool value = (_impl_._has_bits_[0] & 0x00000001u) != 0;
  PROTOBUF_ASSUME(!value || _impl_.extra_ != nullptr);
  return value;
}
inline const ::google::protobuf::Struct& SyncRequest::_internal_extra() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  const ::google::protobuf::Struct* p = _impl_.extra_;
  return p != nullptr ? *p : reinterpret_cast<const ::google::protobuf::Struct&>(::google::protobuf::_Struct_default_instance_);
}
inline const ::google::protobuf::Struct& SyncRequest::extra() const ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:viam.service.datamanager.v1.SyncRequest.extra)
  return _internal_extra();
}
inline void SyncRequest::unsafe_arena_set_allocated_extra(::google::protobuf::Struct* value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::google::protobuf::MessageLite*>(_impl_.extra_);
  }
  _impl_.extra_ = reinterpret_cast<::google::protobuf::Struct*>(value);
  if (value != nullptr) {
    _impl_._has_bits_[0] |= 0x00000001u;
  } else {
    _impl_._has_bits_[0] &= ~0x00000001u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:viam.service.datamanager.v1.SyncRequest.extra)
}
inline ::google::protobuf::Struct* SyncRequest::release_extra() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);

  _impl_._has_bits_[0] &= ~0x00000001u;
  ::google::protobuf::Struct* released = _impl_.extra_;
  _impl_.extra_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old = reinterpret_cast<::google::protobuf::MessageLite*>(released);
  released = ::google::protobuf::internal::DuplicateIfNonNull(released);
  if (GetArena() == nullptr) {
    delete old;
  }
#else   // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArena() != nullptr) {
    released = ::google::protobuf::internal::DuplicateIfNonNull(released);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return released;
}
inline ::google::protobuf::Struct* SyncRequest::unsafe_arena_release_extra() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  // @@protoc_insertion_point(field_release:viam.service.datamanager.v1.SyncRequest.extra)

  _impl_._has_bits_[0] &= ~0x00000001u;
  ::google::protobuf::Struct* temp = _impl_.extra_;
  _impl_.extra_ = nullptr;
  return temp;
}
inline ::google::protobuf::Struct* SyncRequest::_internal_mutable_extra() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_._has_bits_[0] |= 0x00000001u;
  if (_impl_.extra_ == nullptr) {
    auto* p = CreateMaybeMessage<::google::protobuf::Struct>(GetArena());
    _impl_.extra_ = reinterpret_cast<::google::protobuf::Struct*>(p);
  }
  return _impl_.extra_;
}
inline ::google::protobuf::Struct* SyncRequest::mutable_extra() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  ::google::protobuf::Struct* _msg = _internal_mutable_extra();
  // @@protoc_insertion_point(field_mutable:viam.service.datamanager.v1.SyncRequest.extra)
  return _msg;
}
inline void SyncRequest::set_allocated_extra(::google::protobuf::Struct* value) {
  ::google::protobuf::Arena* message_arena = GetArena();
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  if (message_arena == nullptr) {
    delete reinterpret_cast<::google::protobuf::MessageLite*>(_impl_.extra_);
  }

  if (value != nullptr) {
    ::google::protobuf::Arena* submessage_arena = reinterpret_cast<::google::protobuf::MessageLite*>(value)->GetArena();
    if (message_arena != submessage_arena) {
      value = ::google::protobuf::internal::GetOwnedMessage(message_arena, value, submessage_arena);
    }
    _impl_._has_bits_[0] |= 0x00000001u;
  } else {
    _impl_._has_bits_[0] &= ~0x00000001u;
  }

  _impl_.extra_ = reinterpret_cast<::google::protobuf::Struct*>(value);
  // @@protoc_insertion_point(field_set_allocated:viam.service.datamanager.v1.SyncRequest.extra)
}

// -------------------------------------------------------------------

// SyncResponse

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)
}  // namespace v1
}  // namespace datamanager
}  // namespace service
}  // namespace viam


// @@protoc_insertion_point(global_scope)

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_INCLUDED_service_2fdatamanager_2fv1_2fdata_5fmanager_2eproto_2epb_2eh