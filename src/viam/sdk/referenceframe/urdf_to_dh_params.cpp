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

#include <viam/sdk/common/exception.hpp>
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

// dh_params.pdf steps 3a, 3b, 3c for a single (axis i, axis i+1) pair
// Inputs:  z0 = a_i,  p0 = p_i,  z1 = a_{i+1},  p1 = p_{i+1}
CommonNormalResult common_normal(const Eigen::Vector3d& z0,
                                 const Eigen::Vector3d& p0,
                                 const Eigen::Vector3d& z1,
                                 const Eigen::Vector3d& p1) {
    CommonNormalResult r{};
    // step 3a: c = a_i x a_{i+1}. Drives the classify branch below.
    const Eigen::Vector3d cross = z0.cross(z1);

    if (cross.norm() < k_axis_parallel_epsilon) {
        // ||c|| equiv to 0  -->  case C
        r.parallel = true;
        // step 3b case C:  d = d_pp = p_{i+1} - p_i
        // perp = component of d_pp perpendicular to a_i  (= p_{i+1} - f_i).
        const Eigen::Vector3d d = p1 - p0;
        const Eigen::Vector3d perp = d - z0 * d.dot(z0);
        const double perp_norm = perp.norm();
        if (perp_norm < k_axis_parallel_epsilon) {
            // degenerate case C: axes coincident, not merely parallel, x_i is undefined
            // chose to signal via zero x_dir so the caller
            // can inherit x from the previous frame.
            r.x_dir = Eigen::Vector3d::Zero();
            r.foot0 = p0;
            r.foot1 = p0;
            return r;
        }
        // step 3c case C: x_i = (p_{i+1} - f_i) / ||p_{i+1} - f_i|| = perp / ||perp||
        r.x_dir = perp / perp_norm;
        r.foot0 = p0;  // f_i = p_i (d_pp dot a_i component lies on the axis itself)
        r.foot1 = r.foot0 + r.x_dir * perp_norm;
        return r;
    }

    // ||c|| > eps  -->  cases A (skew) and B (intersecting)
    // step 3b
    const Eigen::Vector3d d = p1 - p0;                        // d_pp
    const double a = z0.dot(z0);                              // = 1 (unit)
    const double b = z0.dot(z1);                              // a_i dot a_{i+1}
    const double c = z1.dot(z1);                              // = 1
    const double det = a * c - b * b;                         // = |cross|^2 > 0
    const double t0 = (d.dot(z0) * c - d.dot(z1) * b) / det;  // = s
    const double t1 = (d.dot(z0) * b - d.dot(z1) * a) / det;  // = t

    // step 3b feet:  f_i = p_i + s*a_i,   f_{i+1} = p_{i+1} + t*a_{i+1}
    r.foot0 = p0 + z0 * t0;
    r.foot1 = p1 + z1 * t1;
    const Eigen::Vector3d diff = r.foot1 - r.foot0;
    const double diff_norm = diff.norm();
    if (diff_norm < k_axis_parallel_epsilon) {
        // case B (intersecting): feet coincide, so foot-to-foot is undefined
        // step 3c Case B: x_i = (a_i x a_{i+1}) / ||a_i x a_{i+1}||
        r.x_dir = cross / cross.norm();
    } else {
        // case A (skew): step 3c case A: x_i = (f_{i+1} - f_i) / ||f_{i+1} - f_i||
        r.x_dir = diff / diff_norm;
    }
    r.parallel = false;
    return r;
}

// dh_params.pdf step 4: compute one DH row (d, theta, a, alpha) from two
// consecutive DH frames (i-1, i). All inputs in the base frame.
// Inputs:  z_prev = z_{i-1},  x_prev = x_{i-1},  p_prev = o_{i-1}
//          z_curr = z_i,      x_curr = x_i,      p_curr = o_i
//
// dh.tex writes Eq. (a) as (f_{i+1} - f_i).x_i. That equals (o_i - o_{i-1}).x_i
// used below: the offsets between f's and o's lie along z_{i-1}, which vanishes
// under the x_i projection (x_i is perpendicular to z_{i-1} by construction).
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
    const Eigen::Vector3d delta = p_curr - p_prev;  // o_i - o_{i-1}
    // d_i = (o_i - o_{i-1}) dot z_{i-1}
    d = delta.dot(z_prev);
    // a_i = (o_i - o_{i-1}) dot x_i
    a = delta.dot(x_curr);
    // theta_i = atan2((x_{i-1} x x_i) dot z_{i-1},  x_{i-1} dot x_i)
    theta = std::atan2(x_prev.cross(x_curr).dot(z_prev), x_prev.dot(x_curr));
    // alpha_i = atan2((z_{i-1} x z_i) dot x_i, z_{i-1} dot z_i)
    alpha = std::atan2(z_prev.cross(z_curr).dot(x_curr), z_prev.dot(z_curr));
}

