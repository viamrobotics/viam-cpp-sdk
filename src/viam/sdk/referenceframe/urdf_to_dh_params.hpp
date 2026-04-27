/// @file referenceframe/urdf_to_dh_params.hpp
/// @brief Convert a revolute-only URDF serial chain to Denavit-Hartenberg
///        parameters.
#pragma once

#include <string>
#include <vector>

#include <viam/sdk/common/kinematics.hpp>

namespace viam {
namespace sdk {

class Arm;

/// @brief One Denavit-Hartenberg row for a single revolute joint.
///
/// Units: meters for `d` and `a`, radians for `theta` and `alpha`.
/// `theta` is the joint's angle at the URDF's zero configuration
/// (typically 0). The runtime joint angle is `theta + input`.
struct DHParam {
    std::string name;  ///< joint name copied from the URDF
    double d;          ///< translation along Z_{i-1}
    double theta;      ///< rotation about Z_{i-1} at zero config
    double a;          ///< translation along X_i
    double alpha;      ///< rotation about X_i
};

/// @brief Convert a revolute-only URDF serial chain to DH parameters.
///
/// Returns one `DHParam` per revolute or continuous joint in chain order.
/// Fixed joints are folded into the cumulative transform.
///
/// @throws viam::sdk::Exception if the URDF is not a serial revolute chain,
///         has an unsupported joint type, has a zero-magnitude axis, has an
///         end-effector frame that is not DH-compatible (X-axis not
///         perpendicular to the last joint axis, or origin out of the DH
///         plane), or fails to parse as XML.
std::vector<DHParam> urdf_to_dh_params(const KinematicsDataURDF& urdf);

/// @brief Convenience overload: fetches `arm.get_kinematics()`, requires
///        URDF format, and forwards to the byte-based overload.
///
/// @throws viam::sdk::Exception if the arm's kinematics are not in URDF
///         format, or for any reason the byte-based overload would throw.
std::vector<DHParam> urdf_to_dh_params(Arm& arm);

}  // namespace sdk
}  // namespace viam
