#include <boost/blank.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <unordered_map>
#include <vector>

#include "google/protobuf/struct.pb.h"

using google::protobuf::Struct;
using google::protobuf::Value;

class ProtoType;
Struct map_to_struct(std::unordered_map<std::string, ProtoType*> dict);
std::unordered_map<std::string, ProtoType*> struct_to_map(Struct struct_);

// float, string, struct, array, struct(map from string to any of the above)
class ProtoType {
       public:
	boost::variant<boost::blank, bool, std::string, int, double,
		       std::unordered_map<std::string, ProtoType*>,
		       std::vector<ProtoType*>>
	    proto_type;
	ProtoType() { proto_type = boost::blank(); }
	ProtoType(bool b) { proto_type = b; }
	ProtoType(std::string s) { proto_type = s; }
	ProtoType(int i) { proto_type = i; }
	ProtoType(double d) { proto_type = d; }
	ProtoType(std::unordered_map<std::string, ProtoType*> m) {
		proto_type = m;
	}
	ProtoType(std::vector<ProtoType*> v) { proto_type = v; }
	ProtoType(Value value);
	Value proto_value();
};

ProtoType::ProtoType(Value value) {
	switch (value.kind_case()) {
		case Value::KindCase::kBoolValue: {
			proto_type = value.bool_value();
			break;
		}
		case Value::KindCase::kStringValue: {
			proto_type = value.string_value();
			break;
		}
		case Value::KindCase::kNumberValue: {
			proto_type = value.number_value();
			break;
		}
		case Value::KindCase::kListValue: {
			std::vector<ProtoType*> vec;
			google::protobuf::ListValue list_val =
			    value.list_value();
			for (auto val : value.list_value().values()) {
				ProtoType p = ProtoType(val);
				vec.push_back(&p);
			}
			proto_type = &vec;
			break;
		}
		case Value::KindCase::kStructValue: {
			std::unordered_map<std::string, ProtoType*> map =
			    struct_to_map(value.struct_value());
			proto_type = map;
			break;
		}
		case Value::KindCase::KIND_NOT_SET:
		case Value::KindCase::kNullValue: {
			proto_type = boost::blank();
			break;
		}
	}
};

Struct map_to_struct(std::unordered_map<std::string, ProtoType*> dict) {
	Struct s;
	for (auto& key_and_value : dict) {
		const std::string key = key_and_value.first;
		Value value = key_and_value.second->proto_value();
		google::protobuf::MapPair<std::string, Value> val(key, value);
		s.mutable_fields()->insert(val);
	}

	return s;
}

std::unordered_map<std::string, ProtoType*> struct_to_map(Struct struct_) {
	google::protobuf::Map<std::string, Value> struct_map = struct_.fields();
	std::unordered_map<std::string, ProtoType*> map;
	for (auto val : struct_.fields()) {
		std::string key = val.first;
		ProtoType value(val.second);
		std::pair<std::string, ProtoType*> pair(key, &value);
		map.insert(pair);
	}

	return map;
};

Value ProtoType::proto_value() {
	Value v;
	switch (proto_type.which()) {
		case 0: {
			::google::protobuf::NullValue value;
			v.set_null_value(value);
			break;
		}
		case 1: {
			const bool b = boost::get<bool>(proto_type);
			v.set_bool_value(b);
			break;
		}
		case 2: {
			const std::string s =
			    boost::get<std::string>(proto_type);
			*v.mutable_string_value() = s;
			break;
		}
		case 3: {
			const int i = boost::get<int>(proto_type);
			v.set_number_value(i);
			break;
		}
		case 4: {
			const double i = boost::get<double>(proto_type);
			v.set_number_value(i);
			break;
		}
		case 5: {
			std::unordered_map<std::string, ProtoType*> map =
			    boost::get<
				std::unordered_map<std::string, ProtoType*>>(
				proto_type);
			Struct s = map_to_struct(map);
			*v.mutable_struct_value() = s;
			break;
		}
		case 6: {
			::google::protobuf::ListValue l =
			    *v.mutable_list_value();
			google::protobuf::RepeatedPtrField<Value>* values =
			    l.mutable_values();
			std::vector<ProtoType*> vec =
			    boost::get<std::vector<ProtoType*>>(proto_type);
			for (auto val : vec) {
				*values->Add() = val->proto_value();
			}
			break;
		}
		default: {
			throw "Invalid proto_value conversion type. This should never happen;\
						please file a bug report.";
		}
	}

	return v;
}