// Fail-fast guard for the last DH row.
// Frames 0 through n-1 are built by the common-normal procedure, so they satisfy
// the DH constraints by construction. Frame n is overwritten with the URDF
// tool pose, which the URDF author chose freely so nothing forces it to be DH-compatible.
//
// A DH row T = Rz(theta)*Tz(d)*Tx(a)*Rx(alpha) has only 4 scalars but a rigid
// transform has 6 DoF. The 2 "missing" DoF correspond to two geometric
// constraints that must hold at the frame boundary, both checked here:
//
//   (1) x_n perpendicular to z_{n-1}  -- otherwise no (d,theta,a,alpha) row
//       can represent the orientation step from frame n-1 to frame n.
//   (2) (p_n - o_{n-1}) lies in the (z_{n-1}, x_n) plane -- the row can only
//       translate along z_{n-1} and x_n, so a y-component is unrepresentable.
//
// Without this, a bad URDF produces a DH table that compiles, looks plausible
// but silently disagrees with FK.
void validate_end_effector_dh(const Eigen::Vector3d& z_prev,
                              const Eigen::Vector3d& x_end,
                              const Eigen::Vector3d& origin_prev,
                              const Eigen::Vector3d& p_end) {
    // check (1): x_n . z_{n-1} == 0
    const double perp_dot = x_end.dot(z_prev);
    if (std::abs(perp_dot) > k_dh_compat_epsilon) {
        throw Exception(ErrorCondition::k_general,
                        "URDFToDHParams: end-effector X-axis not perpendicular to last joint axis "
                        "(residual dot = " +
                            std::to_string(perp_dot) + ")");
    }
    // check (2): displacement to tool origin has no y_{n-1} component.
    // y = z_{n-1} x x_n is the plane normal; delta . y must be ~0.
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
        // with reference to dh_params.pdf: Step 1a and 1b are done in one Isometry3d multiply:
        // translation: p_cum_new = p_cum + R_cum * t_i
        // rotation: R_cum_new = R_cum * R_i
        cumulative = cumulative * pose_in_meters(j.origin);

        if (j.type == "fixed") {
        } else if (j.type == "revolute" || j.type == "continuous") {
            try {
                // normalize as we cannot assume unit length
                const Eigen::Vector3d local_axis = axis_unit(j.axis);
                // step 1c: axis is expressed in the child's frame, so use the updated R_cum
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

// Build all n+1 DH frames from the (axes, origins) list produced by
// joint_axes_at_rest. Frame 0 comes from a base-frame projection (dh_params.pdf step 2),
// frames 1 through n from successive common normals (dh_params.pdf step 3), and frame n is
// finally pinned to the URDF tool pose
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

    // n revolute joints means n+1 DH frames
    DHFrames out;
    out.zs.resize(n + 1);
    out.xs.resize(n + 1);
    out.origins.resize(n + 1);

    // dh_params.pdf step 2: Choose Frame 0.
    out.zs[0] = axes[0];
    const Eigen::Vector3d& p0 = origins[0];
    const double t = -p0.dot(axes[0]);  // signed distance along axis from p0 toward (0,0,0)
    out.origins[0] = p0 + axes[0] * t;
    out.xs[0] = pick_base_x(axes[0]);

    // dh_params.pdf step 3: build frame i from the common normal of axis i and axis i+1.
    for (std::size_t i = 1; i <= n; ++i) {
        const Eigen::Vector3d& z_prev = out.zs[i - 1];
        const Eigen::Vector3d& z_curr = all_z[i];
        const Eigen::Vector3d& p_prev = out.origins[i - 1];
        const Eigen::Vector3d& p_curr = all_p[i];

        // steps 3a/3b/3c: classify, find feet f_i and f_{i+1}, derive x_i
        const auto cn = common_normal(z_prev, p_prev, z_curr, p_curr);

        if (cn.x_dir.isZero(k_axis_parallel_epsilon)) {
            // coincident axes (step 3c parallel degenerate)
            // common normal is undefined, so inherit x from the previous frame
            out.xs[i] = out.xs[i - 1];
            out.origins[i] = p_curr;
            out.zs[i] = z_curr;
            continue;
        }

        // sign disambiguation: keep x_i dot x_{i-1} >= 0 so theta_i stays near zero
        Eigen::Vector3d x_dir = cn.x_dir;
        if (x_dir.dot(out.xs[i - 1]) < 0) {
            x_dir = -x_dir;
        }
        // step 3d: assemble frame i.
        // x_i as above
        // o_i = f_i (= cn.foot1)
        // z_i = a_{i+1}
        out.xs[i] = x_dir;
        out.origins[i] = cn.foot1;
        out.zs[i] = z_curr;
    }

    out.origins[n] = end_p;
    out.zs[n] = end_z;
    out.xs[n] = end_x;

    return out;
}

// dh_params.pdf step 2 helper: pick x_0 given z_0, x_0 perpendicular to z_0
// Standard choice is world +x projected onto the plane perp to z, normalized
// Falls back to world +y if z is (anti)parallel to world +x (projection ~0)
Eigen::Vector3d pick_base_x(const Eigen::Vector3d& z) {
    const Eigen::Vector3d world_x(1, 0, 0);
    // remove the component of world_x along z, leaving the perpendicular part
    Eigen::Vector3d proj = world_x - z * z.dot(world_x);
    if (proj.norm() < k_axis_parallel_epsilon) {
        // z parallel to world +x  -->  projection ~ 0. retry with world +y
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

    validate_end_effector_dh(frames.zs[n - 1], frames.xs[n], frames.origins[n - 1], frames.origins[n]);

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
                       frames.origins[i],
                       frames.zs[i + 1],
                       frames.xs[i + 1],
                       frames.origins[i + 1],
                       d,
                       theta,
                       a,
                       alpha);
        result[i] = DHParam{revolute_names[i], d, theta, a, alpha};
    }
    return result;
}

}  // namespace sdk
}  // namespace viam
