// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/rpc/status.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_google_2frpc_2fstatus_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_google_2frpc_2fstatus_2eproto

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
#include <google/protobuf/any.pb.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_google_2frpc_2fstatus_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_google_2frpc_2fstatus_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2frpc_2fstatus_2eproto;
namespace google {
namespace rpc {
class Status;
struct StatusDefaultTypeInternal;
extern StatusDefaultTypeInternal _Status_default_instance_;
}  // namespace rpc
}  // namespace google
PROTOBUF_NAMESPACE_OPEN
template<> ::google::rpc::Status* Arena::CreateMaybeMessage<::google::rpc::Status>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace google {
namespace rpc {

// ===================================================================

class Status final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:google.rpc.Status) */ {
 public:
  inline Status() : Status(nullptr) {}
  ~Status() override;
  explicit PROTOBUF_CONSTEXPR Status(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Status(const Status& from);
  Status(Status&& from) noexcept
    : Status() {
    *this = ::std::move(from);
  }

  inline Status& operator=(const Status& from) {
    CopyFrom(from);
    return *this;
  }
  inline Status& operator=(Status&& from) noexcept {
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
  static const Status& default_instance() {
    return *internal_default_instance();
  }
  static inline const Status* internal_default_instance() {
    return reinterpret_cast<const Status*>(
               &_Status_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Status& a, Status& b) {
    a.Swap(&b);
  }
  inline void Swap(Status* other) {
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
  void UnsafeArenaSwap(Status* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Status* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Status>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Status& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const Status& from) {
    Status::MergeImpl(*this, from);
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
  void InternalSwap(Status* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "google.rpc.Status";
  }
  protected:
  explicit Status(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kDetailsFieldNumber = 3,
    kMessageFieldNumber = 2,
    kCodeFieldNumber = 1,
  };
  // repeated .google.protobuf.Any details = 3 [json_name = "details"];
  int details_size() const;
  private:
  int _internal_details_size() const;
  public:
  void clear_details();
  ::PROTOBUF_NAMESPACE_ID::Any* mutable_details(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PROTOBUF_NAMESPACE_ID::Any >*
      mutable_details();
  private:
  const ::PROTOBUF_NAMESPACE_ID::Any& _internal_details(int index) const;
  ::PROTOBUF_NAMESPACE_ID::Any* _internal_add_details();
  public:
  const ::PROTOBUF_NAMESPACE_ID::Any& details(int index) const;
  ::PROTOBUF_NAMESPACE_ID::Any* add_details();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PROTOBUF_NAMESPACE_ID::Any >&
      details() const;

  // string message = 2 [json_name = "message"];
  void clear_message();
  const std::string& message() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_message(ArgT0&& arg0, ArgT... args);
  std::string* mutable_message();
  PROTOBUF_NODISCARD std::string* release_message();
  void set_allocated_message(std::string* message);
  private:
  const std::string& _internal_message() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_message(const std::string& value);
  std::string* _internal_mutable_message();
  public:

  // int32 code = 1 [json_name = "code"];
  void clear_code();
  int32_t code() const;
  void set_code(int32_t value);
  private:
  int32_t _internal_code() const;
  void _internal_set_code(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:google.rpc.Status)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PROTOBUF_NAMESPACE_ID::Any > details_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr message_;
    int32_t code_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_google_2frpc_2fstatus_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Status

// int32 code = 1 [json_name = "code"];
inline void Status::clear_code() {
  _impl_.code_ = 0;
}
inline int32_t Status::_internal_code() const {
  return _impl_.code_;
}
inline int32_t Status::code() const {
  // @@protoc_insertion_point(field_get:google.rpc.Status.code)
  return _internal_code();
}
inline void Status::_internal_set_code(int32_t value) {
  
  _impl_.code_ = value;
}
inline void Status::set_code(int32_t value) {
  _internal_set_code(value);
  // @@protoc_insertion_point(field_set:google.rpc.Status.code)
}

// string message = 2 [json_name = "message"];
inline void Status::clear_message() {
  _impl_.message_.ClearToEmpty();
}
inline const std::string& Status::message() const {
  // @@protoc_insertion_point(field_get:google.rpc.Status.message)
  return _internal_message();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Status::set_message(ArgT0&& arg0, ArgT... args) {
 
 _impl_.message_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:google.rpc.Status.message)
}
inline std::string* Status::mutable_message() {
  std::string* _s = _internal_mutable_message();
  // @@protoc_insertion_point(field_mutable:google.rpc.Status.message)
  return _s;
}
inline const std::string& Status::_internal_message() const {
  return _impl_.message_.Get();
}
inline void Status::_internal_set_message(const std::string& value) {
  
  _impl_.message_.Set(value, GetArenaForAllocation());
}
inline std::string* Status::_internal_mutable_message() {
  
  return _impl_.message_.Mutable(GetArenaForAllocation());
}
inline std::string* Status::release_message() {
  // @@protoc_insertion_point(field_release:google.rpc.Status.message)
  return _impl_.message_.Release();
}
inline void Status::set_allocated_message(std::string* message) {
  if (message != nullptr) {
    
  } else {
    
  }
  _impl_.message_.SetAllocated(message, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.message_.IsDefault()) {
    _impl_.message_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:google.rpc.Status.message)
}

// repeated .google.protobuf.Any details = 3 [json_name = "details"];
inline int Status::_internal_details_size() const {
  return _impl_.details_.size();
}
inline int Status::details_size() const {
  return _internal_details_size();
}
inline ::PROTOBUF_NAMESPACE_ID::Any* Status::mutable_details(int index) {
  // @@protoc_insertion_point(field_mutable:google.rpc.Status.details)
  return _impl_.details_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PROTOBUF_NAMESPACE_ID::Any >*
Status::mutable_details() {
  // @@protoc_insertion_point(field_mutable_list:google.rpc.Status.details)
  return &_impl_.details_;
}
inline const ::PROTOBUF_NAMESPACE_ID::Any& Status::_internal_details(int index) const {
  return _impl_.details_.Get(index);
}
inline const ::PROTOBUF_NAMESPACE_ID::Any& Status::details(int index) const {
  // @@protoc_insertion_point(field_get:google.rpc.Status.details)
  return _internal_details(index);
}
inline ::PROTOBUF_NAMESPACE_ID::Any* Status::_internal_add_details() {
  return _impl_.details_.Add();
}
inline ::PROTOBUF_NAMESPACE_ID::Any* Status::add_details() {
  ::PROTOBUF_NAMESPACE_ID::Any* _add = _internal_add_details();
  // @@protoc_insertion_point(field_add:google.rpc.Status.details)
  return _add;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PROTOBUF_NAMESPACE_ID::Any >&
Status::details() const {
  // @@protoc_insertion_point(field_list:google.rpc.Status.details)
  return _impl_.details_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace rpc
}  // namespace google

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_google_2frpc_2fstatus_2eproto
