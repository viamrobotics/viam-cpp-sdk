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

namespace proto_value_details {

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
struct all_moves_noexcept
    : std::integral_constant<bool,
                             std::is_nothrow_move_constructible<std::vector<move_may_throw>>{} &&
                                 std::is_nothrow_move_constructible<
                                     std::unordered_map<std::string, move_may_throw>>{}> {};

}  // namespace proto_value_details

/// @brief Type-erased value for storing google::protobuf::Value types.
/// A ProtoValue can be nullptr, bool, int, double, std::string, or, recursively, a vector or
/// string-map of ProtoValue.
/// This type is used at API/ABI boundaries for interfacing with grpc/proto code.
/// @remark The (special) member function templates below all operate on a closed subset of types,
/// so we provide explicit instantiations for all valid template types. See below the class
/// definition.
class ProtoValue {
   public:
    /// @brief Type discriminator constants for possible values stored in a ProtoValue.
    enum Kind { k_null = 0, k_bool = 1, k_double = 2, k_string = 3, k_list = 4, k_struct = 5 };

    /// @brief Construct a null object.
    ProtoValue() noexcept;

    /// @name Value constructors.
    /// @brief Constructors which initialize a ProtoValue holding its argument.
    /// @{

    ProtoValue(std::nullptr_t) noexcept;

    ProtoValue(bool b) noexcept;

    /// @brief Construct a double object upcast from constructor argument.
    ProtoValue(int i) noexcept;

    ProtoValue(double d) noexcept;

    ProtoValue(std::string s) noexcept;

    /// @brief Deduction helper constructor for string from string literal
    ProtoValue(const char* str);

    /// @brief Construct from a ProtoList.
    template <typename Val = ProtoValue,
              typename = std::enable_if_t<std::is_same<Val, ProtoValue>{}>>
    ProtoValue(std::vector<Val>) noexcept(std::is_nothrow_move_constructible<std::vector<Val>>{});

    /// @brief Construct from a ProtoStruct.
    template <typename Val = ProtoValue,
              typename = std::enable_if_t<std::is_same<Val, ProtoValue>{}>>
    ProtoValue(std::unordered_map<std::string, Val>) noexcept(
        std::is_nothrow_move_constructible<std::unordered_map<std::string, Val>>{});

    /// @}

    /// @brief Move construct this from other, leaving other in its unspecified-but-valid moved from
    /// state.
    ProtoValue(ProtoValue&& other) noexcept(proto_value_details::all_moves_noexcept{});

    ProtoValue(const ProtoValue& other);

    /// @brief Move assignment from other, leaving other in its unspecified-but-valid moved from
    /// state.
    ProtoValue& operator=(ProtoValue&& other) noexcept(proto_value_details::all_moves_noexcept{});

    ProtoValue& operator=(const ProtoValue& other);

    ~ProtoValue();

    /// @brief Test equality of two types.
    /// @note "Intuitive" arithmetic equality is not supported, but could be.
    /// Thus, bool{false}, int{0}, and double{0.0} do not compare equal.
    friend bool operator==(const ProtoValue& lhs, const ProtoValue& rhs);

    void swap(ProtoValue& other) noexcept(proto_value_details::all_moves_noexcept{});

    /// @brief Construct from proto value
    /// @note This method is trivially templated to insulate google::protobuf::Value from our
    /// API/ABI. It is meant to be called with no template parameters in a translation unit which
    /// includes <google/protobuf/struct.pb.h>
    template <typename Value = google::protobuf::Value>
    static ProtoValue from_proto(const Value& v);  // NOLINT(misc-no-recursion)

    friend void to_proto(const ProtoValue& t, google::protobuf::Value* v);

    /// @name Value access API
    ///@{

    /// @brief Obtain enumerator constant representing the stored data type.
    Kind kind() const;

    /// @brief Checks whether this ProtoT is an instance of type T.
    template <typename T>
    bool is_a() const;

    /// @brief Convenience version of is_a<T> to check if the value is nullptr.
    bool is_null() const;

    /// @brief Return a T pointer if this is_a<T>(), else return nullptr
    template <typename T>
    T* get();

    /// @brief Return a T pointer if this is_a<T>(), else return nullptr
    template <typename T>
    T const* get() const;

