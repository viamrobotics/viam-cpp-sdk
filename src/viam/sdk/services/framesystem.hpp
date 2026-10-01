/// @file services/framesystem.hpp
///
/// @brief Defines the `FrameSystem` service.
#pragma once

#include <string>
#include <vector>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/world_state.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {

namespace robot {
namespace v1 {

class FrameSystemConfig;

}  // namespace v1
}  // namespace robot

namespace sdk {

/// @defgroup FrameSystem Classes related to the FrameSystem service.

/// @class FrameSystem framesystem.hpp "services/framesystem.hpp"
/// @brief The machine's frame system: the tree of reference frames viam-server builds from the
/// frame config of every resource. Use it to find where a component is and to move poses and point
/// clouds between frames.
/// @ingroup FrameSystem
///
/// viam-server owns one frame system per machine and publishes it under the reserved name
/// `$framesystem`. A modular resource finds it in its `Dependencies` under that name, and
/// `RobotClient::resource_by_name` hands it out under that name too. It is never listed in
/// `RobotClient::resource_names`.
class FrameSystem : public Service {
   public:
    /// @struct frame_system_config
    /// @brief One frame of the frame system, with the kinematics of the resource it belongs to.
    struct frame_system_config {
        WorldState::transform frame;
        ProtoStruct kinematics;
        friend bool operator==(const frame_system_config& lhs, const frame_system_config& rhs);
    };

    /// @brief The reserved name viam-server publishes the machine's frame system under.
    static const std::string kPublicName;

    /// @brief The full resource name of the machine's frame system. Its short name is
    /// @ref kPublicName.
    static Name public_name();

    /// @brief Get the configuration of every frame in the frame system.
    inline std::vector<frame_system_config> get_frame_system_config() {
        return get_frame_system_config({});
    }

    /// @brief Get the configuration of every frame in the frame system.
    /// @param additional_transforms Extra frames to add to the frame system for this call only.
    virtual std::vector<frame_system_config> get_frame_system_config(
        const std::vector<WorldState::transform>& additional_transforms) = 0;

    /// @brief Get the pose of a component in the world reference frame.
    /// @param component_name The component whose pose is being requested.
    inline pose_in_frame get_pose(const std::string& component_name) {
        return get_pose(component_name, "world", {}, {});
    }

    /// @brief Get the pose of a component in the desired reference frame.
    /// @param component_name The component whose pose is being requested.
    /// @param destination_frame The reference frame in which to express the pose.
    /// @param supplemental_transforms Extra frames needed to compute the pose.
    inline pose_in_frame get_pose(
        const std::string& component_name,
        const std::string& destination_frame,
        const std::vector<WorldState::transform>& supplemental_transforms) {
        return get_pose(component_name, destination_frame, supplemental_transforms, {});
    }

    /// @brief Get the pose of a component in the desired reference frame.
    /// @param component_name The component whose pose is being requested.
    /// @param destination_frame The reference frame in which to express the pose.
    /// @param supplemental_transforms Extra frames needed to compute the pose.
    /// @param extra Any additional arguments to the method.
    virtual pose_in_frame get_pose(
        const std::string& component_name,
        const std::string& destination_frame,
        const std::vector<WorldState::transform>& supplemental_transforms,
        const ProtoStruct& extra) = 0;

    /// @brief Express a pose in a different reference frame.
    /// @param query The pose to transform, with the frame it is currently expressed in.
    /// @param destination The reference frame to express the pose in.
    inline pose_in_frame transform_pose(const pose_in_frame& query,
                                        const std::string& destination) {
        return transform_pose(query, destination, {});
    }

    /// @brief Express a pose in a different reference frame.
    /// @param query The pose to transform, with the frame it is currently expressed in.
    /// @param destination The reference frame to express the pose in.
    /// @param additional_transforms Extra frames needed to compute the transform.
    virtual pose_in_frame transform_pose(
        const pose_in_frame& query,
        const std::string& destination,
        const std::vector<WorldState::transform>& additional_transforms) = 0;

    /// @brief Express a point cloud in a different reference frame.
    /// @param pcd The point cloud, serialized in PCD format.
    /// @param source The reference frame the point cloud is currently expressed in.
    /// @param destination The reference frame to express the point cloud in.
    /// @return The transformed point cloud, serialized in PCD format.
    ///
    /// The SDK has no point cloud type, so this works on raw PCD bytes. The transform happens on
    /// the server, unlike the Go SDK which shifts the points locally.
    virtual std::vector<unsigned char> transform_pcd(const std::vector<unsigned char>& pcd,
                                                     const std::string& source,
                                                     const std::string& destination) = 0;

    API api() const override;

   protected:
    explicit FrameSystem(std::string name);
};

template <>
struct API::traits<FrameSystem> {
    static API api();
};

namespace proto_convert_details {

template <>
struct from_proto_impl<robot::v1::FrameSystemConfig> {
    FrameSystem::frame_system_config operator()(const robot::v1::FrameSystemConfig*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
