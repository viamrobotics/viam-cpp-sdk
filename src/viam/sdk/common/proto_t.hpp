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

namespace impl {

struct move_may_throw {
    move_may_throw() noexcept(false) = default;
    move_may_throw& operator=(move_may_throw&&) noexcept(false) = default;
};

// Type trait for determining if move operations on ProtoValue are noexcept.
// conditional noexcept-ness of move construction on vector and unordered_map is not
// guaranteed until c++17, and even then not fully. Thus define a traits class to check whether the
// containers have nothrow move ops, and use that to determine if ProtoValue's move ops are nothrow.
struct all_proto_moves_noexcept
    : std::integral_constant<bool,
                             std::is_nothrow_move_constructible<std::vector<move_may_throw>>{} &&
                                 std::is_nothrow_move_constructible<
                                     std::unordered_map<std::string, move_may_throw>>{}> {};

}  // namespace impl

// Type erased value for storing protobuf Value types.
// The "simple" value types are empty, bool, int, double, and string.
// Moreover, a ProtoValue can be a vector or string-map of ProtoValue.
class ProtoValue {
   public:
    // Construct a null object.
    ProtoValue() noexcept;

    // Construct a nonempty object.
    template <typename T>
    ProtoValue(T t) noexcept(std::is_nothrow_move_constructible<T>{});

    // Deduction helper constructor for string from string literal
    ProtoValue(const char* str);

    // Move construction this from other, leaving other in the "unspecified but valid state" of its
    // moved-from type
    ProtoValue(ProtoValue&& other) noexcept(impl::all_proto_moves_noexcept{});

    ProtoValue(const ProtoValue& other);

    // Move assignment from other, leaving other in the "unspecified but valid state" of its
    // moved-from type.
    ProtoValue& operator=(ProtoValue&& other) noexcept(impl::all_proto_moves_noexcept{});

    ProtoValue& operator=(const ProtoValue& other);

    ~ProtoValue();

    // Test equality of two types.
    // Note that "intuitive" arithmetic equality is not supported, but could be.
    // Thus, bool{false}, int{0}, and double{0.0} do not compare equal.
    friend bool operator==(const ProtoValue& lhs, const ProtoValue& rhs);

    void swap(ProtoValue& other) noexcept(impl::all_proto_moves_noexcept{});

    // Construct from proto value
    // This method is trivially templated to insulate Value from our API/ABI.
    // In a translation unit which includes <google/protobuf/struct.pb.h> you can call
    // this function on a Value instance without specifying a template parameter and it will "just
    // work"
    template <typename Value = google::protobuf::Value>
    static ProtoValue from_proto(const Value& v);

    // Convert to protobuf Value.
    friend void to_proto(const ProtoValue& t, google::protobuf::Value* v);

    // Obtain integer constant representing the stored data type.
    int kind() const;

    // Checks whether this ProtoT is an instance of type T.
    template <typename T>
    bool is_a() const;

    // Checking cast to T, returns non-owning non-null pointer if argument is_a<T>()
    template <typename T>
    friend T* dyn_cast(ProtoValue&);

    // Checking cast to T, returns non-owning non-null pointer if argument is_a<T>()
    template <typename T>
    friend T const* dyn_cast(const ProtoValue&);

   private:
    struct vtable {
        void (*dtor)(void*);
        void (*copy)(void const*, void*);
        void (*move)(void*, void*);
        void (*to_proto)(void const*, google::protobuf::Value*);
        int (*kind)();
        bool (*equal_to)(void const*, void const*, const vtable&);
    };

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

        static constexpr vtable vtable{dtor, copy, move, to_proto, kind, equal_to};
        T data;
    };

    struct storage {
        static constexpr std::size_t local_storage_size =
            sizeof(std::unordered_map<std::string, std::string>);

        using BufType = std::aligned_storage_t<local_storage_size>;

        template <typename T>
        storage(T t) noexcept(std::is_nothrow_move_constructible<T>{});

        storage(const storage&) = delete;
        storage(storage&&) = delete;
        storage& operator=(const storage&) = delete;
        storage& operator=(storage&&) = delete;

        storage(const storage& other, const vtable& vtable);

        storage(storage&& other, const vtable& vtable) noexcept(impl::all_proto_moves_noexcept{});

        void swap(const vtable& this_vtable,
                  storage& other,
                  const vtable& other_vtable) noexcept(impl::all_proto_moves_noexcept{});

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

// The following methods are trivially templated to insulate Value and Struct from our API/ABI.
// In a translation unit which includes <google/protobuf/struct.pb.h>, you can call them without
// specifying a template parameter and it will "just work".

// Convert a type to proto value.
template <typename T, typename Value = google::protobuf::Value>
Value to_proto(T&& t) {
    Value v;
    to_proto(std::forward<T>(t), &v);

    return v;
}

// Convert a proto struct to a map.
template <typename Struct = google::protobuf::Struct>
ProtoStruct struct_to_map(const Struct& s) {
    return struct_to_map(&s);
}

// Convert map to proto struct.
template <typename Struct = google::protobuf::Struct>
Struct map_to_struct(const ProtoStruct& m) {
    Struct s;
    map_to_struct(m, &s);

    return s;
}

// ProtoT RTTI type trait.
// This type trait is used to implement the ProtoT::kind method which provides the type
// discriminator constant that is used in the casting API.
// In practice, the concept requirement for constructing a ProtoT is that this type trait be well
// formed.
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
