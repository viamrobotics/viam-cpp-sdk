#ifndef UTILS_H
#define UTILS_H

#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <unordered_map>

#include "google/protobuf/struct.pb.h"

using google::protobuf::Struct;
using google::protobuf::Value;

class ProtoType {
       public:
	boost::variant<bool, std::string> proto_type;
	ProtoType(std::string s) { proto_type = s; }
	ProtoType(bool b) { proto_type = b; }

       private:
	Value proto_value();
};

Struct map_to_struct(std::unordered_map<std::string, ProtoType> dict);

#endif
