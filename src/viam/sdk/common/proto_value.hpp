#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace google {
namespace protobuf {

// Forward declaration of google::protobuf Value and Struct.
// The class below is written so as to keep Value out of the ABI, and as such can be instantiated
// with Value as an incomplete type.

class Value;
class Struct;
}  // namespace protobuf
}  // namespace google

namespace viam {
namespace sdk {

namespace detail {

struct move_may_throw {
    move_may_throw(move_may_throw&&) noexcept(false) = default;
    move_may_throw& operator=(move_may_throw&&) noexcept(false) = default;
};

// Type trait for determining if move operations on ProtoValue are noexcept.
// Conditional noexcept-ness of move construction on vector and unordered_map is not
// guaranteed until c++17, and even then not fully. Thus define a traits class to check whether the
// containers have nothrow move ops, and use that to determine if ProtoValue's move ops are nothrow.
// We use move_may_throw as an illustrative dummy class since, perhaps surprisingly, nothrow
// movability of vector and unordered_map doesn't depend on their stored type.
struct all_proto_moves_noexcept
    : std::integral_constant<bool,
                             std::is_nothrow_move_constructible<std::vector<move_may_throw>>{} &&
                                 std::is_nothrow_move_constructible<
                                     std::unordered_map<std::string, move_may_throw>>{}> {};

}  // namespace detail

/// @brief Type-erased value for storing google::protobuf::Value types.
/// A ProtoValue can be nullptr, bool, int, double, std::string, or, recursively, a vector or
/// string-map of ProtoValue.
/// This type is used at API/ABI boundaries for interfacing with grpc/proto code.
class ProtoValue {
   public:
    /// @brief Construct a null object.
    ProtoValue() noexcept;

    /// @brief Construct a nonempty object.
    template <typename T>
    ProtoValue(T t) noexcept(std::is_nothrow_move_constructible<T>{});

    /// @brief Deduction helper constructor for string from string literal
    ProtoValue(const char* str);

    /// @brief Move construct this from other, leaving other in its unspecified-but-valid moved from
    /// state.
    ProtoValue(ProtoValue&& other) noexcept(detail::all_proto_moves_noexcept{});

    ProtoValue(const ProtoValue& other);

    /// @brief Move assignment from other, leaving other in its unspecified-but-valid moved from
    /// state.
    ProtoValue& operator=(ProtoValue&& other) noexcept(detail::all_proto_moves_noexcept{});

    ProtoValue& operator=(const ProtoValue& other);

    ~ProtoValue();

    /// @brief Test equality of two types.
    /// @note "Intuitive" arithmetic equality is not supported, but could be.
    /// Thus, bool{false}, int{0}, and double{0.0} do not compare equal.
    friend bool operator==(const ProtoValue& lhs, const ProtoValue& rhs);

    void swap(ProtoValue& other) noexcept(detail::all_proto_moves_noexcept{});

    /// @brief Construct from proto value
    /// @note This method is trivially templated to insulate google::protobuf::Value from our
    /// API/ABI. It is meant to be called with no template parameters in a translation unit which
    /// includes <google/protobuf/struct.pb.h>
    template <typename Value = google::protobuf::Value>
    static ProtoValue from_proto(const Value& v);  // NOLINT(misc-no-recursion)

    friend void to_proto(const ProtoValue& t, google::protobuf::Value* v);

    /// @name Casting API
    ///@{

    /// @brief Obtain integer constant representing the stored data type.
    int kind() const;

    /// @brief Checks whether this ProtoT is an instance of type T.
    template <typename T>
    bool is_a() const;

    /// @brief Checking cast to T, returns non-owning non-null pointer if argument is_a<T>()
    template <typename T>
    friend T* dyn_cast(ProtoValue&);

    /// @brief Checking cast to T, returns non-owning non-null pointer if argument is_a<T>()
    template <typename T>
    friend T const* dyn_cast(const ProtoValue&);

    ///@}

   private:
    // This struct is our implementation of a virtual table, similar to what is created by the
    // compiler for polymorphic types. We can't use actual polymorphic types because the
    // implementation uses aligned stack storage, so we DIY it insted.
    // The vtable can be thought of as defining a concept or interface which our ProtoValue types
    // must satisfy.
    // The first void [const]* parameter in any of the pointers below is always the `this` or `self`
    // pointer.
    struct vtable {
        void (*dtor)(void*);
        void (*copy)(void const*, void*);
        void (*move)(void*, void*);
        void (*to_proto)(void const*, google::protobuf::Value*);
        int (*kind)();
        bool (*equal_to)(void const*, void const*, const vtable&);
    };

    // Class template for a concrete model of the concept described by vtable.
    // All construction and equality operations are implemented using defaults of the stored T.
    template <typename T>
    struct model {
        model(T t) noexcept(std::is_nothrow_move_constructible<T>{});

        static void dtor(void* self) noexcept;

        static void copy(void const* self, void* dest);

        // Clang bug prevents us from marking this noexcept but it's being called through a
        // non-noexcept pointer anyway
        static void move(void* self, void* dest);

        static void to_proto(void const* self, google::protobuf::Value* v);

        static int kind() noexcept;

        static bool equal_to(void const* self, void const* other, const vtable& other_vtable);

        static constexpr vtable vtable_{dtor, copy, move, to_proto, kind, equal_to};
        T data;
    };

    // Stack-based storage for instances of model<T>.
    // This is an RAII class which manages lifetime, pointer access, and construction of a
    // type-erased model<T>. Note that all lifetime operations require a vtable to which the
    // implementation defers.
    struct storage {
        // Local storage in an aligned_union which can hold all the possible ProtoValue types, using
        // stand-ins for std::vector<ProtoValue> and ProtoStruct which are not available until end
        // of class definition.
        using BufType = std::aligned_union_t<0,
                                             std::nullptr_t,
                                             bool,
                                             int,
                                             double,
                                             std::string,
                                             std::vector<void*>,
                                             std::unordered_map<std::string, void*>>;

