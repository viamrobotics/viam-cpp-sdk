#pragma once

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/referenceframe/kinematics_model_table.hpp>

namespace viam {
namespace sdk {
namespace impl {

// @brief Per-joint parse result, pre-validation.
struct ParsedJoint {
    std::string name;   
    std::string type_str;  ///< raw URDF type attribute
    std::string parent_link;
    std::string child_link;
    Vector3 xyz{};
    Vector3 rpy{};
    boost::optional<Vector3> axis_opt;
};

// @brief Parse all <joint> elements from a URDF, no topology checks.
std::vector<ParsedJoint> parse_urdf(const KinematicsDataURDF& urdf);

// @brief Order joints root -> leaf along the kinematic chain.
// @throws viam::sdk::Exception on multi-root, branching, or disconnect.
std::vector<ParsedJoint> walk_urdf_chain(const std::vector<ParsedJoint>& joints);

// @brief Convert one validated ParsedJoint to a ModelTable::JointRow.
// @throws viam::sdk::Exception on unsupported type or zero non-fixed axis.
ModelTable::JointRow to_row(const ParsedJoint& parsed);

}  // namespace impl
}  // namespace sdk
}  // namespace viam
