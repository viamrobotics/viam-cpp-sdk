#include <viam/sdk/common/proto_value.hpp>

#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

using google::protobuf::Struct;
using google::protobuf::Value;

ProtoValue::ProtoValue() noexcept : ProtoValue(nullptr, nullptr) {}
ProtoValue::ProtoValue(std::nullptr_t) noexcept : ProtoValue() {}
ProtoValue::ProtoValue(bool b) noexcept : ProtoValue(b, nullptr) {}
ProtoValue::ProtoValue(int i) noexcept : ProtoValue(static_cast<double>(i)) {}
ProtoValue::ProtoValue(double d) noexcept : ProtoValue(d, nullptr) {}
ProtoValue::ProtoValue(std::string s) noexcept : ProtoValue(std::move(s), nullptr) {}

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

ProtoValue::Kind ProtoValue::kind() const {
    return vtable_.kind();
}

bool ProtoValue::is_null() const {
    return kind() == Kind::k_null;
}

template <typename T>
T& ProtoValue::get_unchecked() & {
    assert(this->is_a<T>());
    return *(this->self_.template get<T>());
}

template bool& ProtoValue::get_unchecked<bool>() &;
template double& ProtoValue::get_unchecked<double>() &;
template std::string& ProtoValue::get_unchecked<std::string>() &;
template ProtoList& ProtoValue::get_unchecked<ProtoList>() &;
template ProtoStruct& ProtoValue::get_unchecked<ProtoStruct>() &;

template <typename T>
const T& ProtoValue::get_unchecked() const& {
    assert(this->is_a<T>());
    return *(this->self_.template get<T>());
}

template const bool& ProtoValue::get_unchecked<bool>() const&;
template const double& ProtoValue::get_unchecked<double>() const&;
template std::string const& ProtoValue::get_unchecked<std::string>() const&;
template ProtoList const& ProtoValue::get_unchecked<ProtoList>() const&;
template ProtoStruct const& ProtoValue::get_unchecked<ProtoStruct>() const&;

template <typename T>
T&& ProtoValue::get_unchecked() && {
    assert(this->is_a<T>());
    return std::move(*(this->self_.template get<T>()));
}

