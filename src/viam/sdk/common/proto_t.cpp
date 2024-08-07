#include <viam/sdk/common/proto_t.hpp>

#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

using google::protobuf::Struct;
using google::protobuf::Value;

ProtoT::ProtoT(const Value* value)  // NOLINT(misc-no-recursion)
    : ProtoT([](const Value& v) {   // NOLINT(misc-no-recursion)
          switch (v.kind_case()) {
              case Value::KindCase::kBoolValue: {
                  return ProtoT(v.bool_value());
              }
              case Value::KindCase::kStringValue: {
                  return ProtoT(v.string_value());
              }
              case Value::KindCase::kNumberValue: {
                  return ProtoT(v.number_value());
              }
              case Value::KindCase::kListValue: {
                  std::vector<ProtoT> vec;
                  vec.reserve(v.list_value().values_size());
                  for (const Value& list_val : v.list_value().values()) {
                      vec.push_back(ProtoT::from_proto(list_val));
                  }

                  return ProtoT(std::move(vec));
              }
              case Value::KindCase::kStructValue: {
                  return ProtoT(struct_to_map(v.struct_value()));
              }
              case Value::KindCase::KIND_NOT_SET:
              case Value::KindCase::kNullValue:
              default:
                  return ProtoT();
          }
      }(*value)) {}

void to_proto(std::nullptr_t, Value* v) {
    v->set_null_value(::google::protobuf::NULL_VALUE);
}

void to_proto(bool b, Value* v) {
    v->set_bool_value(b);
}

void to_proto(int i, Value* v) {
    v->set_number_value(i);
}

void to_proto(double d, Value* v) {
    v->set_number_value(d);
}

void to_proto(std::string s, Value* v) {
    v->set_string_value(std::move(s));
}

void to_proto(const std::vector<ProtoT>& vec, Value* v) {
    ::google::protobuf::ListValue l;
    for (const auto& val : vec) {
        *l.add_values() = to_proto(val);
    }
    *(v->mutable_list_value()) = std::move(l);
}

void to_proto(const AttrMap& m, Value* v) {
    Struct s;
    map_to_struct(m, &s);

    *(v->mutable_struct_value()) = std::move(s);
}

void to_proto(const ProtoT& t, Value* v) {
    return t.vtable_.to_proto(t.self_.get(), v);
}

AttrMap struct_to_map(Struct const* s) {
    AttrMap map;
    for (const auto& val : s->fields()) {
        map.emplace(val.first, ProtoT::from_proto(val.second));
    }

    return map;
}

void map_to_struct(const AttrMap& m, Struct* s) {
    for (const auto& kv : m) {
        s->mutable_fields()->insert(
            google::protobuf::MapPair<std::string, Value>(kv.first, to_proto(kv.second)));
    }
}

}  // namespace sdk
}  // namespace viam
