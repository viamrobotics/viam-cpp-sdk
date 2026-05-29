/// @file referenceframe/kinematics_model_table.hpp
/// @brief Convert a serial URDF chain to a typed per-joint model table
///        and from the table to a double (n, 10) tensor.
#pragma once

#include <string>
#include <vector>

#if defined(__has_include) && (__has_include(<xtensor/containers/xarray.hpp>))
#include <xtensor/containers/xarray.hpp>
#else
#include <xtensor/xarray.hpp>
#endif

#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/common/linear_algebra.hpp>

namespace viam {
namespace sdk {

/// @brief URDF joint type, restricted to arm-relevant joints.
/// @note Underlying values are stable and form the wire encoding for
/// column 9 of the tensor produced by `model_table_to_tensor`.
enum class JointType {
    k_revolute = 0,
    k_continuous = 1,
    k_prismatic = 2,
    k_fixed = 3,
};

/// @brief One row of the model table: the per-joint URDF fields.
/// @note `xyz`/`rpy`/`axis` are taken directly from the URDF.
struct JointRow {
    std::string name;
    Vector3 xyz{};
    Vector3 rpy{};
    Vector3 axis{};
    JointType type = JointType::k_fixed;
};

using ModelTable = std::vector<JointRow>;

/// @brief Parse a kinematics description and return the per-joint model
/// table in chain order. Currently only URDF input is supported; SVA
/// support is planned.
/// @throws viam::sdk::Exception on parse error, branching / multi-root /
///         disconnected topology, unsupported joint types, or zero axis.
ModelTable kinematics_to_model_table(const KinematicsDataURDF& urdf);

/// @brief Convert a model table to a double tensor of shape (n, 10).
/// Columns: 0..2 xyz, 3..5 rpy, 6..8 axis, 9 joint type as the underlying
/// value of `JointType` (see enum declaration for the encoding).
/// @throws viam::sdk::Exception on empty input.
xt::xarray<double> model_table_to_tensor(const ModelTable& table);

/// @brief Inverse of model_table_to_tensor: rehydrate an (n, 10) double
/// tensor into a model table. Reconstructed rows have empty `name`
/// (names are not carried in the tensor).
/// @throws viam::sdk::Exception on non-2D input, wrong column count,
/// empty input, or invalid joint-type encoding (col 9 must be an integer
/// matching one of the `JointType` values).
ModelTable tensor_to_model_table(const xt::xarray<double>& tensor);

}  // namespace sdk
}  // namespace viam
