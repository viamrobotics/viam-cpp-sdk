#include <viam/sdk/referenceframe/urdf_to_dh_params.hpp>

#include <cmath>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/variant/get.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/components/arm.hpp>
#include <viam/sdk/referenceframe/private/urdf_to_dh_internals.hpp>

namespace viam {
namespace sdk {
namespace urdf_to_dh_internals {

namespace {

// Parse "x y z" (space-delimited) to Vector3d. Throws if fewer than 3 tokens.
Eigen::Vector3d parse_triple(const std::string& s) {
    std::istringstream iss(s);
    double x = 0, y = 0, z = 0;
    if (!(iss >> x >> y >> z)) {
        throw Exception(ErrorCondition::k_general,
                        "URDFToDHParams: failed to parse space-delimited triple: '" + s + "'");
    }
    return Eigen::Vector3d(x, y, z);
}

// URDF RPY -> rotation matrix (R = Rz(yaw) * Ry(pitch) * Rx(roll)).
Eigen::Matrix3d rpy_to_rotation(double roll, double pitch, double yaw) {
    return (Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
            Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()) *
            Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX()))
        .toRotationMatrix();
}

}  // namespace

std::vector<Joint> parse_urdf(const KinematicsDataURDF& urdf) {
    namespace pt = boost::property_tree;
    pt::ptree tree;

    std::string text(reinterpret_cast<const char*>(urdf.bytes.data()), urdf.bytes.size());
    std::istringstream iss(text);
    try {
        pt::read_xml(iss, tree);
    } catch (const pt::xml_parser_error& e) {
        throw Exception(ErrorCondition::k_general,
                        std::string("URDFToDHParams: failed to parse URDF XML: ") + e.what());
    }

    const auto robot_it = tree.find("robot");
    if (robot_it == tree.not_found()) {
        throw Exception(ErrorCondition::k_general,
                        "URDFToDHParams: no <robot> root element in URDF");
    }
    const pt::ptree& robot = robot_it->second;

    std::vector<Joint> joints;
    for (const auto& child : robot) {
        if (child.first != "joint") {
            continue;
        }
        const pt::ptree& jnode = child.second;

        Joint j;
        j.name = jnode.get<std::string>("<xmlattr>.name", "");
        j.type = jnode.get<std::string>("<xmlattr>.type", "");
        j.parent_link = jnode.get<std::string>("parent.<xmlattr>.link", "");
        j.child_link = jnode.get<std::string>("child.<xmlattr>.link", "");

        const auto origin_child = jnode.get_child_optional("origin");
        if (origin_child) {
            const std::string xyz = origin_child->get<std::string>("<xmlattr>.xyz", "0 0 0");
            const std::string rpy = origin_child->get<std::string>("<xmlattr>.rpy", "0 0 0");
            const Eigen::Vector3d t = parse_triple(xyz);
            const Eigen::Vector3d r = parse_triple(rpy);
            Eigen::Isometry3d iso = Eigen::Isometry3d::Identity();
            iso.linear() = rpy_to_rotation(r.x(), r.y(), r.z());
            iso.translation() = t;
            j.origin = iso;
        }

        const auto axis_child = jnode.get_child_optional("axis");
        if (axis_child) {
            const std::string xyz = axis_child->get<std::string>("<xmlattr>.xyz", "1 0 0");
            j.axis = parse_triple(xyz);
        }
        joints.push_back(std::move(j));
    }
    return joints;
}

std::vector<Joint> walk_urdf_chain(const std::vector<Joint>& joints) {
    // Index joints by parent-link name.
    std::map<std::string, std::vector<const Joint*>> by_parent;
    std::set<std::string> all_parents;
    std::set<std::string> all_children;
    for (const auto& j : joints) {
        by_parent[j.parent_link].push_back(&j);
        all_parents.insert(j.parent_link);
        all_children.insert(j.child_link);
    }

    // A root link is any parent that is never a child.
    std::vector<std::string> roots;
    for (const auto& p : all_parents) {
        if (all_children.find(p) == all_children.end()) {
            roots.push_back(p);
        }
    }
    if (roots.size() != 1) {
        std::string list;
        for (const auto& r : roots) {
            list += "'" + r + "' ";
        }
        throw Exception(ErrorCondition::k_general,
                        "URDFToDHParams: expected exactly one root link, found " +
                            std::to_string(roots.size()) + ": " + list);
    }

    std::vector<Joint> ordered;
    ordered.reserve(joints.size());
    std::string current = roots[0];
    while (true) {
        auto it = by_parent.find(current);
        if (it == by_parent.end() || it->second.empty()) {
            break;  // reached leaf
        }
        if (it->second.size() > 1) {
            throw Exception(ErrorCondition::k_general,
                            "URDFToDHParams: branching topology at link '" + current + "' (has " +
                                std::to_string(it->second.size()) + " outgoing joints)");
        }
        const Joint* j = it->second.front();
        ordered.push_back(*j);
        current = j->child_link;
    }
    if (ordered.size() != joints.size()) {
        throw Exception(ErrorCondition::k_general,
                        "URDFToDHParams: chain walk visited " + std::to_string(ordered.size()) +
                            " of " + std::to_string(joints.size()) +
                            " joints; URDF may be disconnected");
    }
    return ordered;
}

Eigen::Isometry3d pose_in_meters(const boost::optional<Eigen::Isometry3d>& origin) {
    return origin ? *origin : Eigen::Isometry3d::Identity();
}

Eigen::Vector3d axis_unit(const boost::optional<Eigen::Vector3d>& axis) {
    if (!axis) {
        return Eigen::Vector3d(1.0, 0.0, 0.0);  // URDF default
    }
    const double norm = axis->norm();
    if (norm < k_axis_parallel_epsilon) {
        throw Exception(ErrorCondition::k_general, "URDFToDHParams: joint axis has zero magnitude");
    }
    return *axis / norm;
}

CommonNormalResult common_normal(const Eigen::Vector3d& z0,
                                 const Eigen::Vector3d& p0,
                                 const Eigen::Vector3d& z1,
                                 const Eigen::Vector3d& p1) {
    CommonNormalResult r{};
    const Eigen::Vector3d cross = z0.cross(z1);

    if (cross.norm() < k_axis_parallel_epsilon) {
        r.parallel = true;
        // Perpendicular from line0 to line1: project (p1-p0) onto plane perp. to z0.
        const Eigen::Vector3d d = p1 - p0;
        const Eigen::Vector3d perp = d - z0 * d.dot(z0);
        const double perp_norm = perp.norm();
        if (perp_norm < k_axis_parallel_epsilon) {
            // Coincident lines.
            r.x_dir = Eigen::Vector3d::Zero();
            r.foot0 = p0;
            r.foot1 = p0;
            return r;
        }
        r.x_dir = perp / perp_norm;
        r.foot0 = p0;
        r.foot1 = r.foot0 + r.x_dir * perp_norm;
        return r;
    }

    // Non-parallel lines: skew-line closest-point formula.
    const Eigen::Vector3d d = p1 - p0;
    const double a = z0.dot(z0);  // = 1 (unit)
    const double b = z0.dot(z1);
    const double c = z1.dot(z1);       // = 1
    const double det = a * c - b * b;  // = |cross|^2 > 0
    const double t0 = (d.dot(z0) * c - d.dot(z1) * b) / det;
    const double t1 = (d.dot(z0) * b - d.dot(z1) * a) / det;

    r.foot0 = p0 + z0 * t0;
    r.foot1 = p1 + z1 * t1;
    const Eigen::Vector3d diff = r.foot1 - r.foot0;
    const double diff_norm = diff.norm();
    if (diff_norm < k_axis_parallel_epsilon) {
        // Lines intersect; use cross as the common-normal direction.
        r.x_dir = cross / cross.norm();
    } else {
        r.x_dir = diff / diff_norm;
    }
    r.parallel = false;
    return r;
}

void extract_dh_row(const Eigen::Vector3d& z_prev,
                    const Eigen::Vector3d& x_prev,
                    const Eigen::Vector3d& p_prev,
                    const Eigen::Vector3d& z_curr,
                    const Eigen::Vector3d& x_curr,
                    const Eigen::Vector3d& p_curr,
                    double& d,
                    double& theta,
                    double& a,
                    double& alpha) {
    const Eigen::Vector3d delta = p_curr - p_prev;
    d = delta.dot(z_prev);
    a = delta.dot(x_curr);
    theta = std::atan2(x_prev.cross(x_curr).dot(z_prev), x_prev.dot(x_curr));
    alpha = std::atan2(z_prev.cross(z_curr).dot(x_curr), z_prev.dot(z_curr));
}

void validate_end_effector_dh(const Eigen::Vector3d& z_prev,
                              const Eigen::Vector3d& x_end,
                              const Eigen::Vector3d& origin_prev,
                              const Eigen::Vector3d& p_end) {
    const double perp_dot = x_end.dot(z_prev);
    if (std::abs(perp_dot) > k_dh_compat_epsilon) {
        throw Exception(ErrorCondition::k_general,
                        "URDFToDHParams: end-effector X-axis not perpendicular to last joint axis "
                        "(residual dot = " +
                            std::to_string(perp_dot) + ")");
    }
    const Eigen::Vector3d delta = p_end - origin_prev;
    const Eigen::Vector3d y_dir = z_prev.cross(x_end);
    const double plane_residual = delta.dot(y_dir);
    if (std::abs(plane_residual) > k_dh_compat_epsilon) {
        throw Exception(ErrorCondition::k_general,
                        "URDFToDHParams: end-effector origin out of DH plane (residual along y = " +
                            std::to_string(plane_residual) + ")");
    }
}

// Walk the joint list in chain order and maintain a base to currentl link transform.
// For all revolute or continuous joints record two values in the base frame:
// (1) the joint origin p_i and (2) the joint axis direction a_i
JointAxesAtRest joint_axes_at_rest(const std::vector<Joint>& joints) {
    JointAxesAtRest out;
    // initialize the running transformation
    // stores rotation in .linear() and translation in .translation()
    Eigen::Isometry3d cumulative = Eigen::Isometry3d::Identity();

    for (const auto& j : joints) {
        // With reference to dh_params.pdf: Step 1 substeps (a) and (b) are done in one Isometry3d multiply:
        // translation: p_cum_new = p_cum + R_cum * t_i
        // rotation: R_cum_new = R_cum * R_i
        cumulative = cumulative * pose_in_meters(j.origin);

        if (j.type == "fixed") {
            // Accumulated only; no joint frame.
        } else if (j.type == "revolute" || j.type == "continuous") {
            try {
                // normalize as we cannot assume unit length
                const Eigen::Vector3d local_axis = axis_unit(j.axis);
                // substep (c): axis is expressed in the child's frame, so use the updated R_cum
                const Eigen::Vector3d world_axis = cumulative.linear() * local_axis;
                // record (p_i, a_i) in the base frame
                out.axes.push_back(world_axis);
                out.origins.push_back(cumulative.translation());
            } catch (const Exception& e) {
                throw Exception(ErrorCondition::k_general,
                                "URDFToDHParams: joint '" + j.name + "': " + e.what());
            }
        } else {
            throw Exception(ErrorCondition::k_not_supported,
                            "URDFToDHParams: joint '" + j.name + "' has unsupported type '" +
                                j.type + "' (only revolute, continuous, and fixed are supported)");
        }
    }

    if (out.axes.empty()) {
        throw Exception(ErrorCondition::k_general, "URDFToDHParams: no revolute joints in chain");
    }
    // full base-to-leaf transform, this includes any trailing fixed joints like tool0
    out.end_pose = cumulative;
    return out;
}

DHFrames build_dh_frames(const std::vector<Eigen::Vector3d>& axes,
                         const std::vector<Eigen::Vector3d>& origins,
                         const Eigen::Isometry3d& end_pose) {
    const std::size_t n = axes.size();

    const Eigen::Vector3d end_z = end_pose.linear() * Eigen::Vector3d(0, 0, 1);
    const Eigen::Vector3d end_x = end_pose.linear() * Eigen::Vector3d(1, 0, 0);
    const Eigen::Vector3d end_p = end_pose.translation();

    std::vector<Eigen::Vector3d> all_z(axes.begin(), axes.end());
    all_z.push_back(end_z);
    std::vector<Eigen::Vector3d> all_p(origins.begin(), origins.end());
    all_p.push_back(end_p);

    DHFrames out;
    out.zs.resize(n + 1);
    out.xs.resize(n + 1);
    out.pts.resize(n + 1);

    // Frame 0: Z = joint-1 axis; origin = point on axis closest to world origin.
    out.zs[0] = axes[0];
    const Eigen::Vector3d& p0 = origins[0];
    const double t = -p0.dot(axes[0]);
    out.pts[0] = p0 + axes[0] * t;
    out.xs[0] = pick_base_x(axes[0]);

    // Frames 1..N via common normal with previous Z.
    for (std::size_t i = 1; i <= n; ++i) {
        const Eigen::Vector3d& z_prev = out.zs[i - 1];
        const Eigen::Vector3d& z_curr = all_z[i];
        const Eigen::Vector3d& p_prev = out.pts[i - 1];
        const Eigen::Vector3d& p_curr = all_p[i];

        const auto cn = common_normal(z_prev, p_prev, z_curr, p_curr);

        if (cn.x_dir.isZero(k_axis_parallel_epsilon)) {
            // Coincident axes: keep previous X direction.
            out.xs[i] = out.xs[i - 1];
            out.pts[i] = p_curr;
            out.zs[i] = z_curr;
            continue;
        }

        Eigen::Vector3d x_dir = cn.x_dir;
        if (x_dir.dot(out.xs[i - 1]) < 0) {
            x_dir = -x_dir;
        }
        out.xs[i] = x_dir;
        out.pts[i] = cn.foot1;
        out.zs[i] = z_curr;
    }

    // Final frame N overwrites the loop's canonical placement so that frame N
    // coincides with the URDF's end-effector frame. Consistency is validated
    // separately in validate_end_effector_dh.
    out.pts[n] = end_p;
    out.zs[n] = end_z;
    out.xs[n] = end_x;

    return out;
}

Eigen::Vector3d pick_base_x(const Eigen::Vector3d& z) {
    const Eigen::Vector3d world_x(1, 0, 0);
    Eigen::Vector3d proj = world_x - z * z.dot(world_x);
    if (proj.norm() < k_axis_parallel_epsilon) {
        const Eigen::Vector3d world_y(0, 1, 0);
        proj = world_y - z * z.dot(world_y);
    }
    return proj.normalized();
}

}  // namespace urdf_to_dh_internals

