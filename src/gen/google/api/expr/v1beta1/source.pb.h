// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/api/expr/v1beta1/source.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2fexpr_2fv1beta1_2fsource_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2fexpr_2fv1beta1_2fsource_2eproto

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
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_google_2fapi_2fexpr_2fv1beta1_2fsource_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_google_2fapi_2fexpr_2fv1beta1_2fsource_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fapi_2fexpr_2fv1beta1_2fsource_2eproto;
namespace google {
namespace api {
namespace expr {
namespace v1beta1 {
class SourceInfo;
struct SourceInfoDefaultTypeInternal;
extern SourceInfoDefaultTypeInternal _SourceInfo_default_instance_;
class SourceInfo_PositionsEntry_DoNotUse;
struct SourceInfo_PositionsEntry_DoNotUseDefaultTypeInternal;
extern SourceInfo_PositionsEntry_DoNotUseDefaultTypeInternal _SourceInfo_PositionsEntry_DoNotUse_default_instance_;
class SourcePosition;
struct SourcePositionDefaultTypeInternal;
extern SourcePositionDefaultTypeInternal _SourcePosition_default_instance_;
}  // namespace v1beta1
}  // namespace expr
}  // namespace api
}  // namespace google
PROTOBUF_NAMESPACE_OPEN
template<> ::google::api::expr::v1beta1::SourceInfo* Arena::CreateMaybeMessage<::google::api::expr::v1beta1::SourceInfo>(Arena*);
template<> ::google::api::expr::v1beta1::SourceInfo_PositionsEntry_DoNotUse* Arena::CreateMaybeMessage<::google::api::expr::v1beta1::SourceInfo_PositionsEntry_DoNotUse>(Arena*);
template<> ::google::api::expr::v1beta1::SourcePosition* Arena::CreateMaybeMessage<::google::api::expr::v1beta1::SourcePosition>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace google {
namespace api {
namespace expr {
namespace v1beta1 {

// ===================================================================

class SourceInfo_PositionsEntry_DoNotUse : public ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<SourceInfo_PositionsEntry_DoNotUse, 
    int32_t, int32_t,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32> {
public:
  typedef ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<SourceInfo_PositionsEntry_DoNotUse, 
    int32_t, int32_t,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32> SuperType;
  SourceInfo_PositionsEntry_DoNotUse();
  explicit PROTOBUF_CONSTEXPR SourceInfo_PositionsEntry_DoNotUse(
      ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);
  explicit SourceInfo_PositionsEntry_DoNotUse(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  void MergeFrom(const SourceInfo_PositionsEntry_DoNotUse& other);
  static const SourceInfo_PositionsEntry_DoNotUse* internal_default_instance() { return reinterpret_cast<const SourceInfo_PositionsEntry_DoNotUse*>(&_SourceInfo_PositionsEntry_DoNotUse_default_instance_); }
  static bool ValidateKey(void*) { return true; }
  static bool ValidateValue(void*) { return true; }
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  friend struct ::TableStruct_google_2fapi_2fexpr_2fv1beta1_2fsource_2eproto;
};

// -------------------------------------------------------------------

class SourceInfo final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:google.api.expr.v1beta1.SourceInfo) */ {
 public:
  inline SourceInfo() : SourceInfo(nullptr) {}
  ~SourceInfo() override;
  explicit PROTOBUF_CONSTEXPR SourceInfo(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  SourceInfo(const SourceInfo& from);
  SourceInfo(SourceInfo&& from) noexcept
    : SourceInfo() {
    *this = ::std::move(from);
  }

  inline SourceInfo& operator=(const SourceInfo& from) {
    CopyFrom(from);
    return *this;
  }
  inline SourceInfo& operator=(SourceInfo&& from) noexcept {
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
  static const SourceInfo& default_instance() {
    return *internal_default_instance();
  }
  static inline const SourceInfo* internal_default_instance() {
    return reinterpret_cast<const SourceInfo*>(
               &_SourceInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(SourceInfo& a, SourceInfo& b) {
    a.Swap(&b);
  }
  inline void Swap(SourceInfo* other) {
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
  void UnsafeArenaSwap(SourceInfo* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  SourceInfo* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<SourceInfo>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const SourceInfo& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const SourceInfo& from) {
    SourceInfo::MergeImpl(*this, from);
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
  void InternalSwap(SourceInfo* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "google.api.expr.v1beta1.SourceInfo";
  }
  protected:
  explicit SourceInfo(::PROTOBUF_NAMESPACE_ID::Arena* arena,
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
    kLineOffsetsFieldNumber = 3,
    kPositionsFieldNumber = 4,
    kLocationFieldNumber = 2,
  };
  // repeated int32 line_offsets = 3 [json_name = "lineOffsets"];
  int line_offsets_size() const;
  private:
  int _internal_line_offsets_size() const;
  public:
  void clear_line_offsets();
  private:
  int32_t _internal_line_offsets(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< int32_t >&
      _internal_line_offsets() const;
  void _internal_add_line_offsets(int32_t value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< int32_t >*
      _internal_mutable_line_offsets();
  public:
  int32_t line_offsets(int index) const;
  void set_line_offsets(int index, int32_t value);
  void add_line_offsets(int32_t value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< int32_t >&
      line_offsets() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< int32_t >*
      mutable_line_offsets();

  // map<int32, int32> positions = 4 [json_name = "positions"];
  int positions_size() const;
  private:
  int _internal_positions_size() const;
  public:
  void clear_positions();
  private:
  const ::PROTOBUF_NAMESPACE_ID::Map< int32_t, int32_t >&
      _internal_positions() const;
  ::PROTOBUF_NAMESPACE_ID::Map< int32_t, int32_t >*
      _internal_mutable_positions();
  public:
  const ::PROTOBUF_NAMESPACE_ID::Map< int32_t, int32_t >&
      positions() const;
  ::PROTOBUF_NAMESPACE_ID::Map< int32_t, int32_t >*
      mutable_positions();

  // string location = 2 [json_name = "location"];
  void clear_location();
  const std::string& location() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_location(ArgT0&& arg0, ArgT... args);
  std::string* mutable_location();
  PROTOBUF_NODISCARD std::string* release_location();
  void set_allocated_location(std::string* location);
  private:
  const std::string& _internal_location() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_location(const std::string& value);
  std::string* _internal_mutable_location();
  public:

  // @@protoc_insertion_point(class_scope:google.api.expr.v1beta1.SourceInfo)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedField< int32_t > line_offsets_;
    mutable std::atomic<int> _line_offsets_cached_byte_size_;
    ::PROTOBUF_NAMESPACE_ID::internal::MapField<
        SourceInfo_PositionsEntry_DoNotUse,
        int32_t, int32_t,
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32> positions_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr location_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_google_2fapi_2fexpr_2fv1beta1_2fsource_2eproto;
};
// -------------------------------------------------------------------

class SourcePosition final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:google.api.expr.v1beta1.SourcePosition) */ {
 public:
  inline SourcePosition() : SourcePosition(nullptr) {}
  ~SourcePosition() override;
  explicit PROTOBUF_CONSTEXPR SourcePosition(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  SourcePosition(const SourcePosition& from);
  SourcePosition(SourcePosition&& from) noexcept
    : SourcePosition() {
    *this = ::std::move(from);
  }

  inline SourcePosition& operator=(const SourcePosition& from) {
    CopyFrom(from);
    return *this;
  }
  inline SourcePosition& operator=(SourcePosition&& from) noexcept {
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
  static const SourcePosition& default_instance() {
    return *internal_default_instance();
  }
  static inline const SourcePosition* internal_default_instance() {
    return reinterpret_cast<const SourcePosition*>(
               &_SourcePosition_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(SourcePosition& a, SourcePosition& b) {
    a.Swap(&b);
  }
  inline void Swap(SourcePosition* other) {
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
  void UnsafeArenaSwap(SourcePosition* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  SourcePosition* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<SourcePosition>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const SourcePosition& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const SourcePosition& from) {
    SourcePosition::MergeImpl(*this, from);
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
  void InternalSwap(SourcePosition* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "google.api.expr.v1beta1.SourcePosition";
  }
  protected:
  explicit SourcePosition(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kLocationFieldNumber = 1,
    kOffsetFieldNumber = 2,
    kLineFieldNumber = 3,
    kColumnFieldNumber = 4,
  };
  // string location = 1 [json_name = "location"];
  void clear_location();
  const std::string& location() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_location(ArgT0&& arg0, ArgT... args);
  std::string* mutable_location();
  PROTOBUF_NODISCARD std::string* release_location();
  void set_allocated_location(std::string* location);
  private:
  const std::string& _internal_location() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_location(const std::string& value);
  std::string* _internal_mutable_location();
  public:

  // int32 offset = 2 [json_name = "offset"];
  void clear_offset();
  int32_t offset() const;
  void set_offset(int32_t value);
  private:
  int32_t _internal_offset() const;
  void _internal_set_offset(int32_t value);
  public:

  // int32 line = 3 [json_name = "line"];
  void clear_line();
  int32_t line() const;
  void set_line(int32_t value);
  private:
  int32_t _internal_line() const;
  void _internal_set_line(int32_t value);
  public:

  // int32 column = 4 [json_name = "column"];
  void clear_column();
  int32_t column() const;
  void set_column(int32_t value);
  private:
  int32_t _internal_column() const;
  void _internal_set_column(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:google.api.expr.v1beta1.SourcePosition)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr location_;
    int32_t offset_;
    int32_t line_;
    int32_t column_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_google_2fapi_2fexpr_2fv1beta1_2fsource_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// SourceInfo

// string location = 2 [json_name = "location"];
inline void SourceInfo::clear_location() {
  _impl_.location_.ClearToEmpty();
}
inline const std::string& SourceInfo::location() const {
  // @@protoc_insertion_point(field_get:google.api.expr.v1beta1.SourceInfo.location)
  return _internal_location();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void SourceInfo::set_location(ArgT0&& arg0, ArgT... args) {
 
 _impl_.location_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:google.api.expr.v1beta1.SourceInfo.location)
}
inline std::string* SourceInfo::mutable_location() {
  std::string* _s = _internal_mutable_location();
  // @@protoc_insertion_point(field_mutable:google.api.expr.v1beta1.SourceInfo.location)
  return _s;
}
inline const std::string& SourceInfo::_internal_location() const {
  return _impl_.location_.Get();
}
inline void SourceInfo::_internal_set_location(const std::string& value) {
  
  _impl_.location_.Set(value, GetArenaForAllocation());
}
inline std::string* SourceInfo::_internal_mutable_location() {
  
  return _impl_.location_.Mutable(GetArenaForAllocation());
}
inline std::string* SourceInfo::release_location() {
  // @@protoc_insertion_point(field_release:google.api.expr.v1beta1.SourceInfo.location)
  return _impl_.location_.Release();
}
inline void SourceInfo::set_allocated_location(std::string* location) {
  if (location != nullptr) {
    
  } else {
    
  }
  _impl_.location_.SetAllocated(location, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.location_.IsDefault()) {
    _impl_.location_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:google.api.expr.v1beta1.SourceInfo.location)
}

// repeated int32 line_offsets = 3 [json_name = "lineOffsets"];
inline int SourceInfo::_internal_line_offsets_size() const {
  return _impl_.line_offsets_.size();
}
inline int SourceInfo::line_offsets_size() const {
  return _internal_line_offsets_size();
}
inline void SourceInfo::clear_line_offsets() {
  _impl_.line_offsets_.Clear();
}
inline int32_t SourceInfo::_internal_line_offsets(int index) const {
  return _impl_.line_offsets_.Get(index);
}
inline int32_t SourceInfo::line_offsets(int index) const {
  // @@protoc_insertion_point(field_get:google.api.expr.v1beta1.SourceInfo.line_offsets)
  return _internal_line_offsets(index);
}
inline void SourceInfo::set_line_offsets(int index, int32_t value) {
  _impl_.line_offsets_.Set(index, value);
  // @@protoc_insertion_point(field_set:google.api.expr.v1beta1.SourceInfo.line_offsets)
}
inline void SourceInfo::_internal_add_line_offsets(int32_t value) {
  _impl_.line_offsets_.Add(value);
}
inline void SourceInfo::add_line_offsets(int32_t value) {
  _internal_add_line_offsets(value);
  // @@protoc_insertion_point(field_add:google.api.expr.v1beta1.SourceInfo.line_offsets)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< int32_t >&
SourceInfo::_internal_line_offsets() const {
  return _impl_.line_offsets_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< int32_t >&
SourceInfo::line_offsets() const {
  // @@protoc_insertion_point(field_list:google.api.expr.v1beta1.SourceInfo.line_offsets)
  return _internal_line_offsets();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< int32_t >*
SourceInfo::_internal_mutable_line_offsets() {
  return &_impl_.line_offsets_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< int32_t >*
SourceInfo::mutable_line_offsets() {
  // @@protoc_insertion_point(field_mutable_list:google.api.expr.v1beta1.SourceInfo.line_offsets)
  return _internal_mutable_line_offsets();
}

// map<int32, int32> positions = 4 [json_name = "positions"];
inline int SourceInfo::_internal_positions_size() const {
  return _impl_.positions_.size();
}
inline int SourceInfo::positions_size() const {
  return _internal_positions_size();
}
inline void SourceInfo::clear_positions() {
  _impl_.positions_.Clear();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< int32_t, int32_t >&
SourceInfo::_internal_positions() const {
  return _impl_.positions_.GetMap();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< int32_t, int32_t >&
SourceInfo::positions() const {
  // @@protoc_insertion_point(field_map:google.api.expr.v1beta1.SourceInfo.positions)
  return _internal_positions();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< int32_t, int32_t >*
SourceInfo::_internal_mutable_positions() {
  return _impl_.positions_.MutableMap();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< int32_t, int32_t >*
SourceInfo::mutable_positions() {
  // @@protoc_insertion_point(field_mutable_map:google.api.expr.v1beta1.SourceInfo.positions)
  return _internal_mutable_positions();
}

// -------------------------------------------------------------------

// SourcePosition

// string location = 1 [json_name = "location"];
inline void SourcePosition::clear_location() {
  _impl_.location_.ClearToEmpty();
}
inline const std::string& SourcePosition::location() const {
  // @@protoc_insertion_point(field_get:google.api.expr.v1beta1.SourcePosition.location)
  return _internal_location();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void SourcePosition::set_location(ArgT0&& arg0, ArgT... args) {
 
 _impl_.location_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:google.api.expr.v1beta1.SourcePosition.location)
}
inline std::string* SourcePosition::mutable_location() {
  std::string* _s = _internal_mutable_location();
  // @@protoc_insertion_point(field_mutable:google.api.expr.v1beta1.SourcePosition.location)
  return _s;
}
inline const std::string& SourcePosition::_internal_location() const {
  return _impl_.location_.Get();
}
inline void SourcePosition::_internal_set_location(const std::string& value) {
  
  _impl_.location_.Set(value, GetArenaForAllocation());
}
inline std::string* SourcePosition::_internal_mutable_location() {
  
  return _impl_.location_.Mutable(GetArenaForAllocation());
}
inline std::string* SourcePosition::release_location() {
  // @@protoc_insertion_point(field_release:google.api.expr.v1beta1.SourcePosition.location)
  return _impl_.location_.Release();
}
inline void SourcePosition::set_allocated_location(std::string* location) {
  if (location != nullptr) {
    
  } else {
    
  }
  _impl_.location_.SetAllocated(location, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.location_.IsDefault()) {
    _impl_.location_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:google.api.expr.v1beta1.SourcePosition.location)
}

// int32 offset = 2 [json_name = "offset"];
inline void SourcePosition::clear_offset() {
  _impl_.offset_ = 0;
}
inline int32_t SourcePosition::_internal_offset() const {
  return _impl_.offset_;
}
inline int32_t SourcePosition::offset() const {
  // @@protoc_insertion_point(field_get:google.api.expr.v1beta1.SourcePosition.offset)
  return _internal_offset();
}
inline void SourcePosition::_internal_set_offset(int32_t value) {
  
  _impl_.offset_ = value;
}
inline void SourcePosition::set_offset(int32_t value) {
  _internal_set_offset(value);
  // @@protoc_insertion_point(field_set:google.api.expr.v1beta1.SourcePosition.offset)
}

// int32 line = 3 [json_name = "line"];
inline void SourcePosition::clear_line() {
  _impl_.line_ = 0;
}
inline int32_t SourcePosition::_internal_line() const {
  return _impl_.line_;
}
inline int32_t SourcePosition::line() const {
  // @@protoc_insertion_point(field_get:google.api.expr.v1beta1.SourcePosition.line)
  return _internal_line();
}
inline void SourcePosition::_internal_set_line(int32_t value) {
  
  _impl_.line_ = value;
}
inline void SourcePosition::set_line(int32_t value) {
  _internal_set_line(value);
  // @@protoc_insertion_point(field_set:google.api.expr.v1beta1.SourcePosition.line)
}

// int32 column = 4 [json_name = "column"];
inline void SourcePosition::clear_column() {
  _impl_.column_ = 0;
}
inline int32_t SourcePosition::_internal_column() const {
  return _impl_.column_;
}
inline int32_t SourcePosition::column() const {
  // @@protoc_insertion_point(field_get:google.api.expr.v1beta1.SourcePosition.column)
  return _internal_column();
}
inline void SourcePosition::_internal_set_column(int32_t value) {
  
  _impl_.column_ = value;
}
inline void SourcePosition::set_column(int32_t value) {
  _internal_set_column(value);
  // @@protoc_insertion_point(field_set:google.api.expr.v1beta1.SourcePosition.column)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace v1beta1
}  // namespace expr
}  // namespace api
}  // namespace google

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2fexpr_2fv1beta1_2fsource_2eproto
