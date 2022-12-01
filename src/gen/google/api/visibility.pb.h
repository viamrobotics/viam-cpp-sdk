// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/api/visibility.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2fvisibility_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2fvisibility_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021009 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/descriptor.pb.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_google_2fapi_2fvisibility_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_google_2fapi_2fvisibility_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fapi_2fvisibility_2eproto;
namespace google {
namespace api {
class Visibility;
struct VisibilityDefaultTypeInternal;
extern VisibilityDefaultTypeInternal _Visibility_default_instance_;
class VisibilityRule;
struct VisibilityRuleDefaultTypeInternal;
extern VisibilityRuleDefaultTypeInternal _VisibilityRule_default_instance_;
}  // namespace api
}  // namespace google
PROTOBUF_NAMESPACE_OPEN
template<> ::google::api::Visibility* Arena::CreateMaybeMessage<::google::api::Visibility>(Arena*);
template<> ::google::api::VisibilityRule* Arena::CreateMaybeMessage<::google::api::VisibilityRule>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace google {
namespace api {

// ===================================================================

class Visibility final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:google.api.Visibility) */ {
 public:
  inline Visibility() : Visibility(nullptr) {}
  ~Visibility() override;
  explicit PROTOBUF_CONSTEXPR Visibility(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Visibility(const Visibility& from);
  Visibility(Visibility&& from) noexcept
    : Visibility() {
    *this = ::std::move(from);
  }

  inline Visibility& operator=(const Visibility& from) {
    CopyFrom(from);
    return *this;
  }
  inline Visibility& operator=(Visibility&& from) noexcept {
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
  static const Visibility& default_instance() {
    return *internal_default_instance();
  }
  static inline const Visibility* internal_default_instance() {
    return reinterpret_cast<const Visibility*>(
               &_Visibility_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Visibility& a, Visibility& b) {
    a.Swap(&b);
  }
  inline void Swap(Visibility* other) {
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
  void UnsafeArenaSwap(Visibility* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Visibility* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Visibility>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Visibility& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const Visibility& from) {
    Visibility::MergeImpl(*this, from);
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
  void InternalSwap(Visibility* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "google.api.Visibility";
  }
  protected:
  explicit Visibility(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRulesFieldNumber = 1,
  };
  // repeated .google.api.VisibilityRule rules = 1 [json_name = "rules"];
  int rules_size() const;
  private:
  int _internal_rules_size() const;
  public:
  void clear_rules();
  ::google::api::VisibilityRule* mutable_rules(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::google::api::VisibilityRule >*
      mutable_rules();
  private:
  const ::google::api::VisibilityRule& _internal_rules(int index) const;
  ::google::api::VisibilityRule* _internal_add_rules();
  public:
  const ::google::api::VisibilityRule& rules(int index) const;
  ::google::api::VisibilityRule* add_rules();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::google::api::VisibilityRule >&
      rules() const;

  // @@protoc_insertion_point(class_scope:google.api.Visibility)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::google::api::VisibilityRule > rules_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_google_2fapi_2fvisibility_2eproto;
};
// -------------------------------------------------------------------

class VisibilityRule final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:google.api.VisibilityRule) */ {
 public:
  inline VisibilityRule() : VisibilityRule(nullptr) {}
  ~VisibilityRule() override;
  explicit PROTOBUF_CONSTEXPR VisibilityRule(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  VisibilityRule(const VisibilityRule& from);
  VisibilityRule(VisibilityRule&& from) noexcept
    : VisibilityRule() {
    *this = ::std::move(from);
  }

  inline VisibilityRule& operator=(const VisibilityRule& from) {
    CopyFrom(from);
    return *this;
  }
  inline VisibilityRule& operator=(VisibilityRule&& from) noexcept {
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
  static const VisibilityRule& default_instance() {
    return *internal_default_instance();
  }
  static inline const VisibilityRule* internal_default_instance() {
    return reinterpret_cast<const VisibilityRule*>(
               &_VisibilityRule_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(VisibilityRule& a, VisibilityRule& b) {
    a.Swap(&b);
  }
  inline void Swap(VisibilityRule* other) {
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
  void UnsafeArenaSwap(VisibilityRule* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  VisibilityRule* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<VisibilityRule>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const VisibilityRule& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const VisibilityRule& from) {
    VisibilityRule::MergeImpl(*this, from);
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
  void InternalSwap(VisibilityRule* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "google.api.VisibilityRule";
  }
  protected:
  explicit VisibilityRule(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kSelectorFieldNumber = 1,
    kRestrictionFieldNumber = 2,
  };
  // string selector = 1 [json_name = "selector"];
  void clear_selector();
  const std::string& selector() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_selector(ArgT0&& arg0, ArgT... args);
  std::string* mutable_selector();
  PROTOBUF_NODISCARD std::string* release_selector();
  void set_allocated_selector(std::string* selector);
  private:
  const std::string& _internal_selector() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_selector(const std::string& value);
  std::string* _internal_mutable_selector();
  public:

  // string restriction = 2 [json_name = "restriction"];
  void clear_restriction();
  const std::string& restriction() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_restriction(ArgT0&& arg0, ArgT... args);
  std::string* mutable_restriction();
  PROTOBUF_NODISCARD std::string* release_restriction();
  void set_allocated_restriction(std::string* restriction);
  private:
  const std::string& _internal_restriction() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_restriction(const std::string& value);
  std::string* _internal_mutable_restriction();
  public:

  // @@protoc_insertion_point(class_scope:google.api.VisibilityRule)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr selector_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr restriction_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_google_2fapi_2fvisibility_2eproto;
};
// ===================================================================

static const int kEnumVisibilityFieldNumber = 72295727;
extern ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::PROTOBUF_NAMESPACE_ID::EnumOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::MessageTypeTraits< ::google::api::VisibilityRule >, 11, false >
  enum_visibility;
static const int kValueVisibilityFieldNumber = 72295727;
extern ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::PROTOBUF_NAMESPACE_ID::EnumValueOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::MessageTypeTraits< ::google::api::VisibilityRule >, 11, false >
  value_visibility;
static const int kFieldVisibilityFieldNumber = 72295727;
extern ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::PROTOBUF_NAMESPACE_ID::FieldOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::MessageTypeTraits< ::google::api::VisibilityRule >, 11, false >
  field_visibility;
static const int kMessageVisibilityFieldNumber = 72295727;
extern ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::PROTOBUF_NAMESPACE_ID::MessageOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::MessageTypeTraits< ::google::api::VisibilityRule >, 11, false >
  message_visibility;
static const int kMethodVisibilityFieldNumber = 72295727;
extern ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::PROTOBUF_NAMESPACE_ID::MethodOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::MessageTypeTraits< ::google::api::VisibilityRule >, 11, false >
  method_visibility;
static const int kApiVisibilityFieldNumber = 72295727;
extern ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::PROTOBUF_NAMESPACE_ID::ServiceOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::MessageTypeTraits< ::google::api::VisibilityRule >, 11, false >
  api_visibility;

// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Visibility

// repeated .google.api.VisibilityRule rules = 1 [json_name = "rules"];
inline int Visibility::_internal_rules_size() const {
  return _impl_.rules_.size();
}
inline int Visibility::rules_size() const {
  return _internal_rules_size();
}
inline void Visibility::clear_rules() {
  _impl_.rules_.Clear();
}
inline ::google::api::VisibilityRule* Visibility::mutable_rules(int index) {
  // @@protoc_insertion_point(field_mutable:google.api.Visibility.rules)
  return _impl_.rules_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::google::api::VisibilityRule >*
Visibility::mutable_rules() {
  // @@protoc_insertion_point(field_mutable_list:google.api.Visibility.rules)
  return &_impl_.rules_;
}
inline const ::google::api::VisibilityRule& Visibility::_internal_rules(int index) const {
  return _impl_.rules_.Get(index);
}
inline const ::google::api::VisibilityRule& Visibility::rules(int index) const {
  // @@protoc_insertion_point(field_get:google.api.Visibility.rules)
  return _internal_rules(index);
}
inline ::google::api::VisibilityRule* Visibility::_internal_add_rules() {
  return _impl_.rules_.Add();
}
inline ::google::api::VisibilityRule* Visibility::add_rules() {
  ::google::api::VisibilityRule* _add = _internal_add_rules();
  // @@protoc_insertion_point(field_add:google.api.Visibility.rules)
  return _add;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::google::api::VisibilityRule >&
Visibility::rules() const {
  // @@protoc_insertion_point(field_list:google.api.Visibility.rules)
  return _impl_.rules_;
}

// -------------------------------------------------------------------

// VisibilityRule

// string selector = 1 [json_name = "selector"];
inline void VisibilityRule::clear_selector() {
  _impl_.selector_.ClearToEmpty();
}
inline const std::string& VisibilityRule::selector() const {
  // @@protoc_insertion_point(field_get:google.api.VisibilityRule.selector)
  return _internal_selector();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void VisibilityRule::set_selector(ArgT0&& arg0, ArgT... args) {
 
 _impl_.selector_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:google.api.VisibilityRule.selector)
}
inline std::string* VisibilityRule::mutable_selector() {
  std::string* _s = _internal_mutable_selector();
  // @@protoc_insertion_point(field_mutable:google.api.VisibilityRule.selector)
  return _s;
}
inline const std::string& VisibilityRule::_internal_selector() const {
  return _impl_.selector_.Get();
}
inline void VisibilityRule::_internal_set_selector(const std::string& value) {
  
  _impl_.selector_.Set(value, GetArenaForAllocation());
}
inline std::string* VisibilityRule::_internal_mutable_selector() {
  
  return _impl_.selector_.Mutable(GetArenaForAllocation());
}
inline std::string* VisibilityRule::release_selector() {
  // @@protoc_insertion_point(field_release:google.api.VisibilityRule.selector)
  return _impl_.selector_.Release();
}
inline void VisibilityRule::set_allocated_selector(std::string* selector) {
  if (selector != nullptr) {
    
  } else {
    
  }
  _impl_.selector_.SetAllocated(selector, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.selector_.IsDefault()) {
    _impl_.selector_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:google.api.VisibilityRule.selector)
}

// string restriction = 2 [json_name = "restriction"];
inline void VisibilityRule::clear_restriction() {
  _impl_.restriction_.ClearToEmpty();
}
inline const std::string& VisibilityRule::restriction() const {
  // @@protoc_insertion_point(field_get:google.api.VisibilityRule.restriction)
  return _internal_restriction();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void VisibilityRule::set_restriction(ArgT0&& arg0, ArgT... args) {
 
 _impl_.restriction_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:google.api.VisibilityRule.restriction)
}
inline std::string* VisibilityRule::mutable_restriction() {
  std::string* _s = _internal_mutable_restriction();
  // @@protoc_insertion_point(field_mutable:google.api.VisibilityRule.restriction)
  return _s;
}
inline const std::string& VisibilityRule::_internal_restriction() const {
  return _impl_.restriction_.Get();
}
inline void VisibilityRule::_internal_set_restriction(const std::string& value) {
  
  _impl_.restriction_.Set(value, GetArenaForAllocation());
}
inline std::string* VisibilityRule::_internal_mutable_restriction() {
  
  return _impl_.restriction_.Mutable(GetArenaForAllocation());
}
inline std::string* VisibilityRule::release_restriction() {
  // @@protoc_insertion_point(field_release:google.api.VisibilityRule.restriction)
  return _impl_.restriction_.Release();
}
inline void VisibilityRule::set_allocated_restriction(std::string* restriction) {
  if (restriction != nullptr) {
    
  } else {
    
  }
  _impl_.restriction_.SetAllocated(restriction, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.restriction_.IsDefault()) {
    _impl_.restriction_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:google.api.VisibilityRule.restriction)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace api
}  // namespace google

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2fvisibility_2eproto
