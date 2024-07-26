#pragma once

#include <unordered_map>

#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

// Type erased value for storing protobuf Value types.
// The "simple" value types are empty, bool, int, double, and string.
// Moreover, a ProtoT can be a vector or string-map of ProtoT.
class ProtoT {
   public:
    // Construct an empty object.
    ProtoT() : ProtoT(nullptr) {}

    // Construct a nonempty object.
    template <typename T>
    ProtoT(T t) : self_(std::make_unique<model<T>>(std::move(t))) {}

    // Deduction helper constructor for string from string literal
    ProtoT(const char* str) : ProtoT(std::string(str)) {}

    // Create from proto value
    ProtoT(const google::protobuf::Value&);

    ProtoT(ProtoT&&) noexcept = default;
    ProtoT(const ProtoT& other) : self_(other.self_->copy()) {}

    ProtoT& operator=(ProtoT&&) noexcept = default;
    ProtoT& operator=(const ProtoT& other) {
        self_ = other.self_->copy();
        return *this;
    }

    ~ProtoT() = default;

    // Test equality of two types.
    // Note that "intuitive" arithmetic equality is not supported, but could be.
    // Thus, bool{false}, int{0}, and double{0.0} do not compare equal.
    friend bool operator==(const ProtoT& lhs, const ProtoT& rhs) {
        return lhs.self_->equal_to(*rhs.self_);
    }

    // Convert to protobuf Value.
    friend google::protobuf::Value to_proto_value(const ProtoT& t) {
        return t.self_->to_proto_value();
    }

    // Obtain integer constant representing the stored data type.
    int kind() const {
        return self_->kind();
    }

   private:
    // ABC interface class for type erasure.
    struct concept_t {
        virtual ~concept_t() = default;
        virtual std::unique_ptr<concept_t> copy() const = 0;
        virtual google::protobuf::Value to_proto_value() const = 0;
        virtual int kind() const = 0;
        virtual bool equal_to(const concept_t& other) const = 0;
    };

    // Concrete model of interface.
    template <typename T>
    struct model final : concept_t {
        model(T t) : data(std::move(t)) {}
        ~model() override = default;

        std::unique_ptr<concept_t> copy() const override {
            return std::make_unique<model>(*this);
        }

        google::protobuf::Value to_proto_value() const override;

        int kind() const override;

        virtual bool equal_to(const concept_t& other) const override {
            if (this->kind() != other.kind()) {
                return false;
            }

            return data == reinterpret_cast<const model<T>*>(&other)->data;
        }

        T data;
    };

    // TODO this could be replaced with an SBO storage class to save a heap allocation on
    std::unique_ptr<concept_t> self_;
};

constexpr int x = sizeof(int);

using AttrMap = std::unordered_map<std::string, ProtoT>;

google::protobuf::Value to_proto_value(std::nullptr_t);
google::protobuf::Value to_proto_value(bool b);
google::protobuf::Value to_proto_value(int i);
google::protobuf::Value to_proto_value(double d);
google::protobuf::Value to_proto_value(std::string s);
google::protobuf::Value to_proto_value(const std::vector<ProtoT>& vec);
google::protobuf::Value to_proto_value(const AttrMap& m);

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
google::protobuf::Value ProtoT::model<T>::to_proto_value() const {
    return viam::sdk::to_proto_value(data);
}

template <typename T>
int ProtoT::model<T>::kind() const {
    return kind_t<T>::value;
}

}  // namespace sdk
}  // namespace viam