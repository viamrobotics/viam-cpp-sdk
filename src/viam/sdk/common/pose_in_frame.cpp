#include <viam/sdk/common/pose_in_frame.hpp>

#include <common/v1/common.pb.h>

namespace viam {
namespace sdk {

common::v1::PoseInFrame PoseInFrame::to_proto() const {
    common::v1::PoseInFrame pif;
    *pif.mutable_reference_frame() = reference_frame_;
    common::v1::Pose pose;
    pose.set_x(pose_.coordinates.x);
    pose.set_y(pose_.coordinates.y);
    pose.set_z(pose_.coordinates.z);
    pose.set_o_x(pose_.orientation.o_x);
    pose.set_o_y(pose_.orientation.o_y);
    pose.set_o_z(pose_.orientation.o_z);
    pose.set_theta(pose_.theta);
    *pif.mutable_pose() = std::move(pose);
    return pif;
};

PoseInFrame PoseInFrame::from_proto(const common::v1::PoseInFrame& proto) {
    PoseInFrame pif;
    pif.reference_frame_ = proto.reference_frame();
    const auto& proto_pose = proto.pose();
    pif.pose_.orientation.o_x = proto_pose.o_x();
    pif.pose_.orientation.o_y = proto_pose.o_y();
    pif.pose_.orientation.o_z = proto_pose.o_z();
    pif.pose_.coordinates.x = proto_pose.x();
    pif.pose_.coordinates.y = proto_pose.y();
    pif.pose_.coordinates.z = proto_pose.z();
    pif.pose_.theta = proto_pose.theta();

    return pif;
}

}  // namespace sdk
}  // namespace viam
