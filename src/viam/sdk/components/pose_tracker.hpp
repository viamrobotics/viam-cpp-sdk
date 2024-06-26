/// @file components/pose_tracker.hpp
///
/// @brief Defines a `PoseTracker` component
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/posetracker/v1/pose_tracker.pb.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

/// @defgroup PoseTracker Classes related to the PoseTracker component.

/// @class PoseTracker pose_tracker.hpp "components/pose_tracker.hpp"
/// @brief A `PoseTracker` represents a physical pose or motion tracking device.
///
/// This acts as an abstract base class for any drivers representing specific
/// pose tracker implementations
class PoseTracker : public Component {
   public:
    API api() const override;

    /// @brief Get the poses of each body tracked by the pose tracker.
    /// @param tracker_name The name of the pose tracker.
    /// @param body_names Names of bodies whose poses are being requested. If the vector is empty
    /// then all available poses are returned.
    /// @return A mapping of each body to its pose.
    inline std::unordered_map<std::string, pose_in_frame> get_poses(
        const std::vector<std::string>& body_names) {
        return get_poses(body_names, {});
    }

    /// @brief Get the poses of each body tracked by the pose tracker.
    /// @param tracker_name The name of the pose tracker.
    /// @param body_names Names of bodies whose poses are being requested. If the vector is empty
    /// then all available poses are returned.
    /// @param extra Any additional arguments to the method.
    /// @return A mapping of each body to its pose.
    virtual std::unordered_map<std::string, pose_in_frame> get_poses(
        const std::vector<std::string>& body_names, const AttributeMap& extra) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling pose tracker
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling pose tracker
    /// @param extra Any additional arguments to the method
    virtual std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) = 0;

   protected:
    using Component::Component;
};

template <>
struct API::traits<PoseTracker> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
