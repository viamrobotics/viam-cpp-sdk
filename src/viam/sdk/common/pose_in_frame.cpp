#include <viam/sdk/common/pose_in_frame.hpp>

#include <common/v1/common.pb.h>

namespace viam {
namespace sdk {

common::v1::PoseInFrame PoseInFrame::to_proto() {
    common::v1::PoseInFrame pif;
    *pif.mutable_reference_frame() = reference_frame_;
    common::v1::Pose pose;
    pose.set_x(coordinates_.x);
    pose.set_y(coordinates_.y);
    pose.set_z(coordinates_.z);
    pose.set_o_x(pose_.o_x);
    pose.set_o_y(pose_.o_y);
    pose.set_o_z(pose_.o_z);
    pose.set_theta(theta_);
    *pif.mutable_pose() = std::move(pose);
    return pif;
};

PoseInFrame PoseInFrame::from_proto(const common::v1::PoseInFrame& proto) {
    PoseInFrame pif;
    pif.reference_frame_ = proto.reference_frame();
    const auto& proto_pose = proto.pose();
    pif.pose_.o_x = proto_pose.o_x();
    pif.pose_.o_y = proto_pose.o_y();
    pif.pose_.o_z = proto_pose.o_z();
    pif.coordinates_.x = proto_pose.x();
    pif.coordinates_.y = proto_pose.y();
    pif.coordinates_.z = proto_pose.z();
    pif.theta_ = proto_pose.theta();

    return pif;
}

}  // namespace sdk
}  // namespace viam
