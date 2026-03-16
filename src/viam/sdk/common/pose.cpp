#include <viam/sdk/common/pose.hpp>

#include <common/v1/common.pb.h>

namespace viam {
namespace sdk {

bool operator==(const pose_orientation& lhs, const pose_orientation& rhs) {
    return lhs.o_x == rhs.o_x && lhs.o_y == rhs.o_y && lhs.o_z == rhs.o_z;
}

bool operator==(const pose_cloud& lhs, const pose_cloud& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.o_x == rhs.o_x &&
           lhs.o_y == rhs.o_y && lhs.o_z == rhs.o_z && lhs.theta == rhs.theta;
}

std::ostream& operator<<(std::ostream& os, const pose& v) {
    os << "{ coordinates: " << v.coordinates << ",\n"
       << "  orientation: " << v.orientation << ",\n"
       << "  theta: " << v.theta << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const pose_cloud& v) {
    os << "{ x: " << v.x << ", y: " << v.y << ", z: " << v.z << ", o_x: " << v.o_x
       << ", o_y: " << v.o_y << ", o_z: " << v.o_z << ", theta: " << v.theta << "}";
    return os;
}

bool operator==(const pose_in_frame& lhs, const pose_in_frame& rhs) {
    return lhs.pose == rhs.pose && lhs.reference_frame == rhs.reference_frame &&
           lhs.goal_cloud == rhs.goal_cloud;
}
std::ostream& operator<<(std::ostream& os, const pose_in_frame& v) {
    os << "{ pose: " << v.pose << ",\n"
       << "  reference_frame: " << v.reference_frame;
    if (v.goal_cloud) {
        os << ",\n  goal_cloud: " << *v.goal_cloud;
    }
    os << "}";
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

void to_proto_impl<pose_cloud>::operator()(const pose_cloud& self, common::v1::PoseCloud* proto) const {
    proto->set_x(self.x);
    proto->set_y(self.y);
    proto->set_z(self.z);
    proto->set_o_x(self.o_x);
    proto->set_o_y(self.o_y);
    proto->set_o_z(self.o_z);
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

pose_cloud from_proto_impl<common::v1::PoseCloud>::operator()(const common::v1::PoseCloud* proto) const {
    pose_cloud pc;
    pc.x = proto->x();
    pc.y = proto->y();
    pc.z = proto->z();
    pc.o_x = proto->o_x();
    pc.o_y = proto->o_y();
    pc.o_z = proto->o_z();
    pc.theta = proto->theta();
    return pc;
}

void to_proto_impl<pose_in_frame>::operator()(const pose_in_frame& self,
                                              common::v1::PoseInFrame* pif) const {
    *(pif->mutable_reference_frame()) = self.reference_frame;
    common::v1::Pose proto_pose;
    to_proto_impl<pose>{}(self.pose, &proto_pose);
    *(pif->mutable_pose()) = std::move(proto_pose);
    if (self.goal_cloud) {
        *(pif->mutable_goal_cloud()) = to_proto(*self.goal_cloud);
    }
};

pose_in_frame from_proto_impl<common::v1::PoseInFrame>::operator()(
    const common::v1::PoseInFrame* proto) const {
    pose_in_frame pif;
    pif.reference_frame = proto->reference_frame();
    pif.pose = from_proto_impl<common::v1::Pose>{}(&(proto->pose()));
    if (proto->has_goal_cloud()) {
        pif.goal_cloud = from_proto(proto->goal_cloud());
    }
    return pif;
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam