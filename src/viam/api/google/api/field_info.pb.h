// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/api/field_info.proto
// Protobuf C++ Version: 4.25.1

#ifndef GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2ffield_5finfo_2eproto_2epb_2eh
#define GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2ffield_5finfo_2eproto_2epb_2eh

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
#include "google/protobuf/generated_message_tctable_decl.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/metadata_lite.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/message.h"
#include "google/protobuf/repeated_field.h"  // IWYU pragma: export
#include "google/protobuf/extension_set.h"  // IWYU pragma: export
#include "google/protobuf/generated_enum_reflection.h"
#include "google/protobuf/unknown_field_set.h"
#include "google/protobuf/descriptor.pb.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"

#define PROTOBUF_INTERNAL_EXPORT_google_2fapi_2ffield_5finfo_2eproto

namespace google {
namespace protobuf {
namespace internal {
class AnyMetadata;
}  // namespace internal
}  // namespace protobuf
}  // namespace google

// Internal implementation detail -- do not use these members.
struct TableStruct_google_2fapi_2ffield_5finfo_2eproto {
  static const ::uint32_t offsets[];
};
extern const ::google::protobuf::internal::DescriptorTable
    descriptor_table_google_2fapi_2ffield_5finfo_2eproto;
namespace google {
namespace api {
class FieldInfo;
struct FieldInfoDefaultTypeInternal;
extern FieldInfoDefaultTypeInternal _FieldInfo_default_instance_;
}  // namespace api
namespace protobuf {
}  // namespace protobuf
}  // namespace google

namespace google {
namespace api {
enum FieldInfo_Format : int {
  FieldInfo_Format_FORMAT_UNSPECIFIED = 0,
  FieldInfo_Format_UUID4 = 1,
  FieldInfo_Format_IPV4 = 2,
  FieldInfo_Format_IPV6 = 3,
  FieldInfo_Format_IPV4_OR_IPV6 = 4,
  FieldInfo_Format_FieldInfo_Format_INT_MIN_SENTINEL_DO_NOT_USE_ =
      std::numeric_limits<::int32_t>::min(),
  FieldInfo_Format_FieldInfo_Format_INT_MAX_SENTINEL_DO_NOT_USE_ =
      std::numeric_limits<::int32_t>::max(),
};

bool FieldInfo_Format_IsValid(int value);
extern const uint32_t FieldInfo_Format_internal_data_[];
constexpr FieldInfo_Format FieldInfo_Format_Format_MIN = static_cast<FieldInfo_Format>(0);
constexpr FieldInfo_Format FieldInfo_Format_Format_MAX = static_cast<FieldInfo_Format>(4);
constexpr int FieldInfo_Format_Format_ARRAYSIZE = 4 + 1;
const ::google::protobuf::EnumDescriptor*
FieldInfo_Format_descriptor();
template <typename T>
const std::string& FieldInfo_Format_Name(T value) {
  static_assert(std::is_same<T, FieldInfo_Format>::value ||
                    std::is_integral<T>::value,
                "Incorrect type passed to Format_Name().");
  return FieldInfo_Format_Name(static_cast<FieldInfo_Format>(value));
}
template <>
inline const std::string& FieldInfo_Format_Name(FieldInfo_Format value) {
  return ::google::protobuf::internal::NameOfDenseEnum<FieldInfo_Format_descriptor,
                                                 0, 4>(
      static_cast<int>(value));
}
inline bool FieldInfo_Format_Parse(absl::string_view name, FieldInfo_Format* value) {
  return ::google::protobuf::internal::ParseNamedEnum<FieldInfo_Format>(
      FieldInfo_Format_descriptor(), name, value);
}

// ===================================================================


// -------------------------------------------------------------------

class FieldInfo final :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:google.api.FieldInfo) */ {
 public:
  inline FieldInfo() : FieldInfo(nullptr) {}
  ~FieldInfo() override;
  template<typename = void>
  explicit PROTOBUF_CONSTEXPR FieldInfo(::google::protobuf::internal::ConstantInitialized);

  inline FieldInfo(const FieldInfo& from)
      : FieldInfo(nullptr, from) {}
  FieldInfo(FieldInfo&& from) noexcept
    : FieldInfo() {
    *this = ::std::move(from);
  }

