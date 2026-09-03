/// @file components/arm.hpp
///
/// @brief Defines an `Arm` component
#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <boost/optional/optional.hpp>
#include <boost/variant/variant.hpp>

#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/common/mesh.hpp>
#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/resource/stoppable.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace component {
namespace arm {
namespace v1 {

class TrajectoryPoint;
class MoveThroughJointPositionsStreamedResponse;
class GetPropertiesResponse;

}  // namespace v1
}  // namespace arm
}  // namespace component
}  // namespace viam

namespace viam {
namespace sdk {

/// @defgroup Arm Classes related to the Arm component.

/// @class Arm arm.hpp "components/arm.hpp"
/// @brief An `Arm` represents a physical robot arm that exists in three-dimensional space.
/// @ingroup Arm
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific arm implementations. This class cannot be used on its own.
class Arm : public Component, public Stoppable {
   public:
    /// @brief Kinematics data types (aliases for shared kinematics types)
    using KinematicsDataUnspecified
        [[deprecated("Use ::viam::sdk::KinematicsDataUnspecified instead")]] =
            ::viam::sdk::KinematicsDataUnspecified;
    using KinematicsDataSVA [[deprecated("Use ::viam::sdk::KinematicsDataSVA instead")]] =
        ::viam::sdk::KinematicsDataSVA;
    using KinematicsDataURDF [[deprecated("Use ::viam::sdk::KinematicsDataURDF instead")]] =
        ::viam::sdk::KinematicsDataURDF;
    using KinematicsData [[deprecated("Use ::viam::sdk::KinematicsData instead")]] =
        ::viam::sdk::KinematicsData;

    /// @brief A movement limit: either a single scalar applied uniformly, or per-joint values.
    using MoveLimit = boost::variant<double, std::vector<double>>;

    /// @brief Movement specifications for move_through_joint_positions.
    struct MoveOptions {
        boost::optional<MoveLimit> max_vel_degs_per_sec;
        boost::optional<MoveLimit> max_acc_degs_per_sec2;
        boost::optional<double> max_tcp_speed;
    };

    /// @struct properties
    /// @brief Optional features that the specific arm supports
    struct properties {
        /// True if the arm supports software-enabled manual mode
        bool support_manual_mode = false;
        /// True if the arm supports direct cartesian commands (move_to_position is implemented)
        bool support_cartesian_commands = false;
    };

    /// @brief Get the current position of the end of the arm.
    /// @return The `pose` representing the end position of the arm.
    inline pose get_end_position() {
        return get_end_position({});
    }

    /// @brief Get the current position of the end of the arm.
    /// @param extra Any additional arguments to the method.
    /// @return The `pose` representing the end position of the arm.
    virtual pose get_end_position(const ProtoStruct& extra) = 0;

    /// @brief Move the end of the arm to @param pose.
    inline void move_to_position(const pose& pose) {
        move_to_position(pose, {});
    }

    /// @brief Move the end of the arm to @param pose.
    /// @param pose The destination pose for the arm.
    /// @param extra Any additional arguments to the method.
    virtual void move_to_position(const pose& pose, const ProtoStruct& extra) = 0;

    /// @brief Lists the joint positions in degrees of every joint on a robot arm
    inline std::vector<double> get_joint_positions() {
        return get_joint_positions({});
    }

    /// @brief Lists the joint positions in degrees of every joint on a robot arm
    /// @param extra Any additional arguments to the method.
    virtual std::vector<double> get_joint_positions(const ProtoStruct& extra) = 0;

    /// @brief Move each joint on the arm to the corresponding angle specified in @param positions
    inline void move_to_joint_positions(const std::vector<double>& positions) {
        return move_to_joint_positions(positions, {});
    }

    /// @brief Move each joint on the arm to the corresponding angle specified in @param positions
    /// @param extra Any additional arguments to the method.
    virtual void move_to_joint_positions(const std::vector<double>& positions,
                                         const ProtoStruct& extra) = 0;

    /// @brief Move each joint on the arm through the positions specified in @param positions
    /// @param options optional specifications to be obeyed during the motion.
    /// TODO consider replacing vector vector with xtensor array, and also if it may be
    /// possible to specify or constrain dimensionality of the array in advance.
    inline void move_through_joint_positions(const std::vector<std::vector<double>>& positions,
                                             const MoveOptions& options) {
        return move_through_joint_positions(positions, options, {});
    }

    /// @brief Move each joint on the arm through the positions specified in @param positions
    /// @param options optional specifications to be obeyed during the motion.
    /// @param extra Any additional arguments to the method.
    virtual void move_through_joint_positions(const std::vector<std::vector<double>>& positions,
                                              const MoveOptions& options,
                                              const ProtoStruct& extra) = 0;

