#include <viam/sdk/common/proto_t.hpp>

#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

using google::protobuf::Struct;
using google::protobuf::Value;

ProtoValue::ProtoValue() noexcept : ProtoValue(nullptr) {}

template <typename T>
ProtoValue::ProtoValue(T t) noexcept(std::is_nothrow_move_constructible<T>{})
    : vtable_{model<T>::vtable}, self_{std::move(t)} {}

// -- explicit instantiations of by-value constructors -- //
template ProtoValue::ProtoValue(std::nullptr_t) noexcept;
template ProtoValue::ProtoValue(bool) noexcept;
template ProtoValue::ProtoValue(int) noexcept;
template ProtoValue::ProtoValue(double) noexcept;
template ProtoValue::ProtoValue(std::string) noexcept(
    std::is_nothrow_move_constructible<std::string>{});
template ProtoValue::ProtoValue(std::vector<ProtoValue>) noexcept(
    std::is_nothrow_move_constructible<std::vector<ProtoValue>>{});
template ProtoValue::ProtoValue(ProtoStruct m) noexcept(
    std::is_nothrow_move_constructible<ProtoStruct>{});

ProtoValue::ProtoValue(const char* str) : ProtoValue(std::string(str)) {}

ProtoValue::ProtoValue(ProtoValue&& other) noexcept(impl::all_proto_moves_noexcept{})
    : vtable_(std::move(other.vtable_)), self_(std::move(other.self_), vtable_) {}

ProtoValue::ProtoValue(const ProtoValue& other)
    : vtable_(other.vtable_), self_(other.self_, other.vtable_) {}

ProtoValue::ProtoValue(const Value* value)  // NOLINT(misc-no-recursion)
    : ProtoValue([](const Value& v) {       // NOLINT(misc-no-recursion)
          switch (v.kind_case()) {
              case Value::KindCase::kBoolValue: {
                  return ProtoValue(v.bool_value());
              }
              case Value::KindCase::kStringValue: {
                  return ProtoValue(v.string_value());
              }
              case Value::KindCase::kNumberValue: {
                  return ProtoValue(v.number_value());
              }
              case Value::KindCase::kListValue: {
                  std::vector<ProtoValue> vec;
                  vec.reserve(v.list_value().values_size());
                  for (const Value& list_val : v.list_value().values()) {
                      vec.push_back(ProtoValue::from_proto(list_val));
                  }

                  return ProtoValue(std::move(vec));
              }
              case Value::KindCase::kStructValue: {
                  return ProtoValue(struct_to_map(v.struct_value()));
              }
              case Value::KindCase::KIND_NOT_SET:
              case Value::KindCase::kNullValue:
              default:
                  return ProtoValue(nullptr);
          }
      }(*value)) {}

ProtoValue& ProtoValue::operator=(ProtoValue&& other) noexcept(impl::all_proto_moves_noexcept{}) {
    ProtoValue(std::move(other)).swap(*this);
    return *this;
}

ProtoValue& ProtoValue::operator=(const ProtoValue& other) {
    ProtoValue(other).swap(*this);
    return *this;
}

ProtoValue::~ProtoValue() {
    self_.destruct(vtable_);
}

bool operator==(const ProtoValue& lhs, const ProtoValue& rhs) {
    return lhs.vtable_.equal_to(lhs.self_.get(), rhs.self_.get(), rhs.vtable_);
}

void ProtoValue::swap(ProtoValue& other) noexcept(impl::all_proto_moves_noexcept{}) {
    self_.swap(vtable_, other.self_, other.vtable_);
    std::swap(vtable_, other.vtable_);
}

template <typename Val>
ProtoValue ProtoValue::from_proto(const Val& v) {
    return ProtoValue(&v);
}

template ProtoValue ProtoValue::from_proto(const Value&);

int ProtoValue::kind() const {
    return vtable_.kind();
}

// --- ProtoT::model<T> definitions --- //
template <typename T>
ProtoValue::model<T>::model(T t) noexcept(std::is_nothrow_move_constructible<T>{})
    : data(std::move(t)) {}

