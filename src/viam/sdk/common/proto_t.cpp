#include <viam/sdk/common/proto_t.hpp>

#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

using google::protobuf::Struct;
using google::protobuf::Value;

ProtoT::ProtoT() noexcept : ProtoT(nullptr) {}

template <typename T>
ProtoT::ProtoT(T t) noexcept(std::is_nothrow_move_constructible<T>{})
    : vtable_{model<T>::vtable}, self_{std::move(t)} {
    static_assert(!std::is_base_of<std::false_type, kind_t<T>>{},
                  "Attempted to construct ProtoT from invalid type.");
}

// -- explicit instantiations of by-value constructors -- //
template ProtoT::ProtoT(std::nullptr_t) noexcept;
template ProtoT::ProtoT(bool) noexcept;
template ProtoT::ProtoT(int) noexcept;
template ProtoT::ProtoT(double) noexcept;
template ProtoT::ProtoT(std::string) noexcept(std::is_nothrow_move_constructible<std::string>{});
template ProtoT::ProtoT(std::vector<ProtoT>) noexcept(
    std::is_nothrow_move_constructible<std::vector<ProtoT>>{});
template ProtoT::ProtoT(AttrMap m) noexcept(std::is_nothrow_move_constructible<AttrMap>{});

ProtoT::ProtoT(const char* str) : ProtoT(std::string(str)) {}

template <typename Self>
ProtoT::ProtoT(ProtoT&& other) noexcept(typename Self::is_always_nothrow_move_constructible{})
    : vtable_(std::move(other.vtable_)), self_(std::move(other.self_), vtable_) {}

template ProtoT::ProtoT(ProtoT&& other) noexcept(ProtoT::is_always_nothrow_move_constructible{});

ProtoT::ProtoT(const ProtoT& other) : vtable_(other.vtable_), self_(other.self_, other.vtable_) {}

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
                  return ProtoT(nullptr);
          }
      }(*value)) {}

ProtoT& ProtoT::operator=(ProtoT&& other) {
    ProtoT(std::move(other)).swap(*this);
    return *this;
}

ProtoT& ProtoT::operator=(const ProtoT& other) {
    ProtoT(other).swap(*this);
    return *this;
}

ProtoT::~ProtoT() {
    self_.destruct(vtable_);
}

bool operator==(const ProtoT& lhs, const ProtoT& rhs) {
    return lhs.vtable_.equal_to(lhs.self_.get(), rhs.self_.get(), rhs.vtable_);
}

void ProtoT::swap(ProtoT& other) {
    self_.swap(vtable_, other.self_, other.vtable_);
    std::swap(vtable_, other.vtable_);
}

template <typename Val>
ProtoT ProtoT::from_proto(const Val& v) {
    return ProtoT(&v);
}

template ProtoT ProtoT::from_proto(const Value&);

int ProtoT::kind() const {
    return vtable_.kind();
}

// --- ProtoT::model<T> definitions --- //
template <typename T>
ProtoT::model<T>::model(T t) noexcept(std::is_nothrow_move_constructible<T>{})
    : data(std::move(t)) {}

template <typename T>
void ProtoT::model<T>::dtor(void* self) noexcept {
    static_cast<model*>(self)->~model();
}

template <typename T>
void ProtoT::model<T>::copy(void const* self, void* dest) {
    new (dest) model(*static_cast<model const*>(self));
}

template <typename T>
void ProtoT::model<T>::move(void* self, void* dest) {
    new (dest) model(std::move(*static_cast<model*>(self)));
}

template <typename T>
void ProtoT::model<T>::to_proto(void const* self, google::protobuf::Value* v) {
    viam::sdk::to_proto(static_cast<model const*>(self)->data, v);
}

template <typename T>
int ProtoT::model<T>::kind() noexcept {
    return kind_t<T>::value;
}

template <typename T>
bool ProtoT::model<T>::equal_to(void const* self,
                                void const* other,
                                const ProtoT::vtable& other_vtable) {
    if (model::kind() != other_vtable.kind()) {
        return false;
    }

    return *static_cast<T const*>(self) == *static_cast<T const*>(other);
}

// --- ProtoT::storage definitions --- //
template <typename T>
ProtoT::storage::storage(T t) noexcept(std::is_nothrow_move_constructible<T>{}) {
    // Since ProtoT is an incomplete type at time of definition of struct storage, static_assert
    // post facto to make sure we sized and aligned the storage correctly
    static_assert(std::is_same<BufType,
                               std::aligned_union_t<0,
                                                    std::nullptr_t,
                                                    bool,
                                                    int,
                                                    double,
                                                    std::string,
                                                    std::vector<ProtoT>,
                                                    AttrMap>>{},
                  "storage class storage is misconfigured for possible ProtoT types");

    new (&buf_) model<T>(std::move(t));
}

ProtoT::storage::storage(const ProtoT::storage& other, const ProtoT::vtable& vtab) {
    vtab.copy(other.get(), this->get());
}

ProtoT::storage::storage(ProtoT::storage&& other, const ProtoT::vtable& vtab) {
    vtab.move(other.get(), this->get());
}

void ProtoT::storage::swap(const ProtoT::vtable& this_vtable,
                           ProtoT::storage& other,
                           const ProtoT::vtable& other_vtable) {
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

void ProtoT::storage::destruct(const ProtoT::vtable& vtab) {
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
