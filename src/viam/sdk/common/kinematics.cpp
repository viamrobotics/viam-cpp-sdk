#include <viam/sdk/common/kinematics.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <boost/math/constants/constants.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/private/kinematic_model_validation.hpp>
#include <viam/sdk/common/private/kinematics_response.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>

namespace viam {
namespace sdk {

namespace {

std::string in_quotes(const std::string& s) {
    return "\"" + s + "\"";
}

// Prints a value with as few digits as still tell it apart from its neighbours, so a refusal
// never shows a requested value and the limit it broke as the same number.
std::string number(double value) {
    std::string text;
    for (int digits = 6; digits <= std::numeric_limits<double>::max_digits10; ++digits) {
        std::ostringstream out;
        out.precision(digits);
        out << value;
        text = out.str();
        if (!std::isfinite(value) || std::stod(text) == value) {
            break;
        }
    }
    return text;
}

void check_requested(const std::string& id, const JointLimits& requested) {
    const std::pair<const char*, const std::optional<double>*> fields[] = {
        {"min", &requested.min},
        {"max", &requested.max},
        {"max_velocity", &requested.max_velocity},
        {"max_acceleration", &requested.max_acceleration},
    };
    for (const auto& field : fields) {
        // a field is left out to say a joint is unbounded, so infinity is never a value
        if (*field.second && !std::isfinite(**field.second)) {
            throw Exception("joint " + in_quotes(id) + ": " + field.first +
                            " must be a finite number, got " + number(**field.second));
        }
    }
    if (requested.max_velocity && *requested.max_velocity < 0) {
        throw Exception("joint " + in_quotes(id) + ": max_velocity cannot be negative, got " +
                        number(*requested.max_velocity));
    }
    if (requested.max_acceleration && *requested.max_acceleration < 0) {
        throw Exception("joint " + in_quotes(id) + ": max_acceleration cannot be negative, got " +
                        number(*requested.max_acceleration));
    }
}

// Moves the bounds of one joint to the requested ones, refusing any that would widen what the
// joint holds. A bound the joint does not have is unbounded, so any request narrows it.
void narrow(const std::string& id,
            const JointLimits& requested,
            std::optional<double>& min,
            std::optional<double>& max,
            std::optional<double>& max_velocity,
            std::optional<double>& max_acceleration) {
    if (requested.min && min && *requested.min < *min) {
        throw Exception("joint " + in_quotes(id) + ": min " + number(*requested.min) +
                        " is below the model's min " + number(*min));
    }
    if (requested.max && max && *requested.max > *max) {
        throw Exception("joint " + in_quotes(id) + ": max " + number(*requested.max) +
                        " is above the model's max " + number(*max));
    }
    if (requested.max_velocity && max_velocity && *requested.max_velocity > *max_velocity) {
        throw Exception("joint " + in_quotes(id) + ": max_velocity " +
                        number(*requested.max_velocity) + " is above the model's " +
                        number(*max_velocity));
    }
    if (requested.max_acceleration && max_acceleration &&
        *requested.max_acceleration > *max_acceleration) {
        throw Exception("joint " + in_quotes(id) + ": max_acceleration " +
                        number(*requested.max_acceleration) + " is above the model's " +
                        number(*max_acceleration));
    }
    if (requested.min) {
        min = requested.min;
    }
    if (requested.max) {
        max = requested.max;
    }
    if (requested.max_velocity) {
        max_velocity = requested.max_velocity;
    }
    if (requested.max_acceleration) {
        max_acceleration = requested.max_acceleration;
    }
    if (min && max && *min > *max) {
        throw Exception("joint " + in_quotes(id) + ": min " + number(*min) + " is above max " +
                        number(*max));
    }
}

struct narrow_visitor {
    const std::string& id;
    const JointLimits& requested;

    narrow_visitor(const std::string& joint_id, const JointLimits& joint_limits)
        : id(joint_id), requested(joint_limits) {}

    joint_spec operator()(RevoluteJoint joint) const {
        narrow(id,
               requested,
               joint.min_deg,
               joint.max_deg,
               joint.max_velocity_deg_per_sec,
               joint.max_acceleration_deg_per_sec2);
        return joint;
    }

    joint_spec operator()(const ContinuousJoint& joint) const {
        RevoluteJoint bounded{std::nullopt,
                              std::nullopt,
                              joint.max_velocity_deg_per_sec,
                              joint.max_acceleration_deg_per_sec2};
        narrow(id,
               requested,
               bounded.min_deg,
               bounded.max_deg,
               bounded.max_velocity_deg_per_sec,
               bounded.max_acceleration_deg_per_sec2);
        // the joint stays continuous unless the request gave it a position bound
        if (bounded.min_deg || bounded.max_deg) {
            return bounded;
        }
        return ContinuousJoint{bounded.max_velocity_deg_per_sec,
                               bounded.max_acceleration_deg_per_sec2};
    }

