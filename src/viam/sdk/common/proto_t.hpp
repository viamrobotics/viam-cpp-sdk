#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace google {
namespace protobuf {
class Value;
}
}  // namespace google

namespace viam {
namespace sdk {

// Type erased value for storing protobuf Value types.
// The "simple" value types are empty, bool, int, double, and string.
// Moreover, a ProtoT can be a vector or string-map of ProtoT.
class ProtoT {
   public:
    // Construct an empty object.
    ProtoT() noexcept = default;

    // Explicitly construct an empty object from nullptr
    ProtoT(std::nullptr_t) : ProtoT() {}

    // Construct a nonempty object.
    template <typename T>
    ProtoT(T t) : self_(std::make_unique<model<T>>(std::move(t))) {}

    // Deduction helper constructor for string from string literal
    ProtoT(const char* str) : ProtoT(std::string(str)) {}

    // Move construction this from other, leaving other as a valid nullptr value
    ProtoT(ProtoT&& other) noexcept : self_(std::exchange(other.self_, holder{})) {}

    ProtoT(const ProtoT& other) = default;

    // Move assignment from other, leaving other as a valid nullptr value
    ProtoT& operator=(ProtoT&& other) noexcept {
        self_ = std::exchange(other.self_, holder{});
        return *this;
    }

    ProtoT& operator=(const ProtoT& other) = default;

    ~ProtoT() = default;

    // Test equality of two types.
    // Note that "intuitive" arithmetic equality is not supported, but could be.
    // Thus, bool{false}, int{0}, and double{0.0} do not compare equal.
    friend bool operator==(const ProtoT& lhs, const ProtoT& rhs) {
        return lhs.self_.equal_to(rhs.self_);
    }

    // Construct from proto value
    template <typename Value = google::protobuf::Value>
    static ProtoT from_proto_value(const Value& v) {
        return ProtoT(&v);
    }

    // Convert to protobuf Value.
    friend void to_proto_value(const ProtoT& t, google::protobuf::Value* v);

    // Obtain integer constant representing the stored data type.
    int kind() const {
        return self_.kind();
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
    // ABC interface class for type erasure.
    struct concept_t {
        virtual ~concept_t() = default;

        virtual std::unique_ptr<concept_t> copy() const = 0;

        virtual void to_proto_value(google::protobuf::Value* v) const = 0;

        virtual int kind() const = 0;

        virtual bool equal_to(const concept_t& other) const = 0;
    };

    template <typename T>
    struct model;

    // Concrete model of interface.
    template <typename T>
    struct model final : concept_t {
        model(T t) : data(std::move(t)) {}
        ~model() override = default;

        std::unique_ptr<concept_t> copy() const override {
            return std::make_unique<model>(*this);
        }

        void to_proto_value(google::protobuf::Value*) const override;

        int kind() const override;

        virtual bool equal_to(const concept_t& other) const override {
            if (this->kind() != other.kind()) {
                return false;
            }

            return data == static_cast<const model<T>&>(other).data;
        }

        T data;
    };

    struct holder {
        holder() = default;

        template <typename T>
        holder(std::unique_ptr<model<T>> p) : ptr(std::move(p)) {
            static_assert(!std::is_same<T, std::nullptr_t>{}, "Use default ctor for nullptr value");
        }

        holder(holder&&) = default;

        holder(const holder& other)
            : ptr(other.ptr ? other.ptr->copy() : std::unique_ptr<concept_t>{}) {}

        holder& operator=(holder&&) = default;

        holder& operator=(const holder& other) {
            ptr = other.ptr ? other.ptr->copy() : std::unique_ptr<concept_t>{};
            return *this;
        }

        void to_proto_value(google::protobuf::Value*) const;

        int kind() const;

        bool equal_to(const holder& other) const;

        std::unique_ptr<concept_t> ptr;
    };

    ProtoT(const google::protobuf::Value* value);

    // TODO this could be replaced with an SBO storage class to save a heap allocation on small,
    // trivially copyable types. Pending that, holder special-cases the nullptr_t value to store
    // nothing. This gives us noexcept default construction and move operations, and allows move ops
    // to leave the object in a null state.
    holder self_;
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

template <class T, class Value = google::protobuf::Value>
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
void ProtoT::model<T>::to_proto_value(google::protobuf::Value* v) const {
    viam::sdk::to_proto_value(data, v);
}

template <typename T>
int ProtoT::model<T>::kind() const {
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
        return &(static_cast<ProtoT::model<T>*>(pt.self_.ptr.get())->data);
    }

    return nullptr;
}

template <typename T>
T const* dyn_cast(const ProtoT& pt) {
    static_assert(!std::is_same<T, std::nullptr_t>{}, "Please do not dyn_cast to nullptr");
    if (pt.is_a<T>()) {
        return &(static_cast<const ProtoT::model<T>*>(pt.self_.ptr.get())->data);
    }

    return nullptr;
}

}  // namespace sdk
}  // namespace viam
