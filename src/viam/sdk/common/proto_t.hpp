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

// Type erased value for storing protobuf Value types.
// The "simple" value types are empty, bool, int, double, and string.
// Moreover, a ProtoT can be a vector or string-map of ProtoT.
class ProtoT {
    struct is_always_nothrow_move_constructible;

   public:
    // Construct a null object.
    ProtoT() noexcept;

    // Construct a nonempty object.
    template <typename T>
    ProtoT(T t) noexcept(std::is_nothrow_move_constructible<T>{});

    // Deduction helper constructor for string from string literal
    ProtoT(const char* str);

    // Move construction this from other, leaving other in the "unspecified but valid state" of its
    // moved-from type
    template <typename Self = ProtoT>
    ProtoT(ProtoT&& other) noexcept(typename Self::is_always_nothrow_move_constructible{});

    ProtoT(const ProtoT& other);

    // Move assignment from other, leaving other in the "unspecified but valid state" of its
    // moved-from type.
    ProtoT& operator=(ProtoT&& other);

    ProtoT& operator=(const ProtoT& other);

    ~ProtoT();

    // Test equality of two types.
    // Note that "intuitive" arithmetic equality is not supported, but could be.
    // Thus, bool{false}, int{0}, and double{0.0} do not compare equal.
    friend bool operator==(const ProtoT& lhs, const ProtoT& rhs);

    void swap(ProtoT& other);

    // Construct from proto value
    // This method is trivially templated to insulate Value from our API/ABI.
    // In a translation unit which includes <google/protobuf/struct.pb.h> you can call
    // this function on a Value instance without specifying a template parameter and it will "just
    // work"
    template <typename Value = google::protobuf::Value>
    static ProtoT from_proto(const Value& v);

    // Convert to protobuf Value.
    friend void to_proto(const ProtoT& t, google::protobuf::Value* v);

    // Obtain integer constant representing the stored data type.
    int kind() const;

    // Checks whether this ProtoT is an instance of type T.
    template <typename T>
    bool is_a() const;

    // Checking cast to T, returns non-owning non-null pointer if argument is_a<T>()
    template <typename T>
    friend T* dyn_cast(ProtoT&);

    // Checking cast to T, returns non-owning non-null pointer if argument is_a<T>()
    template <typename T>
    friend T const* dyn_cast(const ProtoT&);

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

        storage(storage&& other, const vtable& vtable);

        void swap(const vtable& this_vtable, storage& other, const vtable& other_vtable);

        void destruct(const vtable& vtable);

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

    ProtoT(const google::protobuf::Value* value);

    vtable vtable_;
    storage self_;
};

using AttrMap = std::unordered_map<std::string, ProtoT>;

struct ProtoT::is_always_nothrow_move_constructible
    : std::integral_constant<bool,
                             std::is_nothrow_move_constructible<std::vector<ProtoT>>{} &&
                                 std::is_nothrow_move_constructible<AttrMap>{}> {};

void to_proto(std::nullptr_t, google::protobuf::Value* v);
void to_proto(bool b, google::protobuf::Value* v);
void to_proto(int i, google::protobuf::Value* v);
void to_proto(double d, google::protobuf::Value* v);
void to_proto(std::string s, google::protobuf::Value* v);
void to_proto(const std::vector<ProtoT>& vec, google::protobuf::Value* v);
void to_proto(const AttrMap& m, google::protobuf::Value* v);
void to_proto(const ProtoT& t, google::protobuf::Value* v);

AttrMap struct_to_map(google::protobuf::Struct const* s);
void map_to_struct(const AttrMap& m, google::protobuf::Struct* s);

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
AttrMap struct_to_map(const Struct& s) {
    return struct_to_map(&s);
}

// Convert map to proto struct.
template <typename Struct = google::protobuf::Struct>
Struct map_to_struct(const AttrMap& m) {
    Struct s;
    map_to_struct(m, &s);

    return s;
}

// Type trait for constant value of each kind.
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
struct kind_t<std::vector<ProtoT>> : std::integral_constant<int, 5> {};

template <>
struct kind_t<AttrMap> : std::integral_constant<int, 6> {};

template <typename T>
bool ProtoT::is_a() const {
    return kind_t<T>::value == kind();
}

template <typename T>
T* dyn_cast(ProtoT& pt) {
    static_assert(!std::is_same<T, std::nullptr_t>{}, "Please do not dyn_cast to nullptr");
    if (pt.is_a<T>()) {
        return pt.self_.template get<T>();
    }

    return nullptr;
}

template <typename T>
T const* dyn_cast(const ProtoT& pt) {
    static_assert(!std::is_same<T, std::nullptr_t>{}, "Please do not dyn_cast to nullptr");
    if (pt.is_a<T>()) {
        return pt.self_.template get<T>();
    }

    return nullptr;
}

}  // namespace sdk
}  // namespace viam