    /// @brief Return a reference to the underlying T, without checking.
    /// @tparam T a bool or double
    template <typename T>
    std::enable_if_t<std::is_scalar<T>{}, T&> get_unchecked();

    /// @brief Return the underlying T by value, without checking.
    /// @tparam T a bool or double.
    template <typename T>
    std::enable_if_t<std::is_scalar<T>{}, T> get_unchecked() const;

    /// @brief Return a mutable reference to the underlying T, without checking
    /// @tparam T a std::string, ProtoList, or ProtoStruct.
    template <typename T>
    std::enable_if_t<!std::is_scalar<T>{}, T&> get_unchecked() &;

    /// @brief Return an immutable reference to the underlying T, without checking.
    /// @tparam T a std::string, ProtoList, or ProtoStruct.
    template <typename T>
    std::enable_if_t<!std::is_scalar<T>{}, T const&> get_unchecked() const&;

    /// @brief Return an rvalue reference to the underlying T, without checking.
    /// @tparam T a std::string, ProtoList, or ProtoStruct.
    template <typename T>
    std::enable_if_t<!std::is_scalar<T>{}, T&&> get_unchecked() &&;

    ///@}

   private:
    // This struct is our implementation of a virtual table, similar to what is created by the
    // compiler for polymorphic types. We can't use actual polymorphic types because the
    // implementation uses aligned stack storage, so we DIY it insted.
    // The vtable can be thought of as defining a concept or interface which our ProtoValue
    // types must satisfy. The first void [const]* parameter in any of the pointers below is
    // always the `this` or `self` pointer.
    struct vtable {
        void (*dtor)(void*);
        void (*copy)(void const*, void*);
        void (*move)(void*, void*);
        void (*to_proto)(void const*, google::protobuf::Value*);
        Kind (*kind)();
        bool (*equal_to)(void const*, void const*, const vtable&);
    };

    // Class template for a concrete model of the concept described by vtable.
    // All construction and equality operations are implemented using defaults of the stored T.
    template <typename T>
    struct model {
        static_assert(std::is_nothrow_destructible<T>{}, "T has a throwing destructor");

        model(T t) noexcept(std::is_nothrow_move_constructible<T>{});

        static void dtor(void* self) noexcept;

        static void copy(void const* self, void* dest);

        // Clang bug prevents us from marking this noexcept but it's being called through a
        // non-noexcept pointer anyway
        static void move(void* self, void* dest);

        static void to_proto(void const* self, google::protobuf::Value* v);

        static Kind kind() noexcept;

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
        // stand-ins for ProtoList and ProtoStruct which are not available until end
        // of class definition.
        using BufType = std::aligned_union_t<0,
                                             std::nullptr_t,
                                             bool,
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
        storage(storage&& other,
                const vtable& vtable) noexcept(proto_value_details::all_moves_noexcept{});

        // Swap this storage with other, with operations on this provided by this_vtable, and
        // operations on other provided by other_vtable.
        void swap(const vtable& this_vtable,
                  storage& other,
                  const vtable& other_vtable) noexcept(proto_value_details::all_moves_noexcept{});

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

    // Helper template for the explicit versions above.
    // Includes nullptr_t as a tag type so we can let the other constructors delegate.
    template <typename T>
    ProtoValue(T t, std::nullptr_t) noexcept(std::is_nothrow_move_constructible<T>{});