    joint_spec operator()(PrismaticJoint joint) const {
        narrow(id,
               requested,
               joint.min_mm,
               joint.max_mm,
               joint.max_velocity_mm_per_sec,
               joint.max_acceleration_mm_per_sec2);
        return joint;
    }
};

}  // namespace

namespace impl {

void validate_kinematic_model_nodes(const std::vector<kinematic_model_node>& nodes) {
    static const char* const k_world = "world";

    std::map<std::string, std::string> kinds;
    std::map<std::string, std::string> parents;
    for (const auto& n : nodes) {
        if (n.id.empty()) {
            throw Exception(std::string("a ") + n.kind + " has no id");
        }
        if (n.id == k_world) {
            throw Exception(std::string("a ") + n.kind + " cannot be named " + in_quotes(k_world) +
                            ", which is a reserved id");
        }
        const auto existing = kinds.find(n.id);
        if (existing != kinds.end()) {
            if (existing->second == n.kind) {
                throw Exception(std::string(n.kind) + " " + in_quotes(n.id) + " is defined twice");
            }
            throw Exception(in_quotes(n.id) + " names both a " + existing->second + " and a " +
                            n.kind);
        }
        kinds[n.id] = n.kind;
        parents[n.id] = n.parent;
    }

    for (const auto& n : nodes) {
        if (n.parent.empty() || n.parent == k_world) {
            continue;
        }
        if (kinds.find(n.parent) == kinds.end()) {
            throw Exception(std::string(n.kind) + " " + in_quotes(n.id) + ": parent " +
                            in_quotes(n.parent) + " is not a link or joint in the model");
        }
    }

    for (const auto& n : nodes) {
        std::set<std::string> seen;
        for (std::string cur = n.id; !cur.empty() && cur != k_world; cur = parents[cur]) {
            if (!seen.insert(cur).second) {
                throw Exception(kinds[cur] + " " + in_quotes(cur) + " is its own ancestor");
            }
        }
    }

    for (const auto& n : nodes) {
        if (!n.mimics) {
            continue;
        }
        if (*n.mimics == n.id) {
            throw Exception("joint " + in_quotes(n.id) + " mimics itself");
        }
        const auto kind = kinds.find(*n.mimics);
        if (kind == kinds.end() || kind->second != "joint") {
            throw Exception("joint " + in_quotes(n.id) + " mimics " + in_quotes(*n.mimics) +
                            ", which is not a joint in the model");
        }
        // a mimic joint follows the joint it mimics, so limits written on it would be dropped
        // without a word
        if (n.has_limits) {
            throw Exception("joint " + in_quotes(n.id) +
                            " is a mimic joint and cannot carry limits of its own");
        }
    }
}

// The one way around the checks in the public constructor, for a model that came over the wire.
// A client has to be able to hold whatever a server sent it.
struct kinematic_model_access {
    static KinematicModel unchecked(std::string name,
                                    std::vector<KinematicLink> links,
                                    std::vector<KinematicJoint> joints,
                                    std::vector<std::string> output_frames,
                                    std::optional<KinematicProperties> properties) {
        KinematicModel model;
        model.name_ = std::move(name);
        model.links_ = std::move(links);
        model.joints_ = std::move(joints);
        model.output_frames_ = std::move(output_frames);
        model.properties_ = std::move(properties);
        return model;
    }
};

}  // namespace impl

namespace {

bool has_limits(const joint_spec& spec) {
    struct visitor {
        bool operator()(const RevoluteJoint& j) const {
            return j.min_deg || j.max_deg || j.max_velocity_deg_per_sec ||
                   j.max_acceleration_deg_per_sec2;
        }
        bool operator()(const PrismaticJoint& j) const {
            return j.min_mm || j.max_mm || j.max_velocity_mm_per_sec ||
                   j.max_acceleration_mm_per_sec2;
        }
        bool operator()(const ContinuousJoint& j) const {
            return j.max_velocity_deg_per_sec || j.max_acceleration_deg_per_sec2;
        }
    };
    return std::visit(visitor{}, spec);
}

}  // namespace

KinematicModel::KinematicModel(std::string name,
                               std::vector<KinematicLink> links,
                               std::vector<KinematicJoint> joints,
                               std::vector<std::string> output_frames,
                               std::optional<KinematicProperties> properties)
    : name_(std::move(name)),
      links_(std::move(links)),
      joints_(std::move(joints)),
      output_frames_(std::move(output_frames)),
      properties_(std::move(properties)) {
    std::vector<impl::kinematic_model_node> nodes;
    nodes.reserve(links_.size() + joints_.size());
    for (const auto& link : links_) {
        nodes.push_back({"link", link.id, link.parent, std::nullopt, false});
    }
    for (const auto& joint : joints_) {
        nodes.push_back(
            {"joint",
             joint.id,
             joint.parent,
             joint.mimic ? std::optional<std::string>(joint.mimic->joint) : std::nullopt,
             has_limits(joint.spec)});
    }
    impl::validate_kinematic_model_nodes(nodes);
}

const std::string& KinematicModel::name() const {
    return name_;
}

void KinematicModel::set_name(std::string name) {
    name_ = std::move(name);
}

const std::vector<KinematicLink>& KinematicModel::links() const {
    return links_;
}

const std::vector<KinematicJoint>& KinematicModel::joints() const {
    return joints_;
}

const std::vector<std::string>& KinematicModel::output_frames() const {
    return output_frames_;
}

const std::optional<KinematicProperties>& KinematicModel::properties() const {
    return properties_;
}

void KinematicModel::narrow_joint_limits(const std::map<std::string, JointLimits>& limits) {
    // we narrow a copy and move it in at the end, so an error on a later joint leaves the model
    // as it was. The map is ordered, so the joint an error names does not change between runs.
    std::vector<KinematicJoint> narrowed = joints_;
    for (const auto& entry : limits) {
        const std::string& id = entry.first;
        const JointLimits& requested = entry.second;
        check_requested(id, requested);
        const auto joint = std::find_if(
            narrowed.begin(), narrowed.end(), [&](const KinematicJoint& j) { return j.id == id; });
        if (joint == narrowed.end()) {
            throw Exception("no joint " + in_quotes(id) + " in model " + in_quotes(name_));
        }
        if (joint->mimic) {
            throw Exception("joint " + in_quotes(id) +
                            " is a mimic joint and cannot carry limits of its own");
        }
        joint->spec = std::visit(narrow_visitor{id, requested}, joint->spec);
    }
    joints_ = std::move(narrowed);
}

namespace {

// a geometry cannot be changed in place, so one that holds a mesh is rebuilt around a copy of
// the mesh without its bytes
void drop_mesh_bytes(GeometryConfig& geometry) {
    const mesh* const held = std::get_if<mesh>(&geometry.get_geometry_specifics());
    if (!held || held->data.empty()) {
        return;
    }
    mesh stripped = *held;
    stripped.data.clear();
    geometry = GeometryConfig(
        geometry.get_pose(), std::move(stripped), geometry.get_orientation(), geometry.get_label());
}

}  // namespace

void KinematicModel::drop_unrequested_mesh_bytes(const kinematics_options& options) {
    if (!options.include_collision_meshes && !options.include_visual_meshes) {
        return;
    }
    for (auto& link : links_) {
        if (!options.include_collision_meshes) {
            std::for_each(link.collision.begin(), link.collision.end(), drop_mesh_bytes);
        }
        if (!options.include_visual_meshes) {
            std::for_each(link.visual.begin(), link.visual.end(), drop_mesh_bytes);
        }
    }
    if (options.include_collision_meshes) {
        return;
    }
    for (auto& joint : joints_) {
        auto* const prismatic = std::get_if<PrismaticJoint>(&joint.spec);
        if (prismatic && prismatic->geometry) {
            drop_mesh_bytes(*prismatic->geometry);
        }
    }
}

namespace impl {

void set_response_model(const KinematicModel& model,
                        const std::string& resource_name,
                        common::v1::GetKinematicsResponse* response) {
    *response->mutable_model() = to_proto(model);
    response->mutable_model()->set_name(resource_name);
}

}  // namespace impl

namespace proto_convert_details {

namespace {

constexpr double kDegPerRad = 180.0 / boost::math::constants::pi<double>();

std::optional<double> optional_field(bool has, double value) {
    return has ? std::optional<double>(value) : std::nullopt;
}

// Writes the JointLimits message only when at least one bound is set, so an unbounded joint
// round-trips as an absent message.
void set_limits(common::v1::KinematicJoint* proto,
                const std::optional<double>& min,
                const std::optional<double>& max,
                const std::optional<double>& max_velocity,
                const std::optional<double>& max_acceleration) {
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

    std::visit(Visitor{proto}, self.orientation);
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
                       std::nullopt,
                       std::nullopt,
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

    std::visit(Visitor{proto}, self.spec);
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
            PrismaticJoint prismatic{min, max, max_velocity, max_acceleration, std::nullopt};
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
    proto->set_name(self.name());
    *proto->mutable_links() = impl::to_repeated_field(self.links());
    *proto->mutable_joints() = impl::to_repeated_field(self.joints());
    for (const auto& frame : self.output_frames()) {
        proto->add_output_frames(frame);
    }
    const auto& properties = self.properties();
    if (properties) {
        *proto->mutable_properties() = to_proto(*properties);
    }
}

KinematicModel from_proto_impl<common::v1::KinematicModel>::operator()(
    const common::v1::KinematicModel* proto) const {
    return impl::kinematic_model_access::unchecked(
        proto->name(),
        impl::from_repeated_field(proto->links()),
        impl::from_repeated_field(proto->joints()),
        {proto->output_frames().begin(), proto->output_frames().end()},
        proto->has_properties()
            ? std::optional<KinematicProperties>(from_proto(proto->properties()))
            : std::nullopt);
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

    std::visit(visitor, self);
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

}  // namespace sdk
}  // namespace viam
