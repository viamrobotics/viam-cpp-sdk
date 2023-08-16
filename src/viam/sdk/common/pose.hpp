#pragma once

#include <common/v1/common.pb.h>

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

    static pose from_proto(const viam::common::v1::Pose& proto);
    viam::common::v1::Pose to_proto() const;

    friend bool operator==(const pose& lhs, const pose& rhs);
    friend std::ostream& operator<<(std::ostream& os, const pose& v);
};

struct pose_in_frame {
    viam::common::v1::PoseInFrame to_proto() const;
    static pose_in_frame from_proto(const viam::common::v1::PoseInFrame& proto);
    pose_in_frame(std::string reference_frame_, struct pose pose_)
        : reference_frame(std::move(reference_frame_)), pose(std::move(pose_)) {}
    pose_in_frame() {}

    std::string reference_frame;
    pose pose;
    friend bool operator==(const pose_in_frame& lhs, const pose_in_frame& rhs);
    friend std::ostream& operator<<(std::ostream& os, const pose_in_frame& v);
};

}  // namespace sdk
}  // namespace viam
