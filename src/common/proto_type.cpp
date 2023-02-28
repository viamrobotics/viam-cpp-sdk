#include <google/protobuf/struct.pb.h>

#include <boost/blank.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <common/proto_type.hpp>
#include <unordered_map>
#include <vector>

using google::protobuf::Struct;
using google::protobuf::Value;

Struct map_to_struct(std::unordered_map<std::string, ProtoType*> dict);
std::unordered_map<std::string, ProtoType*> struct_to_map(Struct struct_);

// float, string, struct, array, struct(map from string to any of the above)

ProtoType ProtoType::of_value(Value value) {
    switch (value.kind_case()) {
        case Value::KindCase::kBoolValue: {
            return ProtoType(value.bool_value());
        }
        case Value::KindCase::kStringValue: {
            return ProtoType(value.string_value());
        }
        case Value::KindCase::kNumberValue: {
            return ProtoType(value.number_value());
        }
        case Value::KindCase::kListValue: {
            std::vector<ProtoType*> vec;
            google::protobuf::ListValue list_val = value.list_value();
            for (auto& val : value.list_value().values()) {
                ProtoType p = ProtoType::of_value(val);
                vec.push_back(&p);
            }
            return ProtoType(vec);
        }
        case Value::KindCase::kStructValue: {
            std::unordered_map<std::string, ProtoType*> map = struct_to_map(value.struct_value());
            return ProtoType(map);
        }
        case Value::KindCase::KIND_NOT_SET:
        case Value::KindCase::kNullValue: {
            return ProtoType();
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
    for (auto& val : struct_.fields()) {
        std::string key = val.first;
        ProtoType value = ProtoType::of_value(val.second);
        map.emplace(key, &value);
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
            const std::string s = boost::get<std::string>(proto_type);
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
                boost::get<std::unordered_map<std::string, ProtoType*>>(proto_type);
            Struct s = map_to_struct(map);
            *v.mutable_struct_value() = s;
            break;
        }
        case 6: {
            ::google::protobuf::ListValue l = *v.mutable_list_value();
            google::protobuf::RepeatedPtrField<Value>* values = l.mutable_values();
            std::vector<ProtoType*> vec = boost::get<std::vector<ProtoType*>>(proto_type);
            for (auto& val : vec) {
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
