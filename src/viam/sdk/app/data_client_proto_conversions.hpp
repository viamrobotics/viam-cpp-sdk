#pragma once

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <viam/api/app/data/v1/data.grpc.pb.h>
#include <viam/api/app/data/v1/data.pb.h>
#include <viam/api/google/protobuf/field_mask.pb.h>
#include <viam/api/google/protobuf/timestamp.pb.h>

#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

// SDK struct for SequenceResourceFilter
struct SequenceResourceFilter {
    std::string part_id;
    std::string resource_name;
    std::string method_name;

    friend bool operator==(const SequenceResourceFilter& lhs, const SequenceResourceFilter& rhs);
};

// SDK struct for Sequence
struct Sequence {
    std::string id;
    std::string organization_id;
    std::vector<std::string> sequence_tags;
    time_pt created_at;
    time_pt updated_at;
    time_pt start_time;
    time_pt end_time;
    std::vector<SequenceResourceFilter> resources;

    friend bool operator==(const Sequence& lhs, const Sequence& rhs);
};

namespace proto_convert_details {

// Conversions for SequenceResourceFilter
template <>
struct to_proto_impl<SequenceResourceFilter> {
    void operator()(const SequenceResourceFilter&, viam::app::data::v1::SequenceResourceFilter*) const;
};

template <>
struct from_proto_impl<viam::app::data::v1::SequenceResourceFilter> {
    SequenceResourceFilter operator()(const viam::app::data::v1::SequenceResourceFilter*) const;
};

// Conversions for Sequence
template <>
struct to_proto_impl<Sequence> {
    void operator()(const Sequence&, viam::app::data::v1::Sequence*) const;
};

template <>
struct from_proto_impl<viam::app::data::v1::Sequence> {
    Sequence operator()(const viam::app::data::v1::Sequence*) const;
};

// Conversions for google::protobuf::FieldMask
template <>
struct to_proto_impl<std::vector<std::string>> {
    void operator()(const std::vector<std::string>&, google::protobuf::FieldMask*) const;
};

template <>
struct from_proto_impl<google::protobuf::FieldMask> {
    std::vector<std::string> operator()(const google::protobuf::FieldMask*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
