#include <viam/sdk/app/data_client_proto_conversions.hpp>

namespace viam {
namespace sdk {

// Equality for SequenceResourceFilter
bool operator==(const SequenceResourceFilter& lhs, const SequenceResourceFilter& rhs) {
    return lhs.part_id == rhs.part_id && lhs.resource_name == rhs.resource_name &&
           lhs.method_name == rhs.method_name;
}

// Equality for Sequence
bool operator==(const Sequence& lhs, const Sequence& rhs) {
    return lhs.id == rhs.id && lhs.organization_id == rhs.organization_id &&
           lhs.sequence_tags == rhs.sequence_tags && lhs.created_at == rhs.created_at &&
           lhs.updated_at == rhs.updated_at && lhs.start_time == rhs.start_time &&
           lhs.end_time == rhs.end_time && lhs.resources == rhs.resources;
}

namespace proto_convert_details {

// Conversions for SequenceResourceFilter
void to_proto_impl<SequenceResourceFilter>::operator()(
    const SequenceResourceFilter& sdk_type, viam::app::data::v1::SequenceResourceFilter* proto_type) const {
    proto_type->set_part_id(sdk_type.part_id);
    proto_type->set_resource_name(sdk_type.resource_name);
    proto_type->set_method_name(sdk_type.method_name);
}

SequenceResourceFilter from_proto_impl<viam::app::data::v1::SequenceResourceFilter>::operator()(
    const viam::app::data::v1::SequenceResourceFilter* proto_type) const {
    SequenceResourceFilter sdk_type;
    sdk_type.part_id = proto_type->part_id();
    sdk_type.resource_name = proto_type->resource_name();
    sdk_type.method_name = proto_type->method_name();
    return sdk_type;
}

// Conversions for Sequence
void to_proto_impl<Sequence>::operator()(const Sequence& sdk_type, viam::app::data::v1::Sequence* proto_type) const {
    proto_type->set_id(sdk_type.id);
    proto_type->set_organization_id(sdk_type.organization_id);
    for (const auto& tag : sdk_type.sequence_tags) {
        proto_type->add_sequence_tags(tag);
    }
    *proto_type->mutable_created_at() = to_proto(sdk_type.created_at);
    *proto_type->mutable_updated_at() = to_proto(sdk_type.updated_at);
    *proto_type->mutable_start_time() = to_proto(sdk_type.start_time);
    *proto_type->mutable_end_time() = to_proto(sdk_type.end_time);
    for (const auto& resource : sdk_type.resources) {
        *proto_type->add_resources() = to_proto(resource);
    }
}

Sequence from_proto_impl<viam::app::data::v1::Sequence>::operator()(
    const viam::app::data::v1::Sequence* proto_type) const {
    Sequence sdk_type;
    sdk_type.id = proto_type->id();
    sdk_type.organization_id = proto_type->organization_id();
    for (const auto& tag : proto_type->sequence_tags()) {
        sdk_type.sequence_tags.push_back(tag);
    }
    sdk_type.created_at = from_proto(proto_type->created_at());
    sdk_type.updated_at = from_proto(proto_type->updated_at());
    sdk_type.start_time = from_proto(proto_type->start_time());
    sdk_type.end_time = from_proto(proto_type->end_time());
    for (const auto& resource : proto_type->resources()) {
        sdk_type.resources.push_back(from_proto(resource));
    }
    return sdk_type;
}

// Conversions for google::protobuf::FieldMask
void to_proto_impl<std::vector<std::string>>::operator()(
    const std::vector<std::string>& sdk_type, google::protobuf::FieldMask* proto_type) const {
    for (const auto& path : sdk_type) {
        proto_type->add_paths(path);
    }
}

std::vector<std::string> from_proto_impl<google::protobuf::FieldMask>::operator()(
    const google::protobuf::FieldMask* proto_type) const {
    std::vector<std::string> sdk_type;
    for (const auto& path : proto_type->paths()) {
        sdk_type.push_back(path);
    }
    return sdk_type;
}

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam