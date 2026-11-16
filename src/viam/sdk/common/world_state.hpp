/// @file common/world_state.hpp
///
/// @brief Defines `WorldState`.
#pragma once

#include <optional>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace common {
namespace v1 {

class GeometriesInFrame;
class Transform;
class WorldState;

}  // namespace v1
}  // namespace common
}  // namespace viam

namespace viam {
namespace sdk {

/// @class WorldState world_state.hpp "common/world_state.hpp"
/// @brief Describes the physical environment around a robot.
///
/// Contains information about obstacles in the robot's environment and any additional
/// transforms needed to describe the positions of objects relative to the robot's frame.
class WorldState {
   public:
    /// @brief Holds a set of geometries observed from a particular reference frame.
    struct geometries_in_frame {
        std::vector<GeometryConfig> geometries;

        /// The reference frame in which the geometries were observed.
        std::string reference_frame;

        friend bool operator==(const geometries_in_frame&, const geometries_in_frame&) = default;
    };

    /// @brief Represents a pose and two reference frames.
    ///
    /// Describes the pose of a physical object (or coordinate frame) in the observer's frame,
    /// allowing the motion planner to account for additional frames beyond those defined by
    /// the robot's kinematic chain.
    struct transform {
        /// The name of this reference frame.
        std::string reference_frame;
        /// The pose of this frame relative to the observer's frame.
        pose_in_frame pose_in_observer_frame;
        /// An optional geometry representing the physical object at this transform's location.
        std::optional<GeometryConfig> physical_object;

        friend bool operator==(const transform&, const transform&) = default;
    };

    WorldState() = default;

    /// @brief Constructs a `WorldState` with the given obstacles and transforms.
    /// @param obstacles Geometries that the motion planner should treat as obstacles.
    /// @param transforms Additional coordinate frame transforms for the planner.
    WorldState(std::vector<geometries_in_frame> obstacles, std::vector<transform> transforms);

    /// @brief A list of obstacles expressed as a geometry and the reference frame in which it was
    /// observed.
    const std::vector<geometries_in_frame>& obstacles() const;

    /// @brief A list of Transforms, optionally with geometries. Used as supplemental transforms to
    /// transform a pose from one reference frame to
    // another, or to attach moving geometries to the frame system.
    const std::vector<transform>& transforms() const;

    friend bool operator==(const WorldState&, const WorldState&) = default;

   private:
    std::vector<geometries_in_frame> obstacles_;
    std::vector<transform> transforms_;
};

namespace proto_convert_details {

template <>
struct to_proto_impl<WorldState::geometries_in_frame> {
    void operator()(const WorldState::geometries_in_frame&, common::v1::GeometriesInFrame*) const;
};

template <>
struct from_proto_impl<common::v1::GeometriesInFrame> {
    WorldState::geometries_in_frame operator()(const common::v1::GeometriesInFrame*) const;
};

template <>
struct to_proto_impl<WorldState::transform> {
    void operator()(const WorldState::transform&, common::v1::Transform*) const;
};

template <>
struct from_proto_impl<common::v1::Transform> {
    WorldState::transform operator()(const common::v1::Transform*) const;
};

template <>
struct to_proto_impl<WorldState> {
    void operator()(const WorldState&, common::v1::WorldState*) const;
};

template <>
struct from_proto_impl<common::v1::WorldState> {
    WorldState operator()(const common::v1::WorldState*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