    vtable vtable_;
    storage self_;
};

// Pre c++17 this is still required
template <typename T>
constexpr ProtoValue::vtable ProtoValue::model<T>::vtable_;

/// @brief Alias declaration for container of repeated ProtoValue, representing a
/// google::protobuf::ListValue.
using ProtoList = std::vector<ProtoValue>;

/// @brief Alias declaration for map of string to type-erased ProtoValue representing
/// google::protobuf::Struct.
/// This stores structured data as a map where the keys can be thought of as member names.
using ProtoStruct = std::unordered_map<std::string, ProtoValue>;

// -- Template specialization declarations of by-value constructors -- //
extern template ProtoValue::ProtoValue(ProtoList) noexcept(
    std::is_nothrow_move_constructible<ProtoList>{});
extern template ProtoValue::ProtoValue(ProtoStruct m) noexcept(
    std::is_nothrow_move_constructible<ProtoStruct>{});

// -- Template specialization declarations of get_unchecked: POD types -- //
extern template bool& ProtoValue::get_unchecked<bool>();
extern template double& ProtoValue::get_unchecked<double>();

extern template bool ProtoValue::get_unchecked<bool>() const;
extern template double ProtoValue::get_unchecked<double>() const;

// -- Template specialization declarations of get_unchecked: string and recursive types -- //
extern template std::string& ProtoValue::get_unchecked<std::string>() &;
extern template ProtoList& ProtoValue::get_unchecked<ProtoList>() &;
extern template ProtoStruct& ProtoValue::get_unchecked<ProtoStruct>() &;

extern template std::string const& ProtoValue::get_unchecked<std::string>() const&;
extern template ProtoList const& ProtoValue::get_unchecked<ProtoList>() const&;
extern template ProtoStruct const& ProtoValue::get_unchecked<ProtoStruct>() const&;

extern template std::string&& ProtoValue::get_unchecked<std::string>() &&;
extern template ProtoList&& ProtoValue::get_unchecked<ProtoList>() &&;
extern template ProtoStruct&& ProtoValue::get_unchecked<ProtoStruct>() &&;

void to_proto(std::nullptr_t, google::protobuf::Value* v);
void to_proto(bool b, google::protobuf::Value* v);
void to_proto(double d, google::protobuf::Value* v);
void to_proto(std::string s, google::protobuf::Value* v);
void to_proto(const ProtoList& vec, google::protobuf::Value* v);
void to_proto(const ProtoStruct& m, google::protobuf::Value* v);
void to_proto(const ProtoValue& t, google::protobuf::Value* v);

void struct_to_map(google::protobuf::Struct const* s, ProtoStruct& m);
void map_to_struct(const ProtoStruct& m, google::protobuf::Struct* s);

/// @brief Convert a type to a google::protobuf::Value.
/// @note This method is trivially templated to insulate google::protobuf::Value from our
/// API/ABI. It is meant to be called with no template parameters in a translation unit which
/// includes <google/protobuf/struct.pb.h>
template <typename Value = google::protobuf::Value>
Value to_proto(const ProtoValue& pv) {
    Value v;
    to_proto(pv, &v);

    return v;
}

/// @brief Convert a google::protobuf::Struct to a ProtoStruct.
/// @note This method is trivially templated to insulate google::protobuf::Struct from our
/// API/ABI. It is meant to be called with no template parameters in a translation unit which
/// includes <google/protobuf/struct.pb.h>
template <typename Struct = google::protobuf::Struct>
ProtoStruct struct_to_map(const Struct& s) {
    ProtoStruct result;
    struct_to_map(&s, result);

    return result;
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

namespace proto_value_details {

template <typename T>
struct kind {};

template <ProtoValue::Kind k>
using KindConstant = std::integral_constant<ProtoValue::Kind, k>;

template <>
struct kind<std::nullptr_t> {
    using type = KindConstant<ProtoValue::Kind::k_null>;
};

template <>
struct kind<bool> {
    using type = KindConstant<ProtoValue::Kind::k_bool>;
};

template <>
struct kind<double> {
    using type = KindConstant<ProtoValue::Kind::k_double>;
};

template <>
struct kind<std::string> {
    using type = KindConstant<ProtoValue::Kind::k_string>;
};

template <>
struct kind<ProtoList> {
    using type = KindConstant<ProtoValue::Kind::k_list>;
};

template <>
struct kind<ProtoStruct> {
    using type = KindConstant<ProtoValue::Kind::k_struct>;
};

}  // namespace proto_value_details

template <typename T>
bool ProtoValue::is_a() const {
    return proto_value_details::kind<T>::type::value == kind();
}

template <typename T>
T* ProtoValue::get() {
    if (this->is_a<T>()) {
        return this->self_.template get<T>();
    }

    return nullptr;
}

template <typename T>
T const* ProtoValue::get() const {
    if (this->is_a<T>()) {
        return this->self_.template get<T>();
    }

    return nullptr;
}

}  // namespace sdk
}  // namespace viam
