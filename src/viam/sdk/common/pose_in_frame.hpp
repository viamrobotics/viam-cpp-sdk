#pragma once

// CR erodkin: fix
#include <common/v1/common.pb.h>

#include <viam/sdk/spatialmath/geometry.hpp>
namespace viam {
namespace sdk {

// CR erodkin: this should be a struct and probably we should move `pose` in here as well
struct PoseInFrame {
    viam::common::v1::PoseInFrame to_proto() const;
    static PoseInFrame from_proto(const viam::common::v1::PoseInFrame& proto);
    PoseInFrame(std::string reference_frame_, struct pose pose_)
        : reference_frame(std::move(reference_frame_)), pose(std::move(pose_)) {}
    PoseInFrame() {}

    std::string reference_frame;
    pose pose;
};

}  // namespace sdk
}  // namespace viam