        static constexpr std::size_t local_storage_size = sizeof(BufType);
        static constexpr std::size_t local_storage_alignment = alignof(BufType);

        // Construct this to store a model<T>.
        template <typename T>
        storage(T t) noexcept(std::is_nothrow_move_constructible<T>{});

        // These default special member functions are deleted in favor of overloads below which take
        // a vtable parameter.
        // We need the vtable because otherwise we have no way to perform the requisite copy, move,
        // or destructor operations.
        storage(const storage&) = delete;
        storage(storage&&) = delete;
        storage& operator=(const storage&) = delete;
        storage& operator=(storage&&) = delete;

        // Copy construct this storage from other, using the copy operation in vtable.
        storage(const storage& other, const vtable& vtable);

        // Move construct this storage from other, using the move operation in vtable.
        storage(storage&& other, const vtable& vtable) noexcept(detail::all_proto_moves_noexcept{});

        // Swap this storage with other, with operations on this provided by this_vtable, and
        // operations on other provided by other_vtable.
        void swap(const vtable& this_vtable,
                  storage& other,
                  const vtable& other_vtable) noexcept(detail::all_proto_moves_noexcept{});

        // Destroy this storage, using the destructor from vtable.
        void destruct(const vtable& vtable) noexcept;

        template <typename T = void>
        T* get() {
            return static_cast<T*>(static_cast<void*>(&buf_));
        }

        template <typename T = void>
        T const* get() const {
            return static_cast<T const*>(static_cast<void const*>(&buf_));
        }

        BufType buf_;
    };

    ProtoValue(const google::protobuf::Value* value);

    vtable vtable_;
    storage self_;
};

/// @brief Alias declaration for map of string to type-erased ProtoValue representing
/// google::protobuf::Struct.
/// This stores structured data as a map where the keys can be thought of as member names.
using ProtoStruct = std::unordered_map<std::string, ProtoValue>;

void to_proto(std::nullptr_t, google::protobuf::Value* v);
void to_proto(bool b, google::protobuf::Value* v);
void to_proto(int i, google::protobuf::Value* v);
void to_proto(double d, google::protobuf::Value* v);
void to_proto(std::string s, google::protobuf::Value* v);
void to_proto(const std::vector<ProtoValue>& vec, google::protobuf::Value* v);
void to_proto(const ProtoStruct& m, google::protobuf::Value* v);
void to_proto(const ProtoValue& t, google::protobuf::Value* v);

ProtoStruct struct_to_map(google::protobuf::Struct const* s);
void map_to_struct(const ProtoStruct& m, google::protobuf::Struct* s);

/// @brief Convert a type to a google::protobuf::Value.
/// @note This method is trivially templated to insulate google::protobuf::Value from our
/// API/ABI. It is meant to be called with no template parameters in a translation unit which
/// includes <google/protobuf/struct.pb.h>
template <typename T, typename Value = google::protobuf::Value>
Value to_proto(T&& t) {
    Value v;
    to_proto(std::forward<T>(t), &v);

    return v;
}

/// @brief Convert a google::protobuf::Struct to a ProtoStruct.
/// @note This method is trivially templated to insulate google::protobuf::Struct from our
/// API/ABI. It is meant to be called with no template parameters in a translation unit which
/// includes <google/protobuf/struct.pb.h>
template <typename Struct = google::protobuf::Struct>
ProtoStruct struct_to_map(const Struct& s) {
    return struct_to_map(&s);
}

/// @brief Convert a ProtoStruct to a google::protobuf::Struct.
/// @note This method is trivially templated to insulate google::protobuf::Struct from our
/// API/ABI. It is meant to be called with no template parameters in a translation unit which
/// includes <google/protobuf/struct.pb.h>
template <typename Struct = google::protobuf::Struct>
Struct map_to_struct(const ProtoStruct& m) {
    Struct s;
    map_to_struct(m, &s);

    return s;
}

/// @brief ProtoValue RTTI type trait.
/// This type trait is used to implement the ProtoValue::kind method which provides the type
/// discriminator constant that is used in the casting API.
/// A ProtoValue can only be constructed from types for which this trait is well formed.
template <typename T>
struct kind_t;

template <>
struct kind_t<std::nullptr_t> : std::integral_constant<int, 0> {};

template <>
struct kind_t<bool> : std::integral_constant<int, 1> {};

template <>
struct kind_t<int> : std::integral_constant<int, 2> {};

template <>
struct kind_t<double> : std::integral_constant<int, 3> {};

template <>
struct kind_t<std::string> : std::integral_constant<int, 4> {};

template <>
struct kind_t<std::vector<ProtoValue>> : std::integral_constant<int, 5> {};

template <>
struct kind_t<ProtoStruct> : std::integral_constant<int, 6> {};

template <typename T>
bool ProtoValue::is_a() const {
    return kind_t<T>::value == kind();
}

template <typename T>
T* dyn_cast(ProtoValue& pt) {
    static_assert(!std::is_same<T, std::nullptr_t>{}, "Please do not dyn_cast to nullptr");
    if (pt.is_a<T>()) {
        return pt.self_.template get<T>();
    }

    return nullptr;
}

template <typename T>
T const* dyn_cast(const ProtoValue& pt) {
    static_assert(!std::is_same<T, std::nullptr_t>{}, "Please do not dyn_cast to nullptr");
    if (pt.is_a<T>()) {
        return pt.self_.template get<T>();
    }

    return nullptr;
}

}  // namespace sdk
}  // namespace viam
