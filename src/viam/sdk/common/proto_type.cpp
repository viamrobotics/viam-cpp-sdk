#include <viam/sdk/common/proto_type.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

#include <boost/blank.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <google/protobuf/struct.pb.h>

#include <viam/sdk/config/resource.hpp>

namespace viam {
namespace sdk {

using google::protobuf::Struct;
using google::protobuf::Value;

// NOLINTBEGIN(readability-redundant-declaration)
Struct map_to_struct(AttributeMap dict);
AttributeMap struct_to_map(Struct struct_);
// NOLINTEND(readability-redundant-declaration)

// float, string, struct, array, struct(map from string to any of the above)

// NOLINTNEXTLINE(misc-no-recursion)
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
            for (const auto& val : value.list_value().values()) {
                ProtoType p = ProtoType::of_value(val);
                vec.push_back(&p);
            }
            return ProtoType(vec);
        }
        case Value::KindCase::kStructValue: {
            AttributeMap map = struct_to_map(value.struct_value());
            return ProtoType(map);
        }
        case Value::KindCase::KIND_NOT_SET:
        case Value::KindCase::kNullValue:
        default: {
            return ProtoType();
        }
    }
}

// NOLINTNEXTLINE(misc-no-recursion)
Struct map_to_struct(AttributeMap dict) {
    Struct s;
    for (auto& key_and_value : *dict) {
        const std::string key = key_and_value.first;
        Value value = key_and_value.second->proto_value();
        google::protobuf::MapPair<std::string, Value> val(key, value);
        s.mutable_fields()->insert(val);
    }

    return s;
}

// NOLINTNEXTLINE(misc-no-recursion)
AttributeMap struct_to_map(Struct struct_) {
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();

    for (const auto& val : struct_.fields()) {
        std::string key = val.first;
        std::shared_ptr<Value> val_ptr = std::make_shared<Value>(val.second);
        std::shared_ptr<ProtoType> value =
            std::make_shared<ProtoType>(ProtoType::of_value(val.second));
        map->emplace(key, value);
    }
    return map;
}

// NOLINTNEXTLINE(misc-no-recursion)
Value ProtoType::proto_value() {
    Value v;
    switch (proto_type_.which()) {
        case 0: {
            v.set_null_value(::google::protobuf::NULL_VALUE);
            break;
        }
        case 1: {
            const bool b = boost::get<bool>(proto_type_);
            v.set_bool_value(b);
            break;
        }
        case 2: {
            const std::string s = boost::get<std::string>(proto_type_);
            *v.mutable_string_value() = s;
            break;
        }
        case 3: {
            const int i = boost::get<int>(proto_type_);
            v.set_number_value(i);
            break;
        }
        case 4: {
            const double i = boost::get<double>(proto_type_);
            v.set_number_value(i);
            break;
        }
        case 5: {
            const AttributeMap map = boost::get<AttributeMap>(proto_type_);
            Struct s = map_to_struct(map);
            *v.mutable_struct_value() = s;
            break;
        }
        case 6: {
            ::google::protobuf::ListValue l = *v.mutable_list_value();
            google::protobuf::RepeatedPtrField<Value>* values = l.mutable_values();
            std::vector<ProtoType*> vec = boost::get<std::vector<ProtoType*>>(proto_type_);
            for (auto& val : vec) {
                *values->Add() = val->proto_value();
            }
            break;
        }
        default: {
            throw std::runtime_error(
                "Invalid proto_value conversion type. This should never happen;\
						please file a bug report.");
        }
    }

    return v;
}

// NOLINTNEXTLINE(misc-no-recursion)
bool operator==(const ProtoType& lhs, const ProtoType& rhs) {
    if (lhs.proto_type_.which() != rhs.proto_type_.which()) {
        return false;
    }
    switch (lhs.proto_type_.which()) {
        case 0: {
            // both null values
            return true;
        }
        case 1: {
            const bool lhs_b = boost::get<bool>(lhs.proto_type_);
            const bool rhs_b = boost::get<bool>(rhs.proto_type_);
            return lhs_b == rhs_b;
        }
        case 2: {
            const std::string lhs_s = boost::get<std::string>(lhs.proto_type_);
            const std::string rhs_s = boost::get<std::string>(rhs.proto_type_);
            return lhs_s == rhs_s;
        }
        case 3: {
            const int lhs_i = boost::get<int>(lhs.proto_type_);
            const int rhs_i = boost::get<int>(rhs.proto_type_);
            return lhs_i == rhs_i;
        }
        case 4: {
            const double lhs_i = boost::get<double>(lhs.proto_type_);
            const double rhs_i = boost::get<double>(rhs.proto_type_);
            return lhs_i == rhs_i;
        }
        case 5: {
            AttributeMap lhs_map = boost::get<AttributeMap>(lhs.proto_type_);
            AttributeMap rhs_map = boost::get<AttributeMap>(rhs.proto_type_);

            if (lhs_map->size() != rhs_map->size()) {
                return false;
            }
            // NOLINTNEXTLINE(misc-no-recursion)
            auto pred = [](auto lhs_map, auto rhs_map) {
                return lhs_map.first == rhs_map.first && *lhs_map.second == *rhs_map.second;
            };

            return std::equal(lhs_map->begin(), lhs_map->end(), rhs_map->begin(), pred);
        }
        case 6: {
            std::vector<ProtoType*> lhs_vec = boost::get<std::vector<ProtoType*>>(lhs.proto_type_);
            std::vector<ProtoType*> rhs_vec = boost::get<std::vector<ProtoType*>>(rhs.proto_type_);
            return lhs_vec == rhs_vec;
        }
        default: {
            throw "Invalid proto_value conversion type. This should never happen;\
                   please file a bug report.";
        }
    }
}

}  // namespace sdk
}  // namespace viam
