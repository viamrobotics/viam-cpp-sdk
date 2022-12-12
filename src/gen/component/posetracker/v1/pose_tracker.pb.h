// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: component/posetracker/v1/pose_tracker.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_component_2fposetracker_2fv1_2fpose_5ftracker_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_component_2fposetracker_2fv1_2fpose_5ftracker_2eproto

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
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/map.h>  // IWYU pragma: export
#include <google/protobuf/map_entry.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/unknown_field_set.h>
#include "common/v1/common.pb.h"
#include "google/api/annotations.pb.h"
#include <google/protobuf/struct.pb.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_component_2fposetracker_2fv1_2fpose_5ftracker_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_component_2fposetracker_2fv1_2fpose_5ftracker_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_component_2fposetracker_2fv1_2fpose_5ftracker_2eproto;
namespace viam {
namespace component {
namespace posetracker {
namespace v1 {
class GetPosesRequest;
struct GetPosesRequestDefaultTypeInternal;
extern GetPosesRequestDefaultTypeInternal _GetPosesRequest_default_instance_;
class GetPosesResponse;
struct GetPosesResponseDefaultTypeInternal;
extern GetPosesResponseDefaultTypeInternal _GetPosesResponse_default_instance_;
class GetPosesResponse_BodyPosesEntry_DoNotUse;
struct GetPosesResponse_BodyPosesEntry_DoNotUseDefaultTypeInternal;
extern GetPosesResponse_BodyPosesEntry_DoNotUseDefaultTypeInternal _GetPosesResponse_BodyPosesEntry_DoNotUse_default_instance_;
}  // namespace v1
}  // namespace posetracker
}  // namespace component
}  // namespace viam
PROTOBUF_NAMESPACE_OPEN
template<> ::viam::component::posetracker::v1::GetPosesRequest* Arena::CreateMaybeMessage<::viam::component::posetracker::v1::GetPosesRequest>(Arena*);
template<> ::viam::component::posetracker::v1::GetPosesResponse* Arena::CreateMaybeMessage<::viam::component::posetracker::v1::GetPosesResponse>(Arena*);
template<> ::viam::component::posetracker::v1::GetPosesResponse_BodyPosesEntry_DoNotUse* Arena::CreateMaybeMessage<::viam::component::posetracker::v1::GetPosesResponse_BodyPosesEntry_DoNotUse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace viam {
namespace component {
namespace posetracker {
namespace v1 {

// ===================================================================

class GetPosesRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:viam.component.posetracker.v1.GetPosesRequest) */ {
 public:
  inline GetPosesRequest() : GetPosesRequest(nullptr) {}
  ~GetPosesRequest() override;
  explicit PROTOBUF_CONSTEXPR GetPosesRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  GetPosesRequest(const GetPosesRequest& from);
  GetPosesRequest(GetPosesRequest&& from) noexcept
    : GetPosesRequest() {
    *this = ::std::move(from);
  }

  inline GetPosesRequest& operator=(const GetPosesRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline GetPosesRequest& operator=(GetPosesRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const GetPosesRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const GetPosesRequest* internal_default_instance() {
    return reinterpret_cast<const GetPosesRequest*>(
               &_GetPosesRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(GetPosesRequest& a, GetPosesRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(GetPosesRequest* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(GetPosesRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  GetPosesRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<GetPosesRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const GetPosesRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const GetPosesRequest& from) {
    GetPosesRequest::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(GetPosesRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "viam.component.posetracker.v1.GetPosesRequest";
  }
  protected:
  explicit GetPosesRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kBodyNamesFieldNumber = 2,
    kNameFieldNumber = 1,
    kExtraFieldNumber = 99,
  };
  // repeated string body_names = 2 [json_name = "bodyNames"];
  int body_names_size() const;
  private:
  int _internal_body_names_size() const;
  public:
  void clear_body_names();
  const std::string& body_names(int index) const;
  std::string* mutable_body_names(int index);
  void set_body_names(int index, const std::string& value);
  void set_body_names(int index, std::string&& value);
  void set_body_names(int index, const char* value);
  void set_body_names(int index, const char* value, size_t size);
  std::string* add_body_names();
  void add_body_names(const std::string& value);
  void add_body_names(std::string&& value);
  void add_body_names(const char* value);
  void add_body_names(const char* value, size_t size);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>& body_names() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>* mutable_body_names();
  private:
  const std::string& _internal_body_names(int index) const;
  std::string* _internal_add_body_names();
  public:

  // string name = 1 [json_name = "name"];
  void clear_name();
  const std::string& name() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_name(ArgT0&& arg0, ArgT... args);
  std::string* mutable_name();
  PROTOBUF_NODISCARD std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // .google.protobuf.Struct extra = 99 [json_name = "extra"];
  bool has_extra() const;
  private:
  bool _internal_has_extra() const;
  public:
  void clear_extra();
  const ::PROTOBUF_NAMESPACE_ID::Struct& extra() const;
  PROTOBUF_NODISCARD ::PROTOBUF_NAMESPACE_ID::Struct* release_extra();
  ::PROTOBUF_NAMESPACE_ID::Struct* mutable_extra();
  void set_allocated_extra(::PROTOBUF_NAMESPACE_ID::Struct* extra);
  private:
  const ::PROTOBUF_NAMESPACE_ID::Struct& _internal_extra() const;
  ::PROTOBUF_NAMESPACE_ID::Struct* _internal_mutable_extra();
  public:
  void unsafe_arena_set_allocated_extra(
      ::PROTOBUF_NAMESPACE_ID::Struct* extra);
  ::PROTOBUF_NAMESPACE_ID::Struct* unsafe_arena_release_extra();

  // @@protoc_insertion_point(class_scope:viam.component.posetracker.v1.GetPosesRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string> body_names_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
    ::PROTOBUF_NAMESPACE_ID::Struct* extra_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_component_2fposetracker_2fv1_2fpose_5ftracker_2eproto;
};
// -------------------------------------------------------------------

class GetPosesResponse_BodyPosesEntry_DoNotUse : public ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<GetPosesResponse_BodyPosesEntry_DoNotUse, 
    std::string, ::viam::common::v1::PoseInFrame,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_MESSAGE> {
public:
  typedef ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<GetPosesResponse_BodyPosesEntry_DoNotUse, 
    std::string, ::viam::common::v1::PoseInFrame,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_MESSAGE> SuperType;
  GetPosesResponse_BodyPosesEntry_DoNotUse();
  explicit PROTOBUF_CONSTEXPR GetPosesResponse_BodyPosesEntry_DoNotUse(
      ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);
  explicit GetPosesResponse_BodyPosesEntry_DoNotUse(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  void MergeFrom(const GetPosesResponse_BodyPosesEntry_DoNotUse& other);
  static const GetPosesResponse_BodyPosesEntry_DoNotUse* internal_default_instance() { return reinterpret_cast<const GetPosesResponse_BodyPosesEntry_DoNotUse*>(&_GetPosesResponse_BodyPosesEntry_DoNotUse_default_instance_); }
  static bool ValidateKey(std::string* s) {
    return ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(s->data(), static_cast<int>(s->size()), ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE, "viam.component.posetracker.v1.GetPosesResponse.BodyPosesEntry.key");
 }
  static bool ValidateValue(void*) { return true; }
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  friend struct ::TableStruct_component_2fposetracker_2fv1_2fpose_5ftracker_2eproto;
};

// -------------------------------------------------------------------

class GetPosesResponse final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:viam.component.posetracker.v1.GetPosesResponse) */ {
 public:
  inline GetPosesResponse() : GetPosesResponse(nullptr) {}
  ~GetPosesResponse() override;
  explicit PROTOBUF_CONSTEXPR GetPosesResponse(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  GetPosesResponse(const GetPosesResponse& from);
  GetPosesResponse(GetPosesResponse&& from) noexcept
    : GetPosesResponse() {
    *this = ::std::move(from);
  }

  inline GetPosesResponse& operator=(const GetPosesResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline GetPosesResponse& operator=(GetPosesResponse&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const GetPosesResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const GetPosesResponse* internal_default_instance() {
    return reinterpret_cast<const GetPosesResponse*>(
               &_GetPosesResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(GetPosesResponse& a, GetPosesResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(GetPosesResponse* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(GetPosesResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  GetPosesResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<GetPosesResponse>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const GetPosesResponse& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const GetPosesResponse& from) {
    GetPosesResponse::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(GetPosesResponse* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "viam.component.posetracker.v1.GetPosesResponse";
  }
  protected:
  explicit GetPosesResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------


  // accessors -------------------------------------------------------

  enum : int {
    kBodyPosesFieldNumber = 1,
  };
  // map<string, .viam.common.v1.PoseInFrame> body_poses = 1 [json_name = "bodyPoses"];
  int body_poses_size() const;
  private:
  int _internal_body_poses_size() const;
  public:
  void clear_body_poses();
  private:
  const ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::viam::common::v1::PoseInFrame >&
      _internal_body_poses() const;
  ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::viam::common::v1::PoseInFrame >*
      _internal_mutable_body_poses();
  public:
  const ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::viam::common::v1::PoseInFrame >&
      body_poses() const;
  ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::viam::common::v1::PoseInFrame >*
      mutable_body_poses();

  // @@protoc_insertion_point(class_scope:viam.component.posetracker.v1.GetPosesResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::MapField<
        GetPosesResponse_BodyPosesEntry_DoNotUse,
        std::string, ::viam::common::v1::PoseInFrame,
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_MESSAGE> body_poses_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_component_2fposetracker_2fv1_2fpose_5ftracker_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// GetPosesRequest

// string name = 1 [json_name = "name"];
inline void GetPosesRequest::clear_name() {
  _impl_.name_.ClearToEmpty();
}
inline const std::string& GetPosesRequest::name() const {
  // @@protoc_insertion_point(field_get:viam.component.posetracker.v1.GetPosesRequest.name)
  return _internal_name();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void GetPosesRequest::set_name(ArgT0&& arg0, ArgT... args) {
 
 _impl_.name_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:viam.component.posetracker.v1.GetPosesRequest.name)
}
inline std::string* GetPosesRequest::mutable_name() {
  std::string* _s = _internal_mutable_name();
  // @@protoc_insertion_point(field_mutable:viam.component.posetracker.v1.GetPosesRequest.name)
  return _s;
}
inline const std::string& GetPosesRequest::_internal_name() const {
  return _impl_.name_.Get();
}
inline void GetPosesRequest::_internal_set_name(const std::string& value) {
  
  _impl_.name_.Set(value, GetArenaForAllocation());
}
inline std::string* GetPosesRequest::_internal_mutable_name() {
  
  return _impl_.name_.Mutable(GetArenaForAllocation());
}
inline std::string* GetPosesRequest::release_name() {
  // @@protoc_insertion_point(field_release:viam.component.posetracker.v1.GetPosesRequest.name)
  return _impl_.name_.Release();
}
inline void GetPosesRequest::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  _impl_.name_.SetAllocated(name, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.name_.IsDefault()) {
    _impl_.name_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:viam.component.posetracker.v1.GetPosesRequest.name)
}

// repeated string body_names = 2 [json_name = "bodyNames"];
inline int GetPosesRequest::_internal_body_names_size() const {
  return _impl_.body_names_.size();
}
inline int GetPosesRequest::body_names_size() const {
  return _internal_body_names_size();
}
inline void GetPosesRequest::clear_body_names() {
  _impl_.body_names_.Clear();
}
inline std::string* GetPosesRequest::add_body_names() {
  std::string* _s = _internal_add_body_names();
  // @@protoc_insertion_point(field_add_mutable:viam.component.posetracker.v1.GetPosesRequest.body_names)
  return _s;
}
inline const std::string& GetPosesRequest::_internal_body_names(int index) const {
  return _impl_.body_names_.Get(index);
}
inline const std::string& GetPosesRequest::body_names(int index) const {
  // @@protoc_insertion_point(field_get:viam.component.posetracker.v1.GetPosesRequest.body_names)
  return _internal_body_names(index);
}
inline std::string* GetPosesRequest::mutable_body_names(int index) {
  // @@protoc_insertion_point(field_mutable:viam.component.posetracker.v1.GetPosesRequest.body_names)
  return _impl_.body_names_.Mutable(index);
}
inline void GetPosesRequest::set_body_names(int index, const std::string& value) {
  _impl_.body_names_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set:viam.component.posetracker.v1.GetPosesRequest.body_names)
}
inline void GetPosesRequest::set_body_names(int index, std::string&& value) {
  _impl_.body_names_.Mutable(index)->assign(std::move(value));
  // @@protoc_insertion_point(field_set:viam.component.posetracker.v1.GetPosesRequest.body_names)
}
inline void GetPosesRequest::set_body_names(int index, const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _impl_.body_names_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:viam.component.posetracker.v1.GetPosesRequest.body_names)
}
inline void GetPosesRequest::set_body_names(int index, const char* value, size_t size) {
  _impl_.body_names_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:viam.component.posetracker.v1.GetPosesRequest.body_names)
}
inline std::string* GetPosesRequest::_internal_add_body_names() {
  return _impl_.body_names_.Add();
}
inline void GetPosesRequest::add_body_names(const std::string& value) {
  _impl_.body_names_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:viam.component.posetracker.v1.GetPosesRequest.body_names)
}
inline void GetPosesRequest::add_body_names(std::string&& value) {
  _impl_.body_names_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:viam.component.posetracker.v1.GetPosesRequest.body_names)
}
inline void GetPosesRequest::add_body_names(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _impl_.body_names_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:viam.component.posetracker.v1.GetPosesRequest.body_names)
}
inline void GetPosesRequest::add_body_names(const char* value, size_t size) {
  _impl_.body_names_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:viam.component.posetracker.v1.GetPosesRequest.body_names)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>&
GetPosesRequest::body_names() const {
  // @@protoc_insertion_point(field_list:viam.component.posetracker.v1.GetPosesRequest.body_names)
  return _impl_.body_names_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>*
GetPosesRequest::mutable_body_names() {
  // @@protoc_insertion_point(field_mutable_list:viam.component.posetracker.v1.GetPosesRequest.body_names)
  return &_impl_.body_names_;
}

// .google.protobuf.Struct extra = 99 [json_name = "extra"];
inline bool GetPosesRequest::_internal_has_extra() const {
  return this != internal_default_instance() && _impl_.extra_ != nullptr;
}
inline bool GetPosesRequest::has_extra() const {
  return _internal_has_extra();
}
inline const ::PROTOBUF_NAMESPACE_ID::Struct& GetPosesRequest::_internal_extra() const {
  const ::PROTOBUF_NAMESPACE_ID::Struct* p = _impl_.extra_;
  return p != nullptr ? *p : reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Struct&>(
      ::PROTOBUF_NAMESPACE_ID::_Struct_default_instance_);
}
inline const ::PROTOBUF_NAMESPACE_ID::Struct& GetPosesRequest::extra() const {
  // @@protoc_insertion_point(field_get:viam.component.posetracker.v1.GetPosesRequest.extra)
  return _internal_extra();
}
inline void GetPosesRequest::unsafe_arena_set_allocated_extra(
    ::PROTOBUF_NAMESPACE_ID::Struct* extra) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.extra_);
  }
  _impl_.extra_ = extra;
  if (extra) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:viam.component.posetracker.v1.GetPosesRequest.extra)
}
inline ::PROTOBUF_NAMESPACE_ID::Struct* GetPosesRequest::release_extra() {
  
  ::PROTOBUF_NAMESPACE_ID::Struct* temp = _impl_.extra_;
  _impl_.extra_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::PROTOBUF_NAMESPACE_ID::Struct* GetPosesRequest::unsafe_arena_release_extra() {
  // @@protoc_insertion_point(field_release:viam.component.posetracker.v1.GetPosesRequest.extra)
  
  ::PROTOBUF_NAMESPACE_ID::Struct* temp = _impl_.extra_;
  _impl_.extra_ = nullptr;
  return temp;
}
inline ::PROTOBUF_NAMESPACE_ID::Struct* GetPosesRequest::_internal_mutable_extra() {
  
  if (_impl_.extra_ == nullptr) {
    auto* p = CreateMaybeMessage<::PROTOBUF_NAMESPACE_ID::Struct>(GetArenaForAllocation());
    _impl_.extra_ = p;
  }
  return _impl_.extra_;
}
inline ::PROTOBUF_NAMESPACE_ID::Struct* GetPosesRequest::mutable_extra() {
  ::PROTOBUF_NAMESPACE_ID::Struct* _msg = _internal_mutable_extra();
  // @@protoc_insertion_point(field_mutable:viam.component.posetracker.v1.GetPosesRequest.extra)
  return _msg;
}
inline void GetPosesRequest::set_allocated_extra(::PROTOBUF_NAMESPACE_ID::Struct* extra) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.extra_);
  }
  if (extra) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(extra));
    if (message_arena != submessage_arena) {
      extra = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, extra, submessage_arena);
    }
    
  } else {
    
  }
  _impl_.extra_ = extra;
  // @@protoc_insertion_point(field_set_allocated:viam.component.posetracker.v1.GetPosesRequest.extra)
}

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// GetPosesResponse

// map<string, .viam.common.v1.PoseInFrame> body_poses = 1 [json_name = "bodyPoses"];
inline int GetPosesResponse::_internal_body_poses_size() const {
  return _impl_.body_poses_.size();
}
inline int GetPosesResponse::body_poses_size() const {
  return _internal_body_poses_size();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::viam::common::v1::PoseInFrame >&
GetPosesResponse::_internal_body_poses() const {
  return _impl_.body_poses_.GetMap();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::viam::common::v1::PoseInFrame >&
GetPosesResponse::body_poses() const {
  // @@protoc_insertion_point(field_map:viam.component.posetracker.v1.GetPosesResponse.body_poses)
  return _internal_body_poses();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::viam::common::v1::PoseInFrame >*
GetPosesResponse::_internal_mutable_body_poses() {
  return _impl_.body_poses_.MutableMap();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::viam::common::v1::PoseInFrame >*
GetPosesResponse::mutable_body_poses() {
  // @@protoc_insertion_point(field_mutable_map:viam.component.posetracker.v1.GetPosesResponse.body_poses)
  return _internal_mutable_body_poses();
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace v1
}  // namespace posetracker
}  // namespace component
}  // namespace viam

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_component_2fposetracker_2fv1_2fpose_5ftracker_2eproto