template <typename T>
void ProtoValue::model<T>::dtor(void* self) noexcept {
    static_cast<model*>(self)->~model();
}

template <typename T>
void ProtoValue::model<T>::copy(void const* self, void* dest) {
    new (dest) model(*static_cast<model const*>(self));
}

template <typename T>
void ProtoValue::model<T>::move(void* self, void* dest) {
    new (dest) model(std::move(*static_cast<model*>(self)));
}

template <typename T>
void ProtoValue::model<T>::to_proto(void const* self, google::protobuf::Value* v) {
    viam::sdk::to_proto(static_cast<model const*>(self)->data, v);
}

template <typename T>
int ProtoValue::model<T>::kind() noexcept {
    return impl::kind_t<T>::value;
}

template <typename T>
bool ProtoValue::model<T>::equal_to(void const* self,
                                    void const* other,
                                    const ProtoValue::vtable& other_vtable) {
    if (model::kind() != other_vtable.kind()) {
        return false;
    }

    return *static_cast<T const*>(self) == *static_cast<T const*>(other);
}

// --- ProtoT::storage definitions --- //
template <typename T>
ProtoValue::storage::storage(T t) noexcept(std::is_nothrow_move_constructible<T>{}) {
    // Since ProtoT is an incomplete type at time of definition of struct storage, static_assert
    // post facto to make sure we sized and aligned the storage correctly
    static_assert(std::is_same<BufType,
                               std::aligned_union_t<0,
                                                    std::nullptr_t,
                                                    bool,
                                                    int,
                                                    double,
                                                    std::string,
                                                    std::vector<ProtoValue>,
                                                    ProtoStruct>>{},
                  "storage class storage is misconfigured for possible ProtoT types");

    new (&buf_) model<T>(std::move(t));
}

ProtoValue::storage::storage(const ProtoValue::storage& other, const ProtoValue::vtable& vtab) {
    vtab.copy(other.get(), this->get());
}

ProtoValue::storage::storage(ProtoValue::storage&& other, const ProtoValue::vtable& vtab) noexcept(
    impl::all_proto_moves_noexcept{}) {
    vtab.move(other.get(), this->get());
}

void ProtoValue::storage::swap(
    const ProtoValue::vtable& this_vtable,
    ProtoValue::storage& other,
    const ProtoValue::vtable& other_vtable) noexcept(impl::all_proto_moves_noexcept{}) {
    if (this == &other) {
        return;
    }

    BufType tmp;
    other_vtable.move(other.get(), &tmp);
    other_vtable.dtor(other.get());

    this_vtable.move(this->get(), other.get());
    this_vtable.dtor(this->get());

    other_vtable.move(&tmp, this->get());
    other_vtable.dtor(&tmp);
}

void ProtoValue::storage::destruct(const ProtoValue::vtable& vtab) noexcept {
    vtab.dtor(this->get());
}

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

void to_proto(const std::vector<ProtoValue>& vec, Value* v) {
    ::google::protobuf::ListValue l;
    for (const auto& val : vec) {
        *l.add_values() = to_proto(val);
    }
    *(v->mutable_list_value()) = std::move(l);
}

void to_proto(const ProtoStruct& m, Value* v) {
    Struct s;
    map_to_struct(m, &s);

    *(v->mutable_struct_value()) = std::move(s);
}

void to_proto(const ProtoValue& t, Value* v) {
    return t.vtable_.to_proto(t.self_.get(), v);
}

ProtoStruct struct_to_map(Struct const* s) {
    ProtoStruct map;
    for (const auto& val : s->fields()) {
        map.emplace(val.first, ProtoValue::from_proto(val.second));
    }

    return map;
}

void map_to_struct(const ProtoStruct& m, Struct* s) {
    for (const auto& kv : m) {
        s->mutable_fields()->insert(
            google::protobuf::MapPair<std::string, Value>(kv.first, to_proto(kv.second)));
    }
}

}  // namespace sdk
}  // namespace viam
