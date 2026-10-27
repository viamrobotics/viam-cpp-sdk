#include <viam/sdk/common/kinematics.hpp>

#include <tuple>

#include <boost/math/constants/constants.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>

namespace viam {
namespace sdk {

bool operator==(const LinkPose& lhs, const LinkPose& rhs) {
    return std::tie(lhs.translation, lhs.orientation) == std::tie(rhs.translation, rhs.orientation);
}

bool operator==(const RevoluteJoint& lhs, const RevoluteJoint& rhs) {
    return std::tie(lhs.min_deg,
                    lhs.max_deg,
                    lhs.max_velocity_deg_per_sec,
                    lhs.max_acceleration_deg_per_sec2) ==
           std::tie(rhs.min_deg,
                    rhs.max_deg,
                    rhs.max_velocity_deg_per_sec,
                    rhs.max_acceleration_deg_per_sec2);
}

bool operator==(const ContinuousJoint& lhs, const ContinuousJoint& rhs) {
    return std::tie(lhs.max_velocity_deg_per_sec, lhs.max_acceleration_deg_per_sec2) ==
           std::tie(rhs.max_velocity_deg_per_sec, rhs.max_acceleration_deg_per_sec2);
}

bool operator==(const PrismaticJoint& lhs, const PrismaticJoint& rhs) {
    return std::tie(lhs.min_mm,
                    lhs.max_mm,
                    lhs.max_velocity_mm_per_sec,
                    lhs.max_acceleration_mm_per_sec2,
                    lhs.geometry) == std::tie(rhs.min_mm,
                                              rhs.max_mm,
                                              rhs.max_velocity_mm_per_sec,
                                              rhs.max_acceleration_mm_per_sec2,
                                              rhs.geometry);
}

bool operator==(const MimicJoint& lhs, const MimicJoint& rhs) {
    return std::tie(lhs.joint, lhs.multiplier, lhs.offset) ==
           std::tie(rhs.joint, rhs.multiplier, rhs.offset);
}

bool operator==(const KinematicProperties& lhs, const KinematicProperties& rhs) {
    return std::tie(lhs.operating_freq_hz, lhs.max_linear_tcp_speed_mm_per_sec) ==
           std::tie(rhs.operating_freq_hz, rhs.max_linear_tcp_speed_mm_per_sec);
}

bool operator==(const KinematicLink& lhs, const KinematicLink& rhs) {
    return std::tie(lhs.id, lhs.parent, lhs.pose, lhs.collision, lhs.visual) ==
           std::tie(rhs.id, rhs.parent, rhs.pose, rhs.collision, rhs.visual);
}

bool operator==(const KinematicJoint& lhs, const KinematicJoint& rhs) {
    return std::tie(lhs.id, lhs.parent, lhs.axis, lhs.spec, lhs.mimic) ==
           std::tie(rhs.id, rhs.parent, rhs.axis, rhs.spec, rhs.mimic);
}

bool operator==(const KinematicModel& lhs, const KinematicModel& rhs) {
    return std::tie(lhs.name, lhs.links, lhs.joints, lhs.output_frames, lhs.properties) ==
           std::tie(rhs.name, rhs.links, rhs.joints, rhs.output_frames, rhs.properties);
}

namespace proto_convert_details {

namespace {

constexpr double kDegPerRad = 180.0 / boost::math::constants::pi<double>();

boost::optional<double> optional_field(bool has, double value) {
    return has ? boost::optional<double>(value) : boost::none;
}

// Writes the JointLimits message only when at least one bound is set, so an unbounded joint
// round-trips as an absent message.
void set_limits(common::v1::KinematicJoint* proto,
                const boost::optional<double>& min,
                const boost::optional<double>& max,
                const boost::optional<double>& max_velocity,
                const boost::optional<double>& max_acceleration) {
    if (!min && !max && !max_velocity && !max_acceleration) {
        return;
    }
    auto* limits = proto->mutable_limits();
    if (min) {
        limits->set_min(*min);
    }
    if (max) {
        limits->set_max(*max);
    }
    if (max_velocity) {
        limits->set_max_velocity(*max_velocity);
    }
    if (max_acceleration) {
        limits->set_max_acceleration(*max_acceleration);
    }
}

}  // namespace

void to_proto_impl<LinkPose>::operator()(const LinkPose& self, common::v1::LinkPose* proto) const {
    *proto->mutable_translation() = to_proto(self.translation);

    struct Visitor {
        common::v1::LinkPose* proto;

        void operator()(const quaternion& q) const {
            auto* out = proto->mutable_quaternion();
            out->set_w(q.w);
            out->set_x(q.x);
            out->set_y(q.y);
            out->set_z(q.z);
        }

        void operator()(const orientation_vector_degrees& ov) const {
            auto* out = proto->mutable_orientation_vector();
            out->set_o_x(ov.x);
            out->set_o_y(ov.y);
            out->set_o_z(ov.z);
            out->set_theta(ov.theta);
        }

        void operator()(const orientation_vector& ov) const {
            (*this)(orientation_vector_degrees{ov.x, ov.y, ov.z, ov.theta * kDegPerRad});
        }

        void operator()(const euler_angles& ea) const {
            auto* out = proto->mutable_euler_angles();
            out->set_roll(ea.roll);
            out->set_pitch(ea.pitch);
            out->set_yaw(ea.yaw);
        }

        void operator()(const axis_angles& aa) const {
            auto* out = proto->mutable_axis_angles();
            out->set_rx(aa.x);
            out->set_ry(aa.y);
            out->set_rz(aa.z);
            out->set_theta(aa.theta);
        }
    };

    boost::apply_visitor(Visitor{proto}, self.orientation);
}

LinkPose from_proto_impl<common::v1::LinkPose>::operator()(
    const common::v1::LinkPose* proto) const {
    LinkPose result;
    result.translation = from_proto(proto->translation());
    switch (proto->orientation_case()) {
        case common::v1::LinkPose::OrientationCase::kQuaternion: {
            const auto& q = proto->quaternion();
            result.orientation = quaternion{q.x(), q.y(), q.z(), q.w()};
            break;
        }
        case common::v1::LinkPose::OrientationCase::kOrientationVector: {
            const auto& ov = proto->orientation_vector();
            result.orientation =
                orientation_vector_degrees{ov.o_x(), ov.o_y(), ov.o_z(), ov.theta()};
            break;
        }
        case common::v1::LinkPose::OrientationCase::kEulerAngles: {
            const auto& ea = proto->euler_angles();
            result.orientation = euler_angles{ea.yaw(), ea.roll(), ea.pitch()};
            break;
        }
        case common::v1::LinkPose::OrientationCase::kAxisAngles: {
            const auto& aa = proto->axis_angles();
            result.orientation = axis_angles{aa.rx(), aa.ry(), aa.rz(), aa.theta()};
            break;
        }
        case common::v1::LinkPose::OrientationCase::ORIENTATION_NOT_SET:  // fallthrough
        default:
            break;
    }
    return result;
}

void to_proto_impl<MimicJoint>::operator()(const MimicJoint& self,
                                           common::v1::MimicJoint* proto) const {
    proto->set_joint(self.joint);
    proto->set_multiplier(self.multiplier);
    proto->set_offset(self.offset);
}

MimicJoint from_proto_impl<common::v1::MimicJoint>::operator()(
    const common::v1::MimicJoint* proto) const {
    return MimicJoint{proto->joint(), proto->multiplier(), proto->offset()};
}

void to_proto_impl<KinematicProperties>::operator()(const KinematicProperties& self,
                                                    common::v1::KinematicProperties* proto) const {
    if (self.operating_freq_hz) {
        proto->set_operating_freq_hz(*self.operating_freq_hz);
    }
    if (self.max_linear_tcp_speed_mm_per_sec) {
        proto->set_max_linear_tcp_speed_mm_per_sec(*self.max_linear_tcp_speed_mm_per_sec);
    }
}

KinematicProperties from_proto_impl<common::v1::KinematicProperties>::operator()(
    const common::v1::KinematicProperties* proto) const {
    KinematicProperties result;
    result.operating_freq_hz =
        optional_field(proto->has_operating_freq_hz(), proto->operating_freq_hz());
    result.max_linear_tcp_speed_mm_per_sec = optional_field(
        proto->has_max_linear_tcp_speed_mm_per_sec(), proto->max_linear_tcp_speed_mm_per_sec());
    return result;
}

void to_proto_impl<KinematicLink>::operator()(const KinematicLink& self,
                                              common::v1::KinematicLink* proto) const {
    proto->set_id(self.id);
    proto->set_parent(self.parent);
    *proto->mutable_pose() = to_proto(self.pose);
    *proto->mutable_collision() = impl::to_repeated_field(self.collision);
    *proto->mutable_visual() = impl::to_repeated_field(self.visual);
}

KinematicLink from_proto_impl<common::v1::KinematicLink>::operator()(
    const common::v1::KinematicLink* proto) const {
    KinematicLink result;
    result.id = proto->id();
    result.parent = proto->parent();
    if (proto->has_pose()) {
        result.pose = from_proto(proto->pose());
    }
    result.collision = impl::from_repeated_field(proto->collision());
    result.visual = impl::from_repeated_field(proto->visual());
    return result;
}

void to_proto_impl<KinematicJoint>::operator()(const KinematicJoint& self,
                                               common::v1::KinematicJoint* proto) const {
    proto->set_id(self.id);
    proto->set_parent(self.parent);
    *proto->mutable_axis() = to_proto(self.axis);
    if (self.mimic) {
        *proto->mutable_mimic() = to_proto(*self.mimic);
    }

    struct Visitor {
        common::v1::KinematicJoint* proto;

        void operator()(const RevoluteJoint& j) const {
            proto->set_type(common::v1::JointType::JOINT_TYPE_REVOLUTE);
            set_limits(proto,
                       j.min_deg,
                       j.max_deg,
                       j.max_velocity_deg_per_sec,
                       j.max_acceleration_deg_per_sec2);
        }

        void operator()(const ContinuousJoint& j) const {
            proto->set_type(common::v1::JointType::JOINT_TYPE_CONTINUOUS);
            set_limits(proto,
                       boost::none,
                       boost::none,
                       j.max_velocity_deg_per_sec,
                       j.max_acceleration_deg_per_sec2);
        }

        void operator()(const PrismaticJoint& j) const {
            proto->set_type(common::v1::JointType::JOINT_TYPE_PRISMATIC);
            set_limits(proto,
                       j.min_mm,
                       j.max_mm,
                       j.max_velocity_mm_per_sec,
                       j.max_acceleration_mm_per_sec2);
            if (j.geometry) {
                *proto->mutable_geometry() = to_proto(*j.geometry);
            }
        }
    };

    boost::apply_visitor(Visitor{proto}, self.spec);
}

KinematicJoint from_proto_impl<common::v1::KinematicJoint>::operator()(
    const common::v1::KinematicJoint* proto) const {
    KinematicJoint result;
    result.id = proto->id();
    result.parent = proto->parent();
    result.axis = from_proto(proto->axis());
    if (proto->has_mimic()) {
        result.mimic = from_proto(proto->mimic());
    }

    const bool has_limits = proto->has_limits();
    const auto& limits = proto->limits();
    const auto min = optional_field(has_limits && limits.has_min(), limits.min());
    const auto max = optional_field(has_limits && limits.has_max(), limits.max());
    const auto max_velocity =
        optional_field(has_limits && limits.has_max_velocity(), limits.max_velocity());
    const auto max_acceleration =
        optional_field(has_limits && limits.has_max_acceleration(), limits.max_acceleration());

    switch (proto->type()) {
        case common::v1::JointType::JOINT_TYPE_REVOLUTE:
            result.spec = RevoluteJoint{min, max, max_velocity, max_acceleration};
            break;
        case common::v1::JointType::JOINT_TYPE_CONTINUOUS:
            result.spec = ContinuousJoint{max_velocity, max_acceleration};
            break;
        case common::v1::JointType::JOINT_TYPE_PRISMATIC: {
            PrismaticJoint prismatic{min, max, max_velocity, max_acceleration, boost::none};
            if (proto->has_geometry()) {
                prismatic.geometry = from_proto(proto->geometry());
            }
            result.spec = std::move(prismatic);
            break;
        }
        case common::v1::JointType::JOINT_TYPE_UNSPECIFIED:  // fallthrough
        default:
            throw Exception(ErrorCondition::k_not_supported,
                            "kinematic joint '" + proto->id() + "' has an unspecified type");
    }
    return result;
}

void to_proto_impl<KinematicModel>::operator()(const KinematicModel& self,
                                               common::v1::KinematicModel* proto) const {
    proto->set_name(self.name);
    *proto->mutable_links() = impl::to_repeated_field(self.links);
    *proto->mutable_joints() = impl::to_repeated_field(self.joints);
    for (const auto& frame : self.output_frames) {
        proto->add_output_frames(frame);
    }
    if (self.properties) {
        *proto->mutable_properties() = to_proto(*self.properties);
    }
}

KinematicModel from_proto_impl<common::v1::KinematicModel>::operator()(
    const common::v1::KinematicModel* proto) const {
    KinematicModel result;
    result.name = proto->name();
    result.links = impl::from_repeated_field(proto->links());
    result.joints = impl::from_repeated_field(proto->joints());
    result.output_frames.assign(proto->output_frames().begin(), proto->output_frames().end());
    if (proto->has_properties()) {
        result.properties = from_proto(proto->properties());
    }
    return result;
}

// The raw-bytes response fields and the KinematicsData variant are deprecated, but remain the
// format older viam-server and SDK versions exchange.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void to_proto_impl<KinematicsData>::operator()(const KinematicsData& self,
                                               common::v1::GetKinematicsResponse* proto) const {
    struct Visitor {
        using FileFormat = common::v1::KinematicsFileFormat;
        common::v1::GetKinematicsResponse* proto;

        void set_common(FileFormat format, const std::vector<unsigned char>& bytes) const {
            proto->set_format(format);
            proto->mutable_kinematics_data()->assign(bytes.begin(), bytes.end());
        }

        void operator()(const KinematicsDataUnspecified& v) const {
            set_common(FileFormat::KINEMATICS_FILE_FORMAT_UNSPECIFIED, v.bytes);
        }

        void operator()(const KinematicsDataSVA& v) const {
            set_common(FileFormat::KINEMATICS_FILE_FORMAT_SVA, v.bytes);
        }

        void operator()(const KinematicsDataURDF& v) const {
            set_common(FileFormat::KINEMATICS_FILE_FORMAT_URDF, v.bytes);
            for (const auto& entry : v.meshes_by_urdf_filepath) {
                proto->mutable_meshes_by_urdf_filepath()->insert(
                    {entry.first, to_proto(entry.second)});
            }
        }
    } visitor{proto};

    boost::apply_visitor(visitor, self);
}

KinematicsData from_proto_impl<common::v1::GetKinematicsResponse>::operator()(
    const common::v1::GetKinematicsResponse* proto) const {
    std::vector<unsigned char> bytes(proto->kinematics_data().begin(),
                                     proto->kinematics_data().end());
    switch (proto->format()) {
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_SVA:
            return KinematicsDataSVA(std::move(bytes));
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_URDF: {
            KinematicsDataURDF data(std::move(bytes));
            for (const auto& entry : proto->meshes_by_urdf_filepath()) {
                data.meshes_by_urdf_filepath[entry.first] = from_proto(entry.second);
            }
            return data;
        }
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_UNSPECIFIED:  // fallthrough
        default:
            return KinematicsDataUnspecified{};
    }
}

#pragma GCC diagnostic pop

}  // namespace proto_convert_details

bool operator==(const KinematicsDataURDF& lhs, const KinematicsDataURDF& rhs) {
    return std::tie(lhs.bytes, lhs.meshes_by_urdf_filepath) ==
           std::tie(rhs.bytes, rhs.meshes_by_urdf_filepath);
}

}  // namespace sdk
}  // namespace viam
