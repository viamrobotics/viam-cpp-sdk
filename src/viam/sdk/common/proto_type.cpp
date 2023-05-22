#include <viam/sdk/common/proto_type.hpp>

#include <algorithm>
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

// NOLINTNEXTLINE(misc-no-recursion)
Struct map_to_struct(AttributeMap dict) {
    Struct s;
    for (const auto& key_and_value : *dict) {
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
        // NOLINTNEXTLINE(misc-no-recursion)
        map->emplace(std::move(key), std::make_shared<ProtoType>(val.second));
    }
    return map;
}

// NOLINTNEXTLINE(misc-no-recursion)
ProtoType::ProtoType(const Value& value) {
    switch (value.kind_case()) {
        case Value::KindCase::kBoolValue: {
            proto_type_ = value.bool_value();
            break;
        }
        case Value::KindCase::kStringValue: {
            proto_type_ = value.string_value();
            break;
        }
        case Value::KindCase::kNumberValue: {
            proto_type_ = value.number_value();
            break;
        }
        case Value::KindCase::kListValue: {
            std::vector<std::shared_ptr<ProtoType>> vec;
            for (const auto& val : value.list_value().values()) {
                // NOLINTNEXTLINE(misc-no-recursion)
                auto p = std::make_shared<ProtoType>(val);
                vec.push_back(p);
            }
            proto_type_ = std::move(vec);
            break;
        }
        case Value::KindCase::kStructValue: {
            proto_type_ = struct_to_map(value.struct_value());
            break;
        }
        case Value::KindCase::KIND_NOT_SET:
        case Value::KindCase::kNullValue:
        default: {
            proto_type_ = boost::blank();
            break;
        }
    }
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
            ::google::protobuf::ListValue l;
            google::protobuf::RepeatedPtrField<Value> values;
            auto vec = boost::get<std::vector<std::shared_ptr<ProtoType>>>(proto_type_);
            for (const auto& val : vec) {
                *l.add_values() = val->proto_value();
            }
            *v.mutable_list_value() = l;
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
            const AttributeMap& lhs_map = boost::get<AttributeMap>(lhs.proto_type_);
            const AttributeMap& rhs_map = boost::get<AttributeMap>(rhs.proto_type_);

            if (lhs_map->size() != rhs_map->size()) {
                return false;
            }
            // NOLINTNEXTLINE(misc-no-recursion)
            return std::all_of(lhs_map->begin(), lhs_map->end(), [rhs_map](auto lhs) {
                return (rhs_map->find(lhs.first) != rhs_map->end() &&
                        (*rhs_map->at(lhs.first) == *lhs.second));
            });
        }
        case 6: {
            // NOLINTNEXTLINE(misc-no-recursion)
            auto pred = [](auto lhs, auto rhs) { return *lhs == *rhs; };

            const auto& lhs_vec =
                boost::get<std::vector<std::shared_ptr<ProtoType>>>(lhs.proto_type_);
            const auto& rhs_vec =
                boost::get<std::vector<std::shared_ptr<ProtoType>>>(rhs.proto_type_);
            return std::equal(lhs_vec.begin(), lhs_vec.end(), rhs_vec.begin(), rhs_vec.end(), pred);
        }
        default: {
            throw "Invalid proto_value conversion type. This should never happen;\
                   please file a bug report.";
        }
    }
}

}  // namespace sdk
}  // namespace viam
