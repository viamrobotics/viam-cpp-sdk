/// @file referenceframe/urdf_model_table.hpp
/// @brief Convert a serial URDF chain to a typed per-joint model table
///        and from the table to a double (n, 10) tensor.
#pragma once

#include <string>
#include <vector>

#include <Eigen/Core>
#if defined(__has_include) && (__has_include(<xtensor/containers/xarray.hpp>))
#include <xtensor/containers/xarray.hpp>
#else
#include <xtensor/xarray.hpp>
#endif

#include <viam/sdk/common/kinematics.hpp>

namespace viam {
namespace sdk {

/// @brief URDF joint type, restricted to arm-relevant joints.
enum class JointType { revolute, continuous, prismatic, fixed };

/// @brief One row of the model table: the per-joint URDF fields.
/// @note `xyz`/`rpy`/`axis` are taken directly from the URDF.
struct JointRow {
    std::string name;  
    Eigen::Vector3d xyz{0, 0, 0};  
    Eigen::Vector3d rpy{0, 0, 0}; 
    Eigen::Vector3d axis{0, 0, 0};
    JointType type = JointType::fixed;
};

using ModelTable = std::vector<JointRow>;

/// @brief Parse a URDF and return the per-joint model table in chain order.
/// @throws viam::sdk::Exception on parse error, branching / multi-root /
///         disconnected topology, unsupported joint types, or zero axis.
ModelTable urdf_to_model_table(const KinematicsDataURDF& urdf);

/// @brief Convert a model table to a double tensor of shape (n, 10).
/// Columns: 0..2 xyz, 3..5 rpy, 6..8 axis, 9 joint type as
/// underlying enum value (0=revolute, 1=continuous, 2=prismatic, 3=fixed).
/// @throws viam::sdk::Exception on empty input.
xt::xarray<double> model_table_to_tensor(const ModelTable& table);

/// @brief Inverse of model_table_to_tensor: rehydrate an (n, 10) double
/// tensor into a model table. Reconstructed rows have empty `name`
/// (names are not carried in the tensor).
/// @throws viam::sdk::Exception on non-2D input, wrong column count,
/// empty input, or invalid joint-type encoding (col 9 must be an
/// integer in [0, 3]).
ModelTable tensor_to_model_table(const xt::xarray<double>& tensor);

}  // namespace sdk
}  // namespace viam
