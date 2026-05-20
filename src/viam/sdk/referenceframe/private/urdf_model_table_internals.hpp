/// @file referenceframe/private/urdf_model_table_internals.hpp
/// @brief Internal helpers for urdf_to_model_table. Exposed only to
///        the testsuite; not a stable public API.
#pragma once

#include <string>
#include <vector>

#include <Eigen/Core>
#include <boost/optional.hpp>

#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/referenceframe/urdf_model_table.hpp>

namespace viam {
namespace sdk {
namespace urdf_model_table_internals {

/// @brief Per-joint parse result, pre-validation.
struct ParsedJoint {
    std::string name;
    std::string type_str;  ///< raw URDF type attribute
    std::string parent_link;
    std::string child_link;
    Eigen::Vector3d xyz{0, 0, 0};
    Eigen::Vector3d rpy{0, 0, 0};
    boost::optional<Eigen::Vector3d> axis_opt;
};

/// @brief Parse all <joint> elements from a URDF, no topology checks.
std::vector<ParsedJoint> parse_urdf(const KinematicsDataURDF& urdf);

/// @brief Order joints root -> leaf along the kinematic chain.
/// @throws viam::sdk::Exception on multi-root, branching, or disconnect.
std::vector<ParsedJoint> walk_urdf_chain(const std::vector<ParsedJoint>& joints);

/// @brief Convert one validated ParsedJoint to a JointRow.
/// @throws viam::sdk::Exception on unsupported type or zero non-fixed axis.
JointRow to_row(const ParsedJoint& parsed);

}  // namespace urdf_model_table_internals
}  // namespace sdk
}  // namespace viam
