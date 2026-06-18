/// @file referenceframe/kinematics_model_table.hpp
/// @brief A per-joint model table for a serial kinematic chain, with
///        conversions to/from URDF and a double (n, 10) tensor.
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

/// @brief A per-joint model table for a serial kinematic chain, held in chain
/// order. Build one with the `from` factories and read it back via `rows()` or
/// `to_tensor()`.
class ModelTable {
   public:
    /// @brief URDF joint type, restricted to arm-relevant joints.
    /// @note Underlying values are stable and form the wire encoding for
    /// column 9 of the tensor produced by `to_tensor()`.
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

    /// @brief Construct directly from rows already in chain order.
    explicit ModelTable(std::vector<JointRow> rows);

    /// @brief Parse a kinematics description into a model table in chain order.
    /// @throws viam::sdk::Exception on parse error, branching / multi-root /
    ///         disconnected topology, unsupported joint types, or zero axis.
    static ModelTable from(const KinematicsDataURDF& urdf);

    /// @brief Inverse of `to_tensor`: rehydrate an (n, 10) double tensor into a
    /// model table. Reconstructed rows have empty `name` (names are not carried
    /// in the tensor).
    /// @throws viam::sdk::Exception on non-2D input, wrong column count, empty
    /// input, or invalid joint-type encoding (col 9 must be an integer matching
    /// one of the `JointType` values).
    static ModelTable from(const xt::xarray<double>& tensor);

    /// @brief Convert to a double tensor of shape (n, 10).
    /// Columns: 0..2 xyz, 3..5 rpy, 6..8 axis, 9 joint type as the underlying
    /// value of `JointType` (see enum declaration for the encoding).
    /// @throws viam::sdk::Exception on empty table.
    xt::xarray<double> to_tensor() const;

    /// @brief The rows of the table, in chain order.
    const std::vector<JointRow>& rows() const;

   private:
    std::vector<JointRow> rows_;
};

}  // namespace sdk
}  // namespace viam