    /// @brief A single waypoint in a streamed joint-space trajectory.
    ///
    /// The first point of a stream must have @ref time equal to zero; subsequent
    /// points must be strictly increasing in time across the entire stream.
    struct trajectory_point {
        /// @brief Target kinematic constraints at a waypoint.
        ///
        /// `velocities` is required whenever this struct is present;
        /// `accelerations` is independently optional. Both are one value per
        /// joint and, like everything else on this interface, denominated in
        /// degrees: `velocities` in degrees per second and `accelerations` in
        /// degrees per second squared.
        struct kinematic_constraints {
            std::vector<double> velocities_degs_per_sec;
            boost::optional<std::vector<double>> accelerations_degs_per_sec2;
        };

        /// @brief Offset of this waypoint from the start of the trajectory.
        ///
        /// Must be zero at the first point of a stream and strictly increasing
        /// thereafter. Microsecond resolution matches the SDK's
        /// `google.protobuf.Duration` conversion; sub-microsecond distinctions
        /// are not preserved on the wire.
        std::chrono::microseconds time;

        /// @brief Target position at this waypoint, one value per joint, in degrees.
        std::vector<double> positions;

        /// @brief Optional velocity and acceleration constraints at this waypoint.
        boost::optional<kinematic_constraints> constraints;
    };

    /// @brief An update emitted while a streamed trajectory executes.
    ///
    /// An arm may emit them at any cadence.
    struct trajectory_update {};

    /// @brief How a streamed trajectory execution ended.
    enum class stream_outcome : std::uint8_t {
        k_completed = 0,  ///< The trajectory ran to its natural end.
        k_halted_by_update_handler =
            1,  ///< `update_handler` returned false, stopping the stream early.
    };

    /// @brief Execute a stream of trajectory points in order.
    /// @param batch_source Pull-source for the next batch of waypoints.
    /// @param update_handler Handler invoked for each update the implementation emits.
    /// @return How the stream ended.
    inline stream_outcome move_through_joint_positions_streamed(
        const std::function<boost::optional<std::vector<trajectory_point>>()>& batch_source,
        const std::function<bool(trajectory_update)>& update_handler) {
        return move_through_joint_positions_streamed(batch_source, update_handler, {});
    }

    /// @brief Execute a stream of trajectory points in order.
    ///
    /// This is the streamed analogue of `move_through_joint_positions`. It is
    /// implemented on both sides of the RPC: an arm driver implements it to
    /// execute a trajectory, and the SDK arm client implements it to forward the
    /// call to a remote arm. Both sides must follow the contract below.
    ///
    /// An implementation pulls the trajectory from `batch_source` and reports
    /// progress through `update_handler` until the stream is exhausted, it is
    /// asked to stop, or it faults. A fault is reported by throwing: a
    /// server-side implementation has its exception converted to a terminal gRPC
    /// status delivered to the remote caller, and a client-side implementation
    /// rethrows to its caller. An exception thrown out of `update_handler` is
    /// propagated the same way rather than swallowed, so neither callback is
    /// required to be `noexcept`.
    ///
    /// Threading: because the stream is bidirectional, an implementation is
    /// permitted to invoke the two callbacks from different threads and to run
    /// them concurrently with each other. A caller must therefore not assume the
    /// callbacks are serialized against one another or pinned to a single
    /// thread, and must synchronize any mutable state it shares between them. A
    /// given callback is never invoked concurrently with itself.
    ///
    /// @param batch_source Pull-source for the trajectory. Call it to obtain the
    /// next batch of waypoints; it yields `boost::none` once no more points are
    /// coming, after which the implementation should finish executing what it
    /// already has and return. Points arrive grouped into the batches the sender
    /// chose; an implementation that wants per-point handling iterates within
    /// each batch. Note that for servers, a disengaged return is ambiguous
    /// between end-of-stream and cancellation. Arm implementors are expected
    /// to explicitly check for cancellation.
    /// @param update_handler Sink for execution updates. Call it with each
    /// `trajectory_update` to report progress; it returns `true` to keep going
    /// and `false` to ask that the stream stop early, in which case the
    /// implementation should stop executing and return
    /// `stream_outcome::k_halted_by_update_handler`.
    /// @param extra Any additional arguments to the method.
    /// @return `stream_outcome::k_completed` if the trajectory ran to its natural
    /// end, or `stream_outcome::k_halted_by_update_handler` if `update_handler`
    /// asked it to stop.
    virtual stream_outcome move_through_joint_positions_streamed(
        const std::function<boost::optional<std::vector<trajectory_point>>()>& batch_source,
        const std::function<bool(trajectory_update)>& update_handler,
        const ProtoStruct& extra) = 0;

