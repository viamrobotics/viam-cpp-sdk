#pragma once

#include <viam/sdk/common/proto_convert.hpp>

#include <ostream>

VIAM_SDK_API_FWD_NAMESPACE_BEGIN(common)

class Pose;
class PoseInFrame;

VIAM_SDK_API_FWD_NAMESPACE_END

namespace viam {
namespace sdk {

struct coordinates {
    double x, y, z;
    friend bool operator==(const coordinates& lhs, const coordinates& rhs);
};

struct pose_orientation {
    double o_x, o_y, o_z;
    friend bool operator==(const pose_orientation& lhs, const pose_orientation& rhs);
};

struct pose {
    struct coordinates coordinates;
    pose_orientation orientation;
    double theta;

    friend bool operator==(const pose& lhs, const pose& rhs);
    friend std::ostream& operator<<(std::ostream& os, const pose& v);
};

struct pose_in_frame {
    pose_in_frame(std::string reference_frame_, struct pose pose_)
        : reference_frame(std::move(reference_frame_)), pose(std::move(pose_)) {}
    pose_in_frame() {}

    std::string reference_frame;
    struct pose pose;
    friend bool operator==(const pose_in_frame& lhs, const pose_in_frame& rhs);
    friend std::ostream& operator<<(std::ostream& os, const pose_in_frame& v);
};

namespace proto_convert_details {

template <>
struct to_proto<pose> {
    void operator()(const pose&, common::v1::Pose*) const;
};

template <>
struct from_proto<common::v1::Pose> {
    pose operator()(const common::v1::Pose*) const;
};

template <>
struct to_proto<pose_in_frame> {
    void operator()(const pose_in_frame&, common::v1::PoseInFrame*) const;
};

template <>
struct from_proto<common::v1::PoseInFrame> {
    pose_in_frame operator()(const common::v1::PoseInFrame*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
