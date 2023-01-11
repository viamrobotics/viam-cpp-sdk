#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <google/protobuf/struct.pb.h>

#include <boost/variant/variant.hpp>
#include <unordered_map>

class ProtoType {
   public:
    boost::variant<boost::blank,
                   bool,
                   std::string,
                   int,
                   double,
                   std::unordered_map<std::string, ProtoType*>,
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
    ProtoType(int i) {
        proto_type = i;
    }
    ProtoType(double d) {
        proto_type = d;
    }
    ProtoType(std::unordered_map<std::string, ProtoType*> m) {
        proto_type = m;
    }
    ProtoType(std::vector<ProtoType*> v) {
        proto_type = v;
    }

    google::protobuf::Value proto_value();
};

std::unordered_map<std::string, ProtoType*> struct_to_map(google::protobuf::Struct struct_);

google::protobuf::Struct map_to_struct(std::unordered_map<std::string, ProtoType*> dict);

#endif
