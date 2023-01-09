// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: service/shell/v1/shell.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_service_2fshell_2fv1_2fshell_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_service_2fshell_2fv1_2fshell_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/unknown_field_set.h>
#include <google/protobuf/struct.pb.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_service_2fshell_2fv1_2fshell_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_service_2fshell_2fv1_2fshell_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_service_2fshell_2fv1_2fshell_2eproto;
namespace viam {
namespace service {
namespace shell {
namespace v1 {
class ShellRequest;
struct ShellRequestDefaultTypeInternal;
extern ShellRequestDefaultTypeInternal _ShellRequest_default_instance_;
class ShellResponse;
struct ShellResponseDefaultTypeInternal;
extern ShellResponseDefaultTypeInternal _ShellResponse_default_instance_;
}  // namespace v1
}  // namespace shell
}  // namespace service
}  // namespace viam
PROTOBUF_NAMESPACE_OPEN
template<> ::viam::service::shell::v1::ShellRequest* Arena::CreateMaybeMessage<::viam::service::shell::v1::ShellRequest>(Arena*);
template<> ::viam::service::shell::v1::ShellResponse* Arena::CreateMaybeMessage<::viam::service::shell::v1::ShellResponse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace viam {
namespace service {
namespace shell {
namespace v1 {

// ===================================================================

class ShellRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:viam.service.shell.v1.ShellRequest) */ {
 public:
  inline ShellRequest() : ShellRequest(nullptr) {}
  ~ShellRequest() override;
  explicit PROTOBUF_CONSTEXPR ShellRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ShellRequest(const ShellRequest& from);
  ShellRequest(ShellRequest&& from) noexcept
    : ShellRequest() {
    *this = ::std::move(from);
  }

  inline ShellRequest& operator=(const ShellRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline ShellRequest& operator=(ShellRequest&& from) noexcept {
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
  static const ShellRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const ShellRequest* internal_default_instance() {
    return reinterpret_cast<const ShellRequest*>(
               &_ShellRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ShellRequest& a, ShellRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(ShellRequest* other) {
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
  void UnsafeArenaSwap(ShellRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ShellRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ShellRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ShellRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const ShellRequest& from) {
    ShellRequest::MergeImpl(*this, from);
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
  void InternalSwap(ShellRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "viam.service.shell.v1.ShellRequest";
  }
  protected:
  explicit ShellRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 1,
    kDataInFieldNumber = 2,
    kExtraFieldNumber = 99,
  };
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

  // string data_in = 2 [json_name = "dataIn"];
  void clear_data_in();
  const std::string& data_in() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_data_in(ArgT0&& arg0, ArgT... args);
  std::string* mutable_data_in();
  PROTOBUF_NODISCARD std::string* release_data_in();
  void set_allocated_data_in(std::string* data_in);
  private:
  const std::string& _internal_data_in() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_data_in(const std::string& value);
  std::string* _internal_mutable_data_in();
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

  // @@protoc_insertion_point(class_scope:viam.service.shell.v1.ShellRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_in_;
    ::PROTOBUF_NAMESPACE_ID::Struct* extra_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_service_2fshell_2fv1_2fshell_2eproto;
};
// -------------------------------------------------------------------

class ShellResponse final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:viam.service.shell.v1.ShellResponse) */ {
 public:
  inline ShellResponse() : ShellResponse(nullptr) {}
  ~ShellResponse() override;
  explicit PROTOBUF_CONSTEXPR ShellResponse(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ShellResponse(const ShellResponse& from);
  ShellResponse(ShellResponse&& from) noexcept
    : ShellResponse() {
    *this = ::std::move(from);
  }

  inline ShellResponse& operator=(const ShellResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline ShellResponse& operator=(ShellResponse&& from) noexcept {
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
  static const ShellResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const ShellResponse* internal_default_instance() {
    return reinterpret_cast<const ShellResponse*>(
               &_ShellResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(ShellResponse& a, ShellResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(ShellResponse* other) {
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
  void UnsafeArenaSwap(ShellResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ShellResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ShellResponse>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ShellResponse& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const ShellResponse& from) {
    ShellResponse::MergeImpl(*this, from);
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
  void InternalSwap(ShellResponse* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "viam.service.shell.v1.ShellResponse";
  }
  protected:
  explicit ShellResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kDataOutFieldNumber = 1,
    kDataErrFieldNumber = 2,
    kEofFieldNumber = 3,
  };
  // string data_out = 1 [json_name = "dataOut"];
  void clear_data_out();
  const std::string& data_out() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_data_out(ArgT0&& arg0, ArgT... args);
  std::string* mutable_data_out();
  PROTOBUF_NODISCARD std::string* release_data_out();
  void set_allocated_data_out(std::string* data_out);
  private:
  const std::string& _internal_data_out() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_data_out(const std::string& value);
  std::string* _internal_mutable_data_out();
  public:

  // string data_err = 2 [json_name = "dataErr"];
  void clear_data_err();
  const std::string& data_err() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_data_err(ArgT0&& arg0, ArgT... args);
  std::string* mutable_data_err();
  PROTOBUF_NODISCARD std::string* release_data_err();
  void set_allocated_data_err(std::string* data_err);
  private:
  const std::string& _internal_data_err() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_data_err(const std::string& value);
  std::string* _internal_mutable_data_err();
  public:

  // bool eof = 3 [json_name = "eof"];
  void clear_eof();
  bool eof() const;
  void set_eof(bool value);
  private:
  bool _internal_eof() const;
  void _internal_set_eof(bool value);
  public:

  // @@protoc_insertion_point(class_scope:viam.service.shell.v1.ShellResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_out_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_err_;
    bool eof_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_service_2fshell_2fv1_2fshell_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ShellRequest

// string name = 1 [json_name = "name"];
inline void ShellRequest::clear_name() {
  _impl_.name_.ClearToEmpty();
}
inline const std::string& ShellRequest::name() const {
  // @@protoc_insertion_point(field_get:viam.service.shell.v1.ShellRequest.name)
  return _internal_name();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void ShellRequest::set_name(ArgT0&& arg0, ArgT... args) {
 
 _impl_.name_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:viam.service.shell.v1.ShellRequest.name)
}
inline std::string* ShellRequest::mutable_name() {
  std::string* _s = _internal_mutable_name();
  // @@protoc_insertion_point(field_mutable:viam.service.shell.v1.ShellRequest.name)
  return _s;
}
inline const std::string& ShellRequest::_internal_name() const {
  return _impl_.name_.Get();
}
inline void ShellRequest::_internal_set_name(const std::string& value) {
  
  _impl_.name_.Set(value, GetArenaForAllocation());
}
inline std::string* ShellRequest::_internal_mutable_name() {
  
  return _impl_.name_.Mutable(GetArenaForAllocation());
}
inline std::string* ShellRequest::release_name() {
  // @@protoc_insertion_point(field_release:viam.service.shell.v1.ShellRequest.name)
  return _impl_.name_.Release();
}
inline void ShellRequest::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  _impl_.name_.SetAllocated(name, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.name_.IsDefault()) {
    _impl_.name_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:viam.service.shell.v1.ShellRequest.name)
}

// string data_in = 2 [json_name = "dataIn"];
inline void ShellRequest::clear_data_in() {
  _impl_.data_in_.ClearToEmpty();
}
inline const std::string& ShellRequest::data_in() const {
  // @@protoc_insertion_point(field_get:viam.service.shell.v1.ShellRequest.data_in)
  return _internal_data_in();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void ShellRequest::set_data_in(ArgT0&& arg0, ArgT... args) {
 
 _impl_.data_in_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:viam.service.shell.v1.ShellRequest.data_in)
}
inline std::string* ShellRequest::mutable_data_in() {
  std::string* _s = _internal_mutable_data_in();
  // @@protoc_insertion_point(field_mutable:viam.service.shell.v1.ShellRequest.data_in)
  return _s;
}
inline const std::string& ShellRequest::_internal_data_in() const {
  return _impl_.data_in_.Get();
}
inline void ShellRequest::_internal_set_data_in(const std::string& value) {
  
  _impl_.data_in_.Set(value, GetArenaForAllocation());
}
inline std::string* ShellRequest::_internal_mutable_data_in() {
  
  return _impl_.data_in_.Mutable(GetArenaForAllocation());
}
inline std::string* ShellRequest::release_data_in() {
  // @@protoc_insertion_point(field_release:viam.service.shell.v1.ShellRequest.data_in)
  return _impl_.data_in_.Release();
}
inline void ShellRequest::set_allocated_data_in(std::string* data_in) {
  if (data_in != nullptr) {
    
  } else {
    
  }
  _impl_.data_in_.SetAllocated(data_in, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.data_in_.IsDefault()) {
    _impl_.data_in_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:viam.service.shell.v1.ShellRequest.data_in)
}

// .google.protobuf.Struct extra = 99 [json_name = "extra"];
inline bool ShellRequest::_internal_has_extra() const {
  return this != internal_default_instance() && _impl_.extra_ != nullptr;
}
inline bool ShellRequest::has_extra() const {
  return _internal_has_extra();
}
inline const ::PROTOBUF_NAMESPACE_ID::Struct& ShellRequest::_internal_extra() const {
  const ::PROTOBUF_NAMESPACE_ID::Struct* p = _impl_.extra_;
  return p != nullptr ? *p : reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Struct&>(
      ::PROTOBUF_NAMESPACE_ID::_Struct_default_instance_);
}
inline const ::PROTOBUF_NAMESPACE_ID::Struct& ShellRequest::extra() const {
  // @@protoc_insertion_point(field_get:viam.service.shell.v1.ShellRequest.extra)
  return _internal_extra();
}
inline void ShellRequest::unsafe_arena_set_allocated_extra(
    ::PROTOBUF_NAMESPACE_ID::Struct* extra) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.extra_);
  }
  _impl_.extra_ = extra;
  if (extra) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:viam.service.shell.v1.ShellRequest.extra)
}
inline ::PROTOBUF_NAMESPACE_ID::Struct* ShellRequest::release_extra() {
  
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
inline ::PROTOBUF_NAMESPACE_ID::Struct* ShellRequest::unsafe_arena_release_extra() {
  // @@protoc_insertion_point(field_release:viam.service.shell.v1.ShellRequest.extra)
  
  ::PROTOBUF_NAMESPACE_ID::Struct* temp = _impl_.extra_;
  _impl_.extra_ = nullptr;
  return temp;
}
inline ::PROTOBUF_NAMESPACE_ID::Struct* ShellRequest::_internal_mutable_extra() {
  
  if (_impl_.extra_ == nullptr) {
    auto* p = CreateMaybeMessage<::PROTOBUF_NAMESPACE_ID::Struct>(GetArenaForAllocation());
    _impl_.extra_ = p;
  }
  return _impl_.extra_;
}
inline ::PROTOBUF_NAMESPACE_ID::Struct* ShellRequest::mutable_extra() {
  ::PROTOBUF_NAMESPACE_ID::Struct* _msg = _internal_mutable_extra();
  // @@protoc_insertion_point(field_mutable:viam.service.shell.v1.ShellRequest.extra)
  return _msg;
}
inline void ShellRequest::set_allocated_extra(::PROTOBUF_NAMESPACE_ID::Struct* extra) {
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
  // @@protoc_insertion_point(field_set_allocated:viam.service.shell.v1.ShellRequest.extra)
}

// -------------------------------------------------------------------

// ShellResponse

// string data_out = 1 [json_name = "dataOut"];
inline void ShellResponse::clear_data_out() {
  _impl_.data_out_.ClearToEmpty();
}
inline const std::string& ShellResponse::data_out() const {
  // @@protoc_insertion_point(field_get:viam.service.shell.v1.ShellResponse.data_out)
  return _internal_data_out();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void ShellResponse::set_data_out(ArgT0&& arg0, ArgT... args) {
 
 _impl_.data_out_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:viam.service.shell.v1.ShellResponse.data_out)
}
inline std::string* ShellResponse::mutable_data_out() {
  std::string* _s = _internal_mutable_data_out();
  // @@protoc_insertion_point(field_mutable:viam.service.shell.v1.ShellResponse.data_out)
  return _s;
}
inline const std::string& ShellResponse::_internal_data_out() const {
  return _impl_.data_out_.Get();
}
inline void ShellResponse::_internal_set_data_out(const std::string& value) {
  
  _impl_.data_out_.Set(value, GetArenaForAllocation());
}
inline std::string* ShellResponse::_internal_mutable_data_out() {
  
  return _impl_.data_out_.Mutable(GetArenaForAllocation());
}
inline std::string* ShellResponse::release_data_out() {
  // @@protoc_insertion_point(field_release:viam.service.shell.v1.ShellResponse.data_out)
  return _impl_.data_out_.Release();
}
inline void ShellResponse::set_allocated_data_out(std::string* data_out) {
  if (data_out != nullptr) {
    
  } else {
    
  }
  _impl_.data_out_.SetAllocated(data_out, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.data_out_.IsDefault()) {
    _impl_.data_out_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:viam.service.shell.v1.ShellResponse.data_out)
}

// string data_err = 2 [json_name = "dataErr"];
inline void ShellResponse::clear_data_err() {
  _impl_.data_err_.ClearToEmpty();
}
inline const std::string& ShellResponse::data_err() const {
  // @@protoc_insertion_point(field_get:viam.service.shell.v1.ShellResponse.data_err)
  return _internal_data_err();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void ShellResponse::set_data_err(ArgT0&& arg0, ArgT... args) {
 
 _impl_.data_err_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:viam.service.shell.v1.ShellResponse.data_err)
}
inline std::string* ShellResponse::mutable_data_err() {
  std::string* _s = _internal_mutable_data_err();
  // @@protoc_insertion_point(field_mutable:viam.service.shell.v1.ShellResponse.data_err)
  return _s;
}
inline const std::string& ShellResponse::_internal_data_err() const {
  return _impl_.data_err_.Get();
}
inline void ShellResponse::_internal_set_data_err(const std::string& value) {
  
  _impl_.data_err_.Set(value, GetArenaForAllocation());
}
inline std::string* ShellResponse::_internal_mutable_data_err() {
  
  return _impl_.data_err_.Mutable(GetArenaForAllocation());
}
inline std::string* ShellResponse::release_data_err() {
  // @@protoc_insertion_point(field_release:viam.service.shell.v1.ShellResponse.data_err)
  return _impl_.data_err_.Release();
}
inline void ShellResponse::set_allocated_data_err(std::string* data_err) {
  if (data_err != nullptr) {
    
  } else {
    
  }
  _impl_.data_err_.SetAllocated(data_err, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.data_err_.IsDefault()) {
    _impl_.data_err_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:viam.service.shell.v1.ShellResponse.data_err)
}

// bool eof = 3 [json_name = "eof"];
inline void ShellResponse::clear_eof() {
  _impl_.eof_ = false;
}
inline bool ShellResponse::_internal_eof() const {
  return _impl_.eof_;
}
inline bool ShellResponse::eof() const {
  // @@protoc_insertion_point(field_get:viam.service.shell.v1.ShellResponse.eof)
  return _internal_eof();
}
inline void ShellResponse::_internal_set_eof(bool value) {
  
  _impl_.eof_ = value;
}
inline void ShellResponse::set_eof(bool value) {
  _internal_set_eof(value);
  // @@protoc_insertion_point(field_set:viam.service.shell.v1.ShellResponse.eof)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace v1
}  // namespace shell
}  // namespace service
}  // namespace viam

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_service_2fshell_2fv1_2fshell_2eproto
