#pragma once

#include <chrono>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <viam/sdk/common/proto_convert.hpp>

namespace viam {
namespace sdk {

/// @struct SequenceResourceFilter
/// @brief Represents a filter for sequence resources.
struct SequenceResourceFilter {
    std::string resource_type;
    std::string resource_id;
    std::string robot_name;
    std::string part_name;

    friend bool operator==(const SequenceResourceFilter& lhs, const SequenceResourceFilter& rhs);
    friend std::ostream& operator<<(std::ostream& os, const SequenceResourceFilter& v);
};

/// @struct Sequence
/// @brief Represents a sequence of data.
struct Sequence {
    std::string id;
    std::vector<std::string> sequence_tags;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point last_modified;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    std::vector<SequenceResourceFilter> resources;
    std::string part_id;
    std::vector<std::string> dataset_ids;

    friend bool operator==(const Sequence& lhs, const Sequence& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Sequence& v);
};

namespace proto_convert_details {

template <>
struct to_proto_impl<SequenceResourceFilter> {
    void operator()(const SequenceResourceFilter&,
                    viam::app::data::v1::SequenceResourceFilter*) const;
};

template <>
struct from_proto_impl<viam::app::data::v1::SequenceResourceFilter> {
    SequenceResourceFilter operator()(const viam::app::data::v1::SequenceResourceFilter*) const;
};

template <>
struct to_proto_impl<Sequence> {
    void operator()(const Sequence&, viam::app::data::v1::Sequence*) const;
};

template <>
struct from_proto_impl<viam::app::data::v1::Sequence> {
    Sequence operator()(const viam::app::data::v1::Sequence*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