template bool&& ProtoValue::get_unchecked<bool>() &&;
template double&& ProtoValue::get_unchecked<double>() &&;
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
void ProtoValue::model<T>::to_value(void const* self, google::protobuf::Value* v) {
    viam::sdk::proto_value_details::to_value(static_cast<model const*>(self)->data, v);
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

namespace proto_value_details {

void to_value(std::nullptr_t, Value* v) {
    v->set_null_value(::google::protobuf::NULL_VALUE);
}

void to_value(bool b, Value* v) {
    v->set_bool_value(b);
}

void to_value(double d, Value* v) {
    v->set_number_value(d);
}

void to_value(std::string s, Value* v) {
    v->set_string_value(std::move(s));
}

void to_value(const ProtoList& vec, Value* v) {
    ::google::protobuf::ListValue l;
    for (const auto& val : vec) {
        *l.add_values() = to_proto(val);
    }
    *(v->mutable_list_value()) = std::move(l);
}

void to_value(const ProtoStruct& m, Value* v) {
    *(v->mutable_struct_value()) = to_proto(m);
}

void to_value(const google::protobuf::FieldMask& fm, google::protobuf::Value* v) {
    ProtoStruct s;
    ProtoList paths_list;
    for (const auto& path : fm.paths()) {
        paths_list.push_back(path);
    }
    s.insert({"paths", paths_list});
    to_value(s, v);
}

void to_value(const DataClient::SequenceResourceFilter& srf, google::protobuf::Value* v) {
    ProtoStruct s;
    s.insert({"resource_name", srf.resource_name});
    s.insert({"method_name", srf.method_name});
    to_value(s, v);
}

void to_value(const DataClient::Sequence& seq, google::protobuf::Value* v) {
    ProtoStruct s;
    s.insert({"id", seq.id});
    s.insert({"part_id", seq.part_id});
    ProtoList tags_list;
    for (const auto& tag : seq.sequence_tags) {
        tags_list.push_back(tag);
    }
    s.insert({"sequence_tags", tags_list});
    s.insert({"created_at", to_proto(seq.created_at)});
    s.insert({"updated_at", to_proto(seq.updated_at)});
    s.insert({"start_time", to_proto(seq.start_time)});
    s.insert({"end_time", to_proto(seq.end_time)});
    ProtoList resources_list;
    for (const auto& r : seq.resources) {
        resources_list.push_back(r);
    }
    s.insert({"resources", resources_list});
    to_value(s, v);
}

}  // namespace proto_value_details

namespace proto_convert_details {

void to_proto_impl<ProtoValue>::operator()(const ProtoValue& self,
                                           google::protobuf::Value* v) const {
    self.vtable_.to_value(self.self_.get(), v);
}

ProtoValue from_proto_impl<google::protobuf::Value>::operator()(  // NOLINT(misc-no-recursion)
    const google::protobuf::Value* v) const {
    switch (v->kind_case()) {
        case Value::KindCase::kBoolValue: {
            return ProtoValue(v->bool_value());
        }
        case Value::KindCase::kStringValue: {
            return ProtoValue(v->string_value());
        }
        case Value::KindCase::kNumberValue: {
            return ProtoValue(v->number_value());
        }
        case Value::KindCase::kListValue: {
            ProtoList vec;
            vec.reserve(v->list_value().values_size());
            for (const Value& list_val : v->list_value().values()) {
                vec.push_back(from_proto(list_val));
            }

            return ProtoValue(std::move(vec));
        }
        case Value::KindCase::kStructValue: {
            return ProtoValue(from_proto(v->struct_value()));
        }
        case Value::KindCase::KIND_NOT_SET:
        case Value::KindCase::kNullValue:
        default:
            return ProtoValue(nullptr);
    }
}

void to_proto_impl<ProtoStruct>::operator()(const ProtoStruct& self,
                                            google::protobuf::Struct* s) const {
    for (const auto& kv : self) {
        s->mutable_fields()->insert(
            google::protobuf::MapPair<std::string, Value>(kv.first, to_proto(kv.second)));
    }
}

ProtoStruct from_proto_impl<google::protobuf::Struct>::operator()(  // NOLINT(misc-no-recursion)
    const google::protobuf::Struct* s) const {
    ProtoStruct result;

    for (const auto& val : s->fields()) {
        result.emplace(val.first, from_proto(val.second));
    }

    return result;
}

void to_proto_impl<DataClient::SequenceResourceFilter>::operator()(
    const DataClient::SequenceResourceFilter& srf, viam::app::data::v1::SequenceResourceFilter* p) const {
    p->set_resource_name(srf.resource_name);
    p->set_method_name(srf.method_name);
}

DataClient::SequenceResourceFilter from_proto_impl<viam::app::data::v1::SequenceResourceFilter>::operator()(
    const viam::app::data::v1::SequenceResourceFilter* p) const {
    DataClient::SequenceResourceFilter srf;
    srf.resource_name = p->resource_name();
    srf.method_name = p->method_name();
    return srf;
}

void to_proto_impl<DataClient::Sequence>::operator()(
    const DataClient::Sequence& s, viam::app::data::v1::Sequence* p) const {
    p->set_id(s.id);
    p->set_part_id(s.part_id);
    for (const auto& tag : s.sequence_tags) {
        *p->mutable_sequence_tags()->Add() = tag;
    }
    *p->mutable_created_at() = to_proto(s.created_at);
    *p->mutable_updated_at() = to_proto(s.updated_at);
    *p->mutable_start_time() = to_proto(s.start_time);
    *p->mutable_end_time() = to_proto(s.end_time);
    for (const auto& r : s.resources) {
        *p->mutable_resources()->Add() = to_proto(r);
    }
}

DataClient::Sequence from_proto_impl<viam::app::data::v1::Sequence>::operator()(
    const viam::app::data::v1::Sequence* p) const {
    DataClient::Sequence s;
    s.id = p->id();
    s.part_id = p->part_id();
    s.sequence_tags = from_repeated_field(p->sequence_tags());
    s.created_at = from_proto(p->created_at());
    s.updated_at = from_proto(p->updated_at());
    s.start_time = from_proto(p->start_time());
    s.end_time = from_proto(p->end_time());
    s.resources = from_repeated_field(p->resources());
    return s;
}

void to_proto_impl<google::protobuf::FieldMask>::operator()(
    const google::protobuf::FieldMask& fm, google::protobuf::FieldMask* p) const {
    for (const auto& path : fm.paths()) {
        *p->mutable_paths()->Add() = path;
    }
}

google::protobuf::FieldMask from_proto_impl<google::protobuf::FieldMask>::operator()(
    const google::protobuf::FieldMask* p) const {
    google::protobuf::FieldMask fm;
    for (const auto& path : p->paths()) {
        *fm.mutable_paths()->Add() = path;
    }
    return fm;
}

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam