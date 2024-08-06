#pragma once

#include <memory>
#include <string>
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
   public:
    // Construct a null object.
    ProtoT() noexcept : ProtoT(nullptr) {}

    // Construct a nonempty object.
    template <typename T>
    ProtoT(T t) : vtable_{model_t<T>::vtable}, self_{std::move(t)} {}

    // Deduction helper constructor for string from string literal
    ProtoT(const char* str) : ProtoT(std::string(str)) {}

    // Move construction this from other, leaving other in the "unspecified but valid state" of its
    // moved-from type
    ProtoT(ProtoT&& other) noexcept
        : vtable_(std::move(other.vtable_)), self_(std::move(other.self_), vtable_) {}

    ProtoT(const ProtoT& other) : vtable_(other.vtable_), self_(other.self_, other.vtable_) {}

    // Move assignment from other, leaving other in the "unspecified but valid state" of its
    // moved-from type.
    ProtoT& operator=(ProtoT&& other) noexcept {
        ProtoT(std::move(other)).swap(*this);
        return *this;
    }

    ProtoT& operator=(const ProtoT& other) {
        ProtoT(other).swap(*this);
        return *this;
    }

    ~ProtoT() {
        self_.destruct(vtable_);
    }

    // Test equality of two types.
    // Note that "intuitive" arithmetic equality is not supported, but could be.
    // Thus, bool{false}, int{0}, and double{0.0} do not compare equal.
    friend bool operator==(const ProtoT& lhs, const ProtoT& rhs) {
        return lhs.vtable_.equal_to(lhs.self_.get(), rhs.self_.get(), rhs.vtable_);
    }

    void swap(ProtoT& other) {
        self_.swap(vtable_, other.self_, other.vtable_);
        std::swap(vtable_, other.vtable_);
    }

    // Construct from proto value
    // This method is trivially templated to insulate Value from our API/ABI.
    // In a translation unit which includes <google/protobuf/struct.pb.h> you can call
    // this function on a Value instance without specifying a template parameter and it will "just
    // work"
    template <typename Value = google::protobuf::Value>
    static ProtoT from_proto_value(const Value& v) {
        return ProtoT(&v);
    }

    // Convert to protobuf Value.
    friend void to_proto_value(const ProtoT& t, google::protobuf::Value* v);

    // Obtain integer constant representing the stored data type.
    int kind() const {
        return vtable_.kind();
    }

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
    struct vtable_t {
        void (*dtor)(void*) noexcept;
        void (*copy)(void const*, void*);
        void (*move)(void*, void*) noexcept;
        void (*to_proto_value)(void const*, google::protobuf::Value*);
        int (*kind)() noexcept;
        bool (*equal_to)(void const*, void const*, const vtable_t&);
    };

    template <typename T>
    struct model_t {
        model_t(T t) : data(std::move(t)) {}

        static void dtor(void* self) noexcept {
            static_cast<model_t*>(self)->~model_t();
        }

        static void copy(void const* self, void* dest) {
            new (dest) model_t(*static_cast<model_t const*>(self));
        }

        static void move(void* self, void* dest) noexcept {
            new (dest) model_t(std::move(*static_cast<model_t*>(self)));
        }

        static void to_proto_value(void const* self, google::protobuf::Value* v);

        static int kind() noexcept;

        static bool equal_to(void const* self, void const* other, const vtable_t& other_vtable) {
            if (model_t::kind() != other_vtable.kind()) {
                return false;
            }

            return *static_cast<T const*>(self) == *static_cast<T const*>(other);
        }

        static constexpr vtable_t vtable{dtor, copy, move, to_proto_value, kind, equal_to};
        T data;
    };

    struct storage_t {
        static constexpr std::size_t small_size =
            sizeof(std::unordered_map<std::string, std::string>);
        template <typename T>
        storage_t(T t) {
            static_assert(sizeof(model_t<T>) <= small_size, "Too big!");
            new (&buf_) model_t<T>(std::move(t));
        }

        storage_t(const storage_t&) = delete;
        storage_t(storage_t&&) = delete;
        storage_t& operator=(const storage_t&) = delete;
        storage_t& operator=(storage_t&&) = delete;

        storage_t(const storage_t& other, const vtable_t& vtable) {
            vtable.copy(other.get(), this->get());
        }

        storage_t(storage_t&& other, const vtable_t& vtable) {
            vtable.move(other.get(), this->get());
        }

        void swap(const vtable_t& this_vtable, storage_t& other, const vtable_t& other_vtable) {
            if (this == &other) {
                return;
            }

            unsigned char tmp[small_size];
            other_vtable.move(other.get(), &tmp);
            other_vtable.dtor(other.get());

            this_vtable.move(this->get(), other.get());
            this_vtable.dtor(this->get());

            other_vtable.move(&tmp, this->get());
            other_vtable.dtor(&tmp);
        }

        void destruct(const vtable_t& vtable) {
            vtable.dtor(this->get());
        }

        template <typename T = void>
        T* get() {
            return static_cast<T*>(static_cast<void*>(&buf_));
        }

        template <typename T = void>
        T const* get() const {
            return static_cast<T const*>(static_cast<void const*>(&buf_));
        }

        unsigned char buf_[small_size];
    };

    ProtoT(const google::protobuf::Value* value);

    vtable_t vtable_;
    storage_t self_;
};

using AttrMap = std::unordered_map<std::string, ProtoT>;

void to_proto_value(std::nullptr_t, google::protobuf::Value* v);
void to_proto_value(bool b, google::protobuf::Value* v);
void to_proto_value(int i, google::protobuf::Value* v);
void to_proto_value(double d, google::protobuf::Value* v);
void to_proto_value(std::string s, google::protobuf::Value* v);
void to_proto_value(const std::vector<ProtoT>& vec, google::protobuf::Value* v);
void to_proto_value(const AttrMap& m, google::protobuf::Value* v);
void to_proto_value(const ProtoT& t, google::protobuf::Value* v);

AttrMap struct_to_map(google::protobuf::Struct const* s);
void map_to_struct(const AttrMap& m, google::protobuf::Struct* s);

// Convert map to proto struct.
// This method is trivially templated to insulate Value from our API/ABI.
// In a translation unit which includes <google/protobuf/struct.pb.h> you can call
// this function to create a Value instance without specifying a template parameter and it will
// "just work"
template <typename Struct = google::protobuf::Struct>
Struct map_to_struct(const AttrMap& m) {
    Struct s;
    map_to_struct(m, &s);

    return s;
}

template <typename Struct = google::protobuf::Struct>
AttrMap struct_to_map(const Struct& s) {
    return struct_to_map(&s);
}

// Convert a type to proto value.
// This method is trivially templated to insulate Value from our API/ABI.
// In a translation unit which includes <google/protobuf/struct.pb.h> you can call
// this function to create a Value instance without specifying a template parameter and it will
// "just work"
template <typename T, typename Value = google::protobuf::Value>
Value to_proto_value(T&& t) {
    Value v;
    to_proto_value(std::forward<T>(t), &v);

    return v;
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
void ProtoT::model_t<T>::to_proto_value(void const* self, google::protobuf::Value* v) {
    viam::sdk::to_proto_value(static_cast<model_t const*>(self)->data, v);
}

template <typename T>
int ProtoT::model_t<T>::kind() noexcept {
    return kind_t<T>::value;
}

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
