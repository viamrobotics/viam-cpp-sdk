#pragma once

#include <google/protobuf/struct.pb.h>

#include <boost/variant/variant.hpp>
#include <unordered_map>

class ProtoType {
   public:
    boost::variant<boost::blank,
                   bool,
                   std::string,
                   char const*,
                   int,
                   double,
                   std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>,
                   std::vector<ProtoType*>>
        proto_type;
    ProtoType() {
        proto_type = boost::blank();
    }
    ProtoType(bool b) {
        proto_type = b;
    }
    ProtoType(std::string s) {
        proto_type = s;
    }
  ProtoType(char const* c) {
        proto_type = c;
    } 
    ProtoType(int i) {
        proto_type = i;
    }
    ProtoType(double d) {
        proto_type = d;
    }
    ProtoType(std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> m) {
        proto_type = m;
    }
    ProtoType(std::vector<ProtoType*> v) {
        proto_type = v;
    }

    static ProtoType of_value(const google::protobuf::Value& value);

    google::protobuf::Value proto_value();

};

std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> struct_to_map(google::protobuf::Struct struct_);

google::protobuf::Struct map_to_struct(std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> dict);


bool operator==(const ProtoType &lhs, const ProtoType &rhs);

