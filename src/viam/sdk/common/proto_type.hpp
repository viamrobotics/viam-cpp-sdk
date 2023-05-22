#pragma once

#include <unordered_map>

#include <boost/variant/variant.hpp>
#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

class ProtoType;
using AttributeMap = std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>;

class ProtoType {
   public:
    ProtoType() {
        proto_type_ = boost::blank();
    }

    explicit ProtoType(bool b) : proto_type_(std::move(b)) {}
    explicit ProtoType(std::string s) : proto_type_(std::move(s)) {}
    explicit ProtoType(int i) : proto_type_(std::move(i)) {}
    explicit ProtoType(double d) : proto_type_(std::move(d)) {}
    explicit ProtoType(AttributeMap m) : proto_type_(std::move(m)) {}
    explicit ProtoType(std::vector<std::shared_ptr<ProtoType>> v) : proto_type_(std::move(v)) {}
    explicit ProtoType(const google::protobuf::Value& value);

    google::protobuf::Value proto_value();
    friend bool operator==(const ProtoType& lhs, const ProtoType& rhs);

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

AttributeMap struct_to_map(google::protobuf::Struct struct_);
google::protobuf::Struct map_to_struct(AttributeMap dict);

}  // namespace sdk
}  // namespace viam
