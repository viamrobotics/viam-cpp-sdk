/// @file referenceframe/private/urdf_to_dh_internals.hpp
/// @brief Internal helpers for urdf_to_dh_params. Exposed only to the
///        test suite; not part of the installed public API.
#pragma once

#include <string>
#include <vector>

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <boost/optional/optional.hpp>

#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/referenceframe/urdf_to_dh_params.hpp>

namespace viam {
namespace sdk {
namespace urdf_to_dh_internals {

constexpr double k_axis_parallel_epsilon = 1e-9;
constexpr double k_dh_compat_epsilon = 1e-6;

/// @brief Parsed joint element from a URDF (subset needed for DH extraction).
struct Joint {
    std::string name;
    std::string type;  // "revolute", "continuous", "fixed", ...
    std::string parent_link;
    std::string child_link;
    boost::optional<Eigen::Isometry3d> origin;  // none == identity
    boost::optional<Eigen::Vector3d> axis;      // none == URDF default (1,0,0)
};

/// @brief Result of walking joint axes at rest along a chain.
struct JointAxesAtRest {
    std::vector<Eigen::Vector3d> axes;     // revolute joint axes in world
    std::vector<Eigen::Vector3d> origins;  // revolute joint origins in world
    Eigen::Isometry3d end_pose;            // final link pose in world
};

/// @brief Result of build_dh_frames: frames 0..N, each length N+1.
struct DHFrames {
    std::vector<Eigen::Vector3d> zs;
    std::vector<Eigen::Vector3d> xs;
    std::vector<Eigen::Vector3d> origins;
};

/// @brief Result of common_normal: direction, feet on each line, and
///        whether the two axes are (anti-)parallel.
struct CommonNormalResult {
    Eigen::Vector3d x_dir;  // zero vector if the two lines are coincident
    Eigen::Vector3d foot0;
    Eigen::Vector3d foot1;
    bool parallel;
};

std::vector<Joint> parse_urdf(const KinematicsDataURDF& urdf);

std::vector<Joint> walk_urdf_chain(const std::vector<Joint>& joints);

Eigen::Isometry3d pose_in_meters(const boost::optional<Eigen::Isometry3d>& origin);

Eigen::Vector3d axis_unit(const boost::optional<Eigen::Vector3d>& axis);

JointAxesAtRest joint_axes_at_rest(const std::vector<Joint>& joints);

CommonNormalResult common_normal(const Eigen::Vector3d& z0,
                                 const Eigen::Vector3d& p0,
                                 const Eigen::Vector3d& z1,
                                 const Eigen::Vector3d& p1);

DHFrames build_dh_frames(const std::vector<Eigen::Vector3d>& axes,
                         const std::vector<Eigen::Vector3d>& origins,
                         const Eigen::Isometry3d& end_pose);

void extract_dh_row(const Eigen::Vector3d& z_prev,
                    const Eigen::Vector3d& x_prev,
                    const Eigen::Vector3d& p_prev,
                    const Eigen::Vector3d& z_curr,
                    const Eigen::Vector3d& x_curr,
                    const Eigen::Vector3d& p_curr,
                    double& d,
                    double& theta,
                    double& a,
                    double& alpha);

void validate_end_effector_dh(const Eigen::Vector3d& z_prev,
                              const Eigen::Vector3d& x_end,
                              const Eigen::Vector3d& origin_prev,
                              const Eigen::Vector3d& p_end);

Eigen::Vector3d pick_base_x(const Eigen::Vector3d& z);

}  // namespace urdf_to_dh_internals
}  // namespace sdk
}  // namespace viam
