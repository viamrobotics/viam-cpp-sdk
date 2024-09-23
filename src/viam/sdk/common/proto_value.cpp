#include <viam/sdk/common/proto_value.hpp>

#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

using google::protobuf::Struct;
using google::protobuf::Value;

ProtoValue::ProtoValue() noexcept : ProtoValue(nullptr, 0) {}
ProtoValue::ProtoValue(std::nullptr_t) noexcept : ProtoValue() {}
ProtoValue::ProtoValue(bool b) noexcept : ProtoValue(b, 0) {}
ProtoValue::ProtoValue(int i) noexcept : ProtoValue(static_cast<double>(i)) {}
ProtoValue::ProtoValue(double d) noexcept : ProtoValue(d, 0) {}
ProtoValue::ProtoValue(std::string s) noexcept : ProtoValue(std::move(s), 0) {}

template <typename Val, typename>
ProtoValue::ProtoValue(std::vector<Val> v) noexcept(
    std::is_nothrow_move_constructible<std::vector<Val>>{})
    : ProtoValue(std::move(v), 0) {}

template <typename Val, typename>
ProtoValue::ProtoValue(std::unordered_map<std::string, Val> s) noexcept(
    std::is_nothrow_move_constructible<std::unordered_map<std::string, Val>>{})
    : ProtoValue(std::move(s), 0) {}

template <typename T>
ProtoValue::ProtoValue(T t, std::nullptr_t) noexcept(std::is_nothrow_move_constructible<T>{})
    : vtable_{model<T>::vtable_}, self_{std::move(t)} {}

// -- explicit instantiations of by-value constructors -- //
template ProtoValue::ProtoValue(ProtoList) noexcept(
    std::is_nothrow_move_constructible<ProtoList>{});
template ProtoValue::ProtoValue(ProtoStruct m) noexcept(
    std::is_nothrow_move_constructible<ProtoStruct>{});

ProtoValue::ProtoValue(const char* str) : ProtoValue(std::string(str)) {}

ProtoValue::ProtoValue(ProtoValue&& other) noexcept(proto_value_details::all_moves_noexcept{})
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
                  ProtoList vec;
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

ProtoValue& ProtoValue::operator=(ProtoValue&& other) noexcept(
    proto_value_details::all_moves_noexcept{}) {
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

void ProtoValue::swap(ProtoValue& other) noexcept(proto_value_details::all_moves_noexcept{}) {
    self_.swap(vtable_, other.self_, other.vtable_);
    std::swap(vtable_, other.vtable_);
}

template <typename Val>
ProtoValue ProtoValue::from_proto(const Val& v) {  // NOLINT(misc-no-recursion)
    return ProtoValue(&v);
}

template ProtoValue ProtoValue::from_proto(const Value&);

ProtoValue::Kind ProtoValue::kind() const {
    return vtable_.kind();
}

bool ProtoValue::is_null() const {
    return kind() == Kind::k_null;
}

template <typename T>
std::enable_if_t<std::is_scalar<T>{}, T&> ProtoValue::get_unchecked() {
    assert(this->is_a<T>());
    return *(this->self_.template get<T>());
}

template <typename T>
std::enable_if_t<std::is_scalar<T>{}, T> ProtoValue::get_unchecked() const {
    assert(this->is_a<T>());
    return *(this->self_.template get<T>());
}

template bool& ProtoValue::get_unchecked<bool>();
template double& ProtoValue::get_unchecked<double>();

template bool ProtoValue::get_unchecked<bool>() const;
template double ProtoValue::get_unchecked<double>() const;

template <typename T>
std::enable_if_t<!std::is_scalar<T>{}, T&> ProtoValue::get_unchecked() & {
    assert(this->is_a<T>());
    return *(this->self_.template get<T>());
}

template <typename T>
std::enable_if_t<!std::is_scalar<T>{}, T const&> ProtoValue::get_unchecked() const& {
    assert(this->is_a<T>());
    return *(this->self_.template get<T>());
}

template <typename T>
std::enable_if_t<!std::is_scalar<T>{}, T&&> ProtoValue::get_unchecked() && {
    assert(this->is_a<T>());
    return std::move(*(this->self_.template get<T>()));
}

template std::string& ProtoValue::get_unchecked<std::string>() &;
template ProtoList& ProtoValue::get_unchecked<ProtoList>() &;
template ProtoStruct& ProtoValue::get_unchecked<ProtoStruct>() &;

template std::string const& ProtoValue::get_unchecked<std::string>() const&;
template ProtoList const& ProtoValue::get_unchecked<ProtoList>() const&;
template ProtoStruct const& ProtoValue::get_unchecked<ProtoStruct>() const&;

template std::string&& ProtoValue::get_unchecked<std::string>() &&;
template ProtoList&& ProtoValue::get_unchecked<ProtoList>() &&;
template ProtoStruct&& ProtoValue::get_unchecked<ProtoStruct>() &&;

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
ProtoValue::Kind ProtoValue::model<T>::kind() noexcept {
    return proto_value_details::kind<T>::type::value;
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
    static_assert(sizeof(T) <= local_storage_size, "Type too large to fit in local storage");
    static_assert(alignof(T) <= local_storage_alignment,
                  "Type alignment too strict for local storage");

    static_assert(sizeof(std::vector<void*>) == sizeof(ProtoList),
                  "vector<ProtoValue> stand-in size mismatch");
    static_assert(alignof(std::vector<void*>) == alignof(ProtoList),
                  "vector<ProtoValue> stand-in alignment mismatch");

    static_assert(sizeof(std::unordered_map<std::string, void*>) == sizeof(ProtoStruct),
                  "ProtoStruct stand-in size mismatch");
    static_assert(alignof(std::unordered_map<std::string, void*>) == alignof(ProtoStruct),
                  "ProtoStruct stand-in alignment mismatch");

    new (&buf_) model<T>(std::move(t));
}

ProtoValue::storage::storage(const ProtoValue::storage& other, const ProtoValue::vtable& vtab) {
    vtab.copy(other.get(), this->get());
}

ProtoValue::storage::storage(ProtoValue::storage&& other, const ProtoValue::vtable& vtab) noexcept(
    proto_value_details::all_moves_noexcept{}) {
    vtab.move(other.get(), this->get());
}

void ProtoValue::storage::swap(
    const ProtoValue::vtable& this_vtable,
    ProtoValue::storage& other,
    const ProtoValue::vtable& other_vtable) noexcept(proto_value_details::all_moves_noexcept{}) {
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

void to_proto(double d, Value* v) {
    v->set_number_value(d);
}

void to_proto(std::string s, Value* v) {
    v->set_string_value(std::move(s));
}

void to_proto(const ProtoList& vec, Value* v) {
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
    t.vtable_.to_proto(t.self_.get(), v);
}

ProtoStruct struct_to_map(Struct const* s) {  // NOLINT(misc-no-recursion)
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
