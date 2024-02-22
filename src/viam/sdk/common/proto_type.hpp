#pragma once

#include <unordered_map>

#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

class ProtoType;
using AttributeMap = std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>;

namespace prototype_details {

template <typename T>
struct get_helper {
    template <typename V>
    static T* get(V& v) {
        return boost::get<T>(&v);
    }

    template <typename V>
    static const T* get(const V& v) {
        return boost::get<T>(&v);
    }
};

template <>
struct get_helper<AttributeMap> {
    template <typename V>
    static AttributeMap get(V& v) {
        auto* const result = boost::get<AttributeMap>(&v);
        return result ? *result : nullptr;
    }

    template <typename V>
    static std::shared_ptr<const AttributeMap::element_type> get(const V& v) {
        auto* const result = boost::get<AttributeMap>(&v);
        return result ? *result : nullptr;
    }
};

}  // namespace prototype_details

class ProtoType {
   public:
    ProtoType() {
        proto_type_ = boost::blank();
    }

    explicit ProtoType(bool b) : proto_type_(std::move(b)) {}
    explicit ProtoType(std::string s) : proto_type_(std::move(s)) {}
    explicit ProtoType(const char* c) : proto_type_(std::string(c)) {}
    explicit ProtoType(int i) : proto_type_(std::move(i)) {}
    explicit ProtoType(double d) : proto_type_(std::move(d)) {}
    explicit ProtoType(AttributeMap m) : proto_type_(std::move(m)) {}
    explicit ProtoType(std::vector<std::shared_ptr<ProtoType>> v) : proto_type_(std::move(v)) {}
    explicit ProtoType(const google::protobuf::Value& value);

    google::protobuf::Value proto_value();
    friend bool operator==(const ProtoType& lhs, const ProtoType& rhs);

    template <typename T>
    auto get() {
        return prototype_details::get_helper<T>::get(proto_type_);
    }

    template <typename T>
    auto get() const {
        return prototype_details::get_helper<T>::get(proto_type_);
    }

   private:
    boost::variant<boost::blank,
                   bool,
                   std::string,
                   int,
                   double,
                   AttributeMap,
                   std::vector<std::shared_ptr<ProtoType>>>
        proto_type_;
};

AttributeMap struct_to_map(const google::protobuf::Struct& struct_);
google::protobuf::Struct map_to_struct(const AttributeMap& dict);

}  // namespace sdk
}  // namespace viam
