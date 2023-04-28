#pragma once

#include <unordered_map>

#include <boost/variant/variant.hpp>
#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

class ProtoType {
   public:
    ProtoType() {
        proto_type_ = boost::blank();
    }
    ProtoType(bool b) {
        proto_type_ = b;
    }
    ProtoType(std::string s) {
        proto_type_ = s;
    }
    ProtoType(int i) {
        proto_type_ = i;
    }
    ProtoType(double d) {
        proto_type_ = d;
    }
    ProtoType(std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> m) {
        proto_type_ = m;
    }
    ProtoType(std::vector<ProtoType*> v) {
        proto_type_ = v;
    }

    // TODO: RSDK-2421 consider switching this to a constructor
    static ProtoType of_value(google::protobuf::Value value);

    google::protobuf::Value proto_value();
    friend bool operator==(const ProtoType& lhs, const ProtoType& rhs);

   private:
    boost::variant<boost::blank,
                   bool,
                   std::string,
                   int,
                   double,
                   std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>,
                   std::vector<ProtoType*>>
        proto_type_;
};

std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> struct_to_map(
    google::protobuf::Struct struct_);

google::protobuf::Struct map_to_struct(
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> dict);

}  // namespace sdk
}  // namespace viam
