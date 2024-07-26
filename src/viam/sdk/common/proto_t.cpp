#include <viam/sdk/common/proto_t.hpp>

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

using google::protobuf::Struct;
using google::protobuf::Value;

ProtoT ctor_helper(const Value& value) {
    switch (value.kind_case()) {
        case Value::KindCase::kBoolValue: {
            return ProtoT(value.bool_value());
        }
        case Value::KindCase::kStringValue: {
            return ProtoT(value.string_value());
        }
        case Value::KindCase::kNumberValue: {
            return ProtoT(value.number_value());
        }
        case Value::KindCase::kListValue: {
            return ProtoT(std::vector<ProtoT>(value.list_value().values().begin(),
                                              value.list_value().values().end()));
        }
        case Value::KindCase::kStructValue: {
            std::unordered_map<std::string, ProtoT> map;

            for (const auto& val : value.struct_value().fields()) {
                map.emplace(val.first, ProtoT(val.second));
            }

            return ProtoT(std::move(map));
        }
        case Value::KindCase::KIND_NOT_SET:
        case Value::KindCase::kNullValue:
        default:
            return ProtoT();
    }
}

ProtoT::ProtoT(const Value& value) : ProtoT(ctor_helper(value)) {}

google::protobuf::Value to_proto_value(std::nullptr_t) {
    Value v;
    v.set_null_value(::google::protobuf::NULL_VALUE);

    return v;
}

google::protobuf::Value to_proto_value(bool b) {
    Value v;
    v.set_bool_value(b);

    return v;
}

google::protobuf::Value to_proto_value(int i) {
    Value v;
    v.set_number_value(i);

    return v;
}

google::protobuf::Value to_proto_value(double d) {
    Value v;
    v.set_number_value(d);

    return v;
}

google::protobuf::Value to_proto_value(std::string s) {
    Value v;
    v.set_string_value(std::move(s));

    return v;
}

google::protobuf::Value to_proto_value(const std::vector<ProtoT>& vec) {
    Value v;
    ::google::protobuf::ListValue l;
    for (const auto& val : vec) {
        *l.add_values() = to_proto_value(val);
    }
    *v.mutable_list_value() = l;

    return v;
}

google::protobuf::Value to_proto_value(const std::unordered_map<std::string, ProtoT>& m) {
    Struct s;

    for (const auto& kv : m) {
        const std::string key = kv.first;
        const Value val = to_proto_value(kv.second);
        const google::protobuf::MapPair<std::string, Value> mp(key, val);
        s.mutable_fields()->insert(mp);
    }

    Value v;
    *v.mutable_struct_value() = s;

    return v;
}

}  // namespace sdk
}  // namespace viam
