#include <viam/sdk/app/data_client_types.hpp>

#include <viam/api/app/data/v1/data.pb.h>

#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

// SequenceResourceFilter
bool operator==(const SequenceResourceFilter& lhs, const SequenceResourceFilter& rhs) {
    return lhs.resource_type == rhs.resource_type && lhs.resource_id == rhs.resource_id &&
           lhs.robot_name == rhs.robot_name && lhs.part_name == rhs.part_name;
}

std::ostream& operator<<(std::ostream& os, const SequenceResourceFilter& v) {
    return os << "SequenceResourceFilter[resource_type=" << v.resource_type
              << ", resource_id=" << v.resource_id << ", robot_name=" << v.robot_name
              << ", part_name=" << v.part_name << "]";
}

namespace proto_convert_details {

void to_proto_impl<SequenceResourceFilter>::operator()(const SequenceResourceFilter& sdk_type,
                                                   viam::app::data::v1::SequenceResourceFilter* proto_type) const {
    proto_type->set_resource_type(sdk_type.resource_type);
    proto_type->set_resource_id(sdk_type.resource_id);
    proto_type->set_robot_name(sdk_type.robot_name);
    proto_type->set_part_name(sdk_type.part_name);
}

SequenceResourceFilter from_proto_impl<viam::app::data::v1::SequenceResourceFilter>::operator()(
    const viam::app::data::v1::SequenceResourceFilter* proto_type) const {
    SequenceResourceFilter sdk_type;
    sdk_type.resource_type = proto_type->resource_type();
    sdk_type.resource_id = proto_type->resource_id();
    sdk_type.robot_name = proto_type->robot_name();
    sdk_type.part_name = proto_type->part_name();
    return sdk_type;
}

// Sequence
void to_proto_impl<Sequence>::operator()(const Sequence& sdk_type,
                                         viam::app::data::v1::Sequence* proto_type) const {
    proto_type->set_id(sdk_type.id);
    *proto_type->mutable_sequence_tags() = sdk::impl::to_repeated_field(sdk_type.sequence_tags);
    *proto_type->mutable_created_at() = to_proto(sdk_type.created_at);
    *proto_type->mutable_last_modified() = to_proto(sdk_type.last_modified);
    *proto_type->mutable_start_time() = to_proto(sdk_type.start_time);
    *proto_type->mutable_end_time() = to_proto(sdk_type.end_time);
    *proto_type->mutable_resources() = sdk::impl::to_repeated_field(sdk_type.resources);
    proto_type->set_part_id(sdk_type.part_id);
    *proto_type->mutable_dataset_ids() = sdk::impl::to_repeated_field(sdk_type.dataset_ids);
}

Sequence from_proto_impl<viam::app::data::v1::Sequence>::operator()(
    const viam::app::data::v1::Sequence* proto_type) const {
    Sequence sdk_type;
    sdk_type.id = proto_type->id();
    sdk_type.sequence_tags = sdk::impl::from_repeated_field(proto_type->sequence_tags());
    sdk_type.created_at = from_proto(proto_type->created_at());
    sdk_type.last_modified = from_proto(proto_type->last_modified());
    sdk_type.start_time = from_proto(proto_type->start_time());
    sdk_type.end_time = from_proto(proto_type->end_time());
    sdk_type.resources = sdk::impl::from_repeated_field(proto_type->resources());
    sdk_type.part_id = proto_type->part_id();
    sdk_type.dataset_ids = sdk::impl::from_repeated_field(proto_type->dataset_ids());
    return sdk_type;
}

}  // namespace proto_convert_details

bool operator==(const Sequence& lhs, const Sequence& rhs) {
    return lhs.id == rhs.id && lhs.sequence_tags == rhs.sequence_tags &&
           lhs.created_at == rhs.created_at && lhs.last_modified == rhs.last_modified &&
           lhs.start_time == rhs.start_time && lhs.end_time == rhs.end_time &&
           lhs.resources == rhs.resources && lhs.part_id == rhs.part_id &&
           lhs.dataset_ids == rhs.dataset_ids;
}

std::ostream& operator<<(std::ostream& os, const Sequence& v) {
    os << "Sequence[id=" << v.id << ", sequence_tags=[";
    for (size_t i = 0; i < v.sequence_tags.size(); ++i) {
        os << v.sequence_tags[i];
        if (i < v.sequence_tags.size() - 1) {
            os << ", ";
        }
    }
    os << "], created_at=" << std::chrono::duration_cast<std::chrono::milliseconds>(
                                   v.created_at.time_since_epoch())
                                   .count()
       << "ms, last_modified="
       << std::chrono::duration_cast<std::chrono::milliseconds>(v.last_modified.time_since_epoch())
              .count()
       << "ms, start_time="
       << std::chrono::duration_cast<std::chrono::milliseconds>(v.start_time.time_since_epoch())
              .count()
       << "ms, end_time="
       << std::chrono::duration_cast<std::chrono::milliseconds>(v.end_time.time_since_epoch())
              .count()
       << "ms, resources=[";
    for (size_t i = 0; i < v.resources.size(); ++i) {
        os << v.resources[i];
        if (i < v.resources.size() - 1) {
            os << ", ";
        }
    }
    os << "], part_id=" << v.part_id << ", dataset_ids=[";
    for (size_t i = 0; i < v.dataset_ids.size(); ++i) {
        os << v.dataset_ids[i];
        if (i < v.dataset_ids.size() - 1) {
            os << ", ";
        }
    }
    os << "]] ";
    return os;
}

}  // namespace sdk
}  // namespace viam