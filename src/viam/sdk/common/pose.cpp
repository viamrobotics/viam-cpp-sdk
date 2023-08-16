#include <viam/sdk/common/pose.hpp>

#include <common/v1/common.pb.h>

namespace viam {
namespace sdk {

common::v1::PoseInFrame pose_in_frame::to_proto() const {
    common::v1::PoseInFrame pif;
    *pif.mutable_reference_frame() = reference_frame;
    common::v1::Pose proto_pose;
    proto_pose.set_x(pose.coordinates.x);
    proto_pose.set_y(pose.coordinates.y);
    proto_pose.set_z(pose.coordinates.z);
    proto_pose.set_o_x(pose.orientation.o_x);
    proto_pose.set_o_y(pose.orientation.o_y);
    proto_pose.set_o_z(pose.orientation.o_z);
    proto_pose.set_theta(pose.theta);
    *pif.mutable_pose() = std::move(proto_pose);
    return pif;
};

pose_in_frame pose_in_frame::from_proto(const common::v1::PoseInFrame& proto) {
    pose_in_frame pif;
    pif.reference_frame = proto.reference_frame();
    const auto& proto_pose = proto.pose();
    pif.pose.orientation.o_x = proto_pose.o_x();
    pif.pose.orientation.o_y = proto_pose.o_y();
    pif.pose.orientation.o_z = proto_pose.o_z();
    pif.pose.coordinates.x = proto_pose.x();
    pif.pose.coordinates.y = proto_pose.y();
    pif.pose.coordinates.z = proto_pose.z();
    pif.pose.theta = proto_pose.theta();

    return pif;
}

bool operator==(const pose_in_frame& lhs, const pose_in_frame& rhs) {
    return lhs.pose == rhs.pose && lhs.reference_frame == rhs.reference_frame;
}
std::ostream& operator<<(std::ostream& os, const pose_in_frame& v) {
    os << "{ pose: " << v.pose << ",\n"
       << "  reference_frame: " << v.reference_frame << "}";
    return os;
}

}  // namespace sdk
}  // namespace viam
