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
        /// `accelerations` is independently optional. The
        /// accelerations-require-velocities invariant is enforced by the type
        /// system, not just documentation.
        struct kinematic_constraints {
            std::vector<double> velocities;
            boost::optional<std::vector<double>> accelerations;
        };

        /// @brief Offset of this waypoint from the start of the trajectory.
        ///
        /// Must be zero at the first point of a stream and strictly increasing
        /// thereafter. Microsecond resolution matches the SDK's
        /// `google.protobuf.Duration` conversion; sub-microsecond distinctions
        /// are not preserved on the wire.
        std::chrono::microseconds time;
        std::vector<double> positions;
        boost::optional<kinematic_constraints> constraints;
    };

    /// @brief An update emitted by the implementation as a streamed trajectory
    /// executes.
    ///
    /// Updates flow back independently of the input batches: the implementation
    /// may emit them at any cadence, or not at all, and a future implementation
    /// may deliver them as a truly asynchronous channel. Currently empty; it
    /// will grow fields carrying per-execution status.
    struct trajectory_update {};

    /// @brief How a streamed trajectory execution ended.
    enum class stream_outcome : std::uint8_t {
        k_completed,                 ///< The trajectory ran to its natural end.
        k_halted_by_update_handler,  ///< update_handler returned false, stopping the stream early.
    };

    /// @brief Execute a stream of trajectory points in order.
    /// @param batch_source Pull-source for the next batch of waypoints.
    /// @param update_handler Handler invoked for each update the implementation emits.
    /// @return How the stream ended.
    inline stream_outcome move_through_joint_positions_streamed(
        std::function<boost::optional<std::vector<trajectory_point>>()> batch_source,
        std::function<bool(trajectory_update)> update_handler) {
        return move_through_joint_positions_streamed(
            std::move(batch_source), std::move(update_handler), {});
    }

    /// @brief Execute a stream of trajectory points in order.
    ///
    /// `batch_source` returns the next batch of points or `boost::none` on
    /// client EOF or cancellation. Empty batches and protocol-noise messages
    /// are filtered out by the dispatcher and never surface here.
    ///
    /// `update_handler` consumes each `trajectory_update` the implementation
    /// emits. It returns `true` to continue or `false` to stop the stream
    /// early. The `false` return is bivalent by side: on the CLIENT it is the
    /// caller electing to halt (perhaps reacting to what it sees coming back),
    /// which ends the call with `stream_outcome::k_halted_by_update_handler`;
    /// on the SERVER it is the framework telling the implementation to stop
    /// (cancellation, wire failure). If `update_handler` throws, the exception
    /// is not swallowed -- it propagates out of the call on the client, and
    /// converts to a terminal gRPC status on the server. It need not be
    /// `noexcept`.
    ///
    /// Returns `stream_outcome::k_completed` when the trajectory ran to its
    /// natural end, or `k_halted_by_update_handler` when `update_handler`
    /// stopped it. Throwing from `batch_source` or from the implementation
    /// converts to a terminal gRPC status surfaced to the client.
    ///
    /// THREADING:
    /// - On the SERVER (i.e., when invoked through ArmServer dispatch),
    ///   `batch_source` and `update_handler` are both invoked on the gRPC
    ///   handler thread, and the implementation runs on that thread too. No
    ///   additional threads. The two callbacks are mutually serialized on
    ///   that thread: while the implementation is blocked in
    ///   `batch_source`'s Read it cannot emit a `trajectory_update`, and vice
    ///   versa. Update-emit latency on the wire is therefore bounded by the
    ///   client's send cadence.
    ///   TODO: this mutual serialization must be lifted before
    ///   `trajectory_update` grows non-empty fields and updates need to flow at
    ///   a cadence independent of the inbound batches. See ERROR SURFACING
    ///   below for the narrow consequence today and why the obvious fix is not
    ///   trivial.
    /// - On the CLIENT (i.e., when invoked through ArmClient on a remote
    ///   arm), `batch_source` runs on the caller's thread and
    ///   `update_handler` runs on a separate SDK-managed reader thread. The
    ///   two callbacks may execute concurrently. Each is called serially
    ///   with respect to itself (no concurrent calls to the same callback).
    ///   Implementations sharing mutable state between the two callbacks
    ///   must provide their own synchronization.
    ///
    /// ERROR SURFACING: the single-threaded server dispatch narrows, but does
    /// not defeat, the point of a bidi stream. A terminal arm-side fault reaches
    /// the client at one of two moments. While the client is still sending, the
    /// fault surfaces on the next `batch_source` Read -- bounded by the client's
    /// inter-batch cadence, which is small precisely for the tight-cadence
    /// streams batching exists to serve. Once the client half-closes its send
    /// side (the common shape for a pre-kinematized trajectory, whose sends
    /// finish in milliseconds while execution runs for seconds), the handler is
    /// no longer parked in Read and a fault surfaces promptly. Robust surfacing
    /// therefore assumes a well-behaved client that closes its send side when
    /// done; a client that stops sending without half-closing and then idles can
    /// leave a fault unreported until it does. Non-terminal per-update status is
    /// the case that genuinely needs concurrent emit, and it stays gated behind
    /// `trajectory_update` growing fields.
    ///
    /// The obvious fix -- a background reader thread feeding a queue -- is not
    /// trivial: a thread parked in `stream->Read` only unblocks once the RPC
    /// finishes, which needs the handler to return, so it cannot be joined
    /// before returning (the deadlock rdk's recv path hit). `TryCancel` unblocks
    /// the Read but flattens the client-visible status to CANCELLED, losing the
    /// specific fault code. A proper fix points at the async CompletionQueue
    /// gRPC API -- a large change, disproportionate for this landing.
    ///
    /// @param batch_source Pull-source for the next batch of waypoints.
    /// @param update_handler Handler invoked for each update the implementation emits.
    /// @param extra Any additional arguments to the method.
    /// @return How the stream ended.
    virtual stream_outcome move_through_joint_positions_streamed(
        std::function<boost::optional<std::vector<trajectory_point>>()> batch_source,
        std::function<bool(trajectory_update)> update_handler,
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

    API api() const override;

   protected:
    explicit Arm(std::string name);
};

template <>
struct API::traits<Arm> {
    static API api();
};

namespace proto_convert_details {

template <>
struct to_proto_impl<Arm::trajectory_point> {
    void operator()(const Arm::trajectory_point&, viam::component::arm::v1::TrajectoryPoint*) const;
};

template <>
struct from_proto_impl<viam::component::arm::v1::TrajectoryPoint> {
    Arm::trajectory_point operator()(const viam::component::arm::v1::TrajectoryPoint*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
