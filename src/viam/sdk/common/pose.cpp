#include <viam/sdk/common/pose.hpp>

#include <common/v1/common.pb.h>

namespace viam {
namespace sdk {

bool operator==(const pose_in_frame& lhs, const pose_in_frame& rhs) {
    return lhs.pose == rhs.pose && lhs.reference_frame == rhs.reference_frame;
}
std::ostream& operator<<(std::ostream& os, const pose_in_frame& v) {
    os << "{ pose: " << v.pose << ",\n"
       << "  reference_frame: " << v.reference_frame << "}";
    return os;
}

namespace proto_convert_details {

void to_proto_impl<pose>::operator()(const pose& self, common::v1::Pose* proto) const {
    proto->set_x(self.coordinates.x);
    proto->set_y(self.coordinates.y);
    proto->set_z(self.coordinates.z);
    proto->set_o_x(self.orientation.o_x);
    proto->set_o_y(self.orientation.o_y);
    proto->set_o_z(self.orientation.o_z);
    proto->set_theta(self.theta);
}

pose from_proto_impl<common::v1::Pose>::operator()(const common::v1::Pose* proto) const {
    pose pose;
    pose.coordinates.x = proto->x();
    pose.coordinates.y = proto->y();
    pose.coordinates.z = proto->z();
    pose.orientation.o_x = proto->o_x();
    pose.orientation.o_y = proto->o_y();
    pose.orientation.o_z = proto->o_z();
    pose.theta = proto->theta();

    return pose;
}

void to_proto_impl<pose_in_frame>::operator()(const pose_in_frame& self,
                                              common::v1::PoseInFrame* pif) const {
    *(pif->mutable_reference_frame()) = self.reference_frame;
    common::v1::Pose proto_pose;
    to_proto_impl<pose>{}(self.pose, &proto_pose);
    *(pif->mutable_pose()) = std::move(proto_pose);
};

pose_in_frame from_proto_impl<common::v1::PoseInFrame>::operator()(
    const common::v1::PoseInFrame* proto) const {
    pose_in_frame pif;
    pif.reference_frame = proto->reference_frame();
    pif.pose = from_proto_impl<common::v1::Pose>{}(&(proto->pose()));

    return pif;
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