    /// @brief Reports if the arm is in motion.
    virtual bool is_moving() = 0;

    /// @brief Get the status of the arm.
    /// @return A `ProtoStruct` containing the status of the arm.
    virtual ProtoStruct get_status() = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    /// @brief Get the kinematics data associated with the arm.
    /// @param extra Any additional arguments to the method.
    /// @return A variant of kinematics data; URDF variants may include meshes by filepath.
    virtual ::viam::sdk::KinematicsData get_kinematics(const ProtoStruct& extra) = 0;

    /// @brief Get the kinematics data associated with the arm.
    /// @return A variant of kinematics data, with the type indicating the format of the data.
    inline ::viam::sdk::KinematicsData get_kinematics() {
        return get_kinematics({});
    }

    /// @brief Returns `3DModel`s associated with the calling arm
    /// @param extra Any additional arguments to the method
    /// @return A map of `3DModel`s associated with the calling arm
    virtual std::map<std::string, mesh> get_3d_models(const ProtoStruct& extra) = 0;

    /// @brief Returns `3DModel`s associated with the calling arm
    /// @return A map of `3DModel`s associated with the calling arm
    inline std::map<std::string, mesh> get_3d_models() {
        return get_3d_models({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling arm
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling arm
    /// @param extra Any additional arguments to the method
    virtual std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) = 0;

    /// @brief Returns the properties of the arm which comprises the booleans indicating
    /// which optional features the arm supports.
    inline properties get_properties() {
        return get_properties({});
    }

    /// @brief Returns the properties of the arm which comprises the booleans indicating
    /// which optional features the arm supports.
    /// @param extra Any additional arguments to the method.
    virtual properties get_properties(const ProtoStruct& extra) = 0;

    /// @brief Enter or exit manual mode, with no time limit.
    /// @param manual_mode True to enter manual mode, false to exit it.
    /// @throws `Exception` if manual mode is not supported
    inline void set_manual_mode(bool manual_mode) {
        return set_manual_mode(manual_mode, std::chrono::seconds::zero(), {});
    }

    /// @brief Enter or exit manual mode.
    /// @param manual_mode True to enter manual mode, false to exit it.
    /// @param enabled_for How long to stay in manual mode; zero means no time limit.
    /// @throws `Exception` if manual mode is not supported
    inline void set_manual_mode(bool manual_mode, std::chrono::seconds enabled_for) {
        return set_manual_mode(manual_mode, enabled_for, {});
    }

    /// @brief Enter or exit manual mode.
    /// @param manual_mode True to enter manual mode, false to exit it.
    /// @param enabled_for How long to stay in manual mode; zero means no time limit.
    /// @param extra Any additional arguments to the method.
    /// @throws `Exception` if manual mode is not supported
    virtual void set_manual_mode(bool manual_mode,
                                 std::chrono::seconds enabled_for,
                                 const ProtoStruct& extra) = 0;

    /// @brief Reports whether the arm is currently in manual mode.
    /// @throws `Exception` if manual mode is not supported
    inline bool get_manual_mode() {
        return get_manual_mode({});
    }

    /// @brief Reports whether the arm is currently in manual mode.
    /// @param extra Any additional arguments to the method.
    /// @throws `Exception` if manual mode is not supported
    virtual bool get_manual_mode(const ProtoStruct& extra) = 0;

    API api() const override;

   protected:
    explicit Arm(std::string name);
};

template <>
struct API::traits<Arm> {
    static API api();
};

bool operator==(const Arm::properties& lhs, const Arm::properties& rhs);

namespace proto_convert_details {

template <>
struct to_proto_impl<Arm::trajectory_point> {
    void operator()(const Arm::trajectory_point&, viam::component::arm::v1::TrajectoryPoint*) const;
};

template <>
struct from_proto_impl<viam::component::arm::v1::TrajectoryPoint> {
    Arm::trajectory_point operator()(const viam::component::arm::v1::TrajectoryPoint*) const;
};

template <>
struct to_proto_impl<Arm::trajectory_update> {
    void operator()(const Arm::trajectory_update&,
                    viam::component::arm::v1::MoveThroughJointPositionsStreamedResponse*) const;
};

template <>
struct from_proto_impl<viam::component::arm::v1::MoveThroughJointPositionsStreamedResponse> {
    Arm::trajectory_update operator()(
        const viam::component::arm::v1::MoveThroughJointPositionsStreamedResponse*) const;
};

template <>
struct to_proto_impl<Arm::properties> {
    void operator()(const Arm::properties&, viam::component::arm::v1::GetPropertiesResponse*) const;
};

template <>
struct from_proto_impl<viam::component::arm::v1::GetPropertiesResponse> {
    Arm::properties operator()(const viam::component::arm::v1::GetPropertiesResponse*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