  inline FieldInfo& operator=(const FieldInfo& from) {
    CopyFrom(from);
    return *this;
  }
  inline FieldInfo& operator=(FieldInfo&& from) noexcept {
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
  static const FieldInfo& default_instance() {
    return *internal_default_instance();
  }
  static inline const FieldInfo* internal_default_instance() {
    return reinterpret_cast<const FieldInfo*>(
               &_FieldInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(FieldInfo& a, FieldInfo& b) {
    a.Swap(&b);
  }
  inline void Swap(FieldInfo* other) {
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
  void UnsafeArenaSwap(FieldInfo* other) {
    if (other == this) return;
    ABSL_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  FieldInfo* New(::google::protobuf::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<FieldInfo>(arena);
  }
  using ::google::protobuf::Message::CopyFrom;
  void CopyFrom(const FieldInfo& from);
  using ::google::protobuf::Message::MergeFrom;
  void MergeFrom( const FieldInfo& from) {
    FieldInfo::MergeImpl(*this, from);
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
  void InternalSwap(FieldInfo* other);

  private:
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::absl::string_view FullMessageName() {
    return "google.api.FieldInfo";
  }
  protected:
  explicit FieldInfo(::google::protobuf::Arena* arena);
  FieldInfo(::google::protobuf::Arena* arena, const FieldInfo& from);
  public:

  static const ClassData _class_data_;
  const ::google::protobuf::Message::ClassData*GetClassData() const final;

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  using Format = FieldInfo_Format;
  static constexpr Format FORMAT_UNSPECIFIED = FieldInfo_Format_FORMAT_UNSPECIFIED;
  static constexpr Format UUID4 = FieldInfo_Format_UUID4;
  static constexpr Format IPV4 = FieldInfo_Format_IPV4;
  static constexpr Format IPV6 = FieldInfo_Format_IPV6;
  static constexpr Format IPV4_OR_IPV6 = FieldInfo_Format_IPV4_OR_IPV6;
  static inline bool Format_IsValid(int value) {
    return FieldInfo_Format_IsValid(value);
  }
  static constexpr Format Format_MIN = FieldInfo_Format_Format_MIN;
  static constexpr Format Format_MAX = FieldInfo_Format_Format_MAX;
  static constexpr int Format_ARRAYSIZE = FieldInfo_Format_Format_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor* Format_descriptor() {
    return FieldInfo_Format_descriptor();
  }
  template <typename T>
  static inline const std::string& Format_Name(T value) {
    return FieldInfo_Format_Name(value);
  }
  static inline bool Format_Parse(absl::string_view name, Format* value) {
    return FieldInfo_Format_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kFormatFieldNumber = 1,
  };
  // .google.api.FieldInfo.Format format = 1 [json_name = "format"];
  void clear_format() ;
  ::google::api::FieldInfo_Format format() const;
  void set_format(::google::api::FieldInfo_Format value);

  private:
  ::google::api::FieldInfo_Format _internal_format() const;
  void _internal_set_format(::google::api::FieldInfo_Format value);

  public:
  // @@protoc_insertion_point(class_scope:google.api.FieldInfo)
 private:
  class _Internal;

  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<
      0, 1, 0,
      0, 2>
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
    int format_;
    mutable ::google::protobuf::internal::CachedSize _cached_size_;
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_google_2fapi_2ffield_5finfo_2eproto;
};

// ===================================================================



static const int kFieldInfoFieldNumber = 291403980;
extern ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::FieldOptions,
    ::google::protobuf::internal::MessageTypeTraits< ::google::api::FieldInfo >, 11, false >
  field_info;

// ===================================================================


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// FieldInfo

// .google.api.FieldInfo.Format format = 1 [json_name = "format"];
inline void FieldInfo::clear_format() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.format_ = 0;
}
inline ::google::api::FieldInfo_Format FieldInfo::format() const {
  // @@protoc_insertion_point(field_get:google.api.FieldInfo.format)
  return _internal_format();
}
inline void FieldInfo::set_format(::google::api::FieldInfo_Format value) {
  _internal_set_format(value);
  // @@protoc_insertion_point(field_set:google.api.FieldInfo.format)
}
inline ::google::api::FieldInfo_Format FieldInfo::_internal_format() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return static_cast<::google::api::FieldInfo_Format>(_impl_.format_);
}
inline void FieldInfo::_internal_set_format(::google::api::FieldInfo_Format value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.format_ = value;
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)
}  // namespace api
}  // namespace google


namespace google {
namespace protobuf {

template <>
struct is_proto_enum<::google::api::FieldInfo_Format> : std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor<::google::api::FieldInfo_Format>() {
  return ::google::api::FieldInfo_Format_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_INCLUDED_google_2fapi_2ffield_5finfo_2eproto_2epb_2eh