std::vector<DHParam> urdf_to_dh_params(const KinematicsDataURDF& urdf) {
    using namespace urdf_to_dh_internals;

    auto chain = walk_urdf_chain(parse_urdf(urdf));
    auto axes_origins = joint_axes_at_rest(chain);
    const std::size_t n = axes_origins.axes.size();

    auto frames = build_dh_frames(axes_origins.axes, axes_origins.origins, axes_origins.end_pose);

    validate_end_effector_dh(frames.zs[n - 1], frames.xs[n], frames.pts[n - 1], frames.pts[n]);

    // Revolute joint names in chain order (matches joint_axes_at_rest filter).
    std::vector<std::string> revolute_names;
    revolute_names.reserve(n);
    for (const auto& j : chain) {
        if (j.type == "revolute" || j.type == "continuous") {
            revolute_names.push_back(j.name);
        }
    }

    std::vector<DHParam> result(n);
    for (std::size_t i = 0; i < n; ++i) {
        double d, theta, a, alpha;
        extract_dh_row(frames.zs[i],
                       frames.xs[i],
                       frames.pts[i],
                       frames.zs[i + 1],
                       frames.xs[i + 1],
                       frames.pts[i + 1],
                       d,
                       theta,
                       a,
                       alpha);
        result[i] = DHParam{revolute_names[i], d, theta, a, alpha};
    }
    return result;
}

std::vector<DHParam> urdf_to_dh_params(Arm& arm) {
    KinematicsData kin = arm.get_kinematics();
    const KinematicsDataURDF* urdf = boost::get<KinematicsDataURDF>(&kin);
    if (urdf == nullptr) {
        throw Exception(ErrorCondition::k_not_supported,
                        "urdf_to_dh_params(Arm&): kinematics format is not URDF");
    }
    return urdf_to_dh_params(*urdf);
}

}  // namespace sdk
}  // namespace viam
