/// @file services/motion/motion.hpp
///
/// @brief Defines a `Motion` service.
#pragma once

#include <string>

#include <viam/api/service/motion/v1/motion.pb.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/common/world_state.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/services/service.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

namespace gp = google::protobuf;

/// @defgroup Motion Classes related to the Motion service.

/// @class MotionRegistration
/// @brief Defines a `ResourceRegistration` for the `Motion` service.
/// @ingroup Motion
class MotionRegistration : public ResourceRegistration {
   public:
    explicit MotionRegistration(const gp::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

/// @struct obstacle_detector_name
/// @brief Defines configuration for obstacle detectors by pairing a vision service name and a
/// camera name.
/// @ingroup Motion
struct obstacle_detector {
    /// @brief The name of the vision service to be used for obstacle detection.
    Name vision_service;
    /// @brief The name of the camera component to be used for obstacle detection.
    Name camera;

    service::motion::v1::ObstacleDetector to_proto() const;
    static obstacle_detector from_proto(const service::motion::v1::ObstacleDetector& proto);
    friend bool operator==(const obstacle_detector& lhs, const obstacle_detector& rhs);
    friend std::ostream& operator<<(std::ostream& os, const obstacle_detector& v);
};

/// @struct motion_configuration
/// @brief Defines configuration options for certain `Motion` APIs.
/// @ingroup Motion
struct motion_configuration {
    /// @brief The obstacle detectors to be used for the API call.
    std::vector<obstacle_detector> obstacle_detectors;

    /// @brief If not null, sets the frequency to poll for the position of the robot.
    boost::optional<double> position_polling_frequency_hz;

    /// @brief If not null, sets the frequency to poll the vision service(s) for new obstacles.
    boost::optional<double> obstacle_polling_frequency_hz;

    /// @brief Optional distance in meters a robot is allowed to deviate from the motion plan.
    boost::optional<double> plan_deviation_m;

    /// @brief Optional linear velocity to target when moving
    boost::optional<double> linear_m_per_sec;

    /// @brief Optional angular velocity to target when turning
    boost::optional<double> angular_degs_per_sec;

    service::motion::v1::MotionConfiguration to_proto() const;
    static motion_configuration from_proto(const service::motion::v1::MotionConfiguration& proto);
    friend bool operator==(const motion_configuration& lhs, const motion_configuration& rhs);
    friend std::ostream& operator<<(std::ostream& os, const motion_configuration& v);
};

/// @class Motion motion.hpp "services/motion/motion.hpp"
/// @brief The `Motion` service coordinates motion planning across all components of a given robot.
/// @ingroup Motion
/// The Viam Motion planning service calculates a valid path that avoids self-collision by default.
/// If additional constraints are supplied in the `world_state` message, the motion planning
/// service will account for those as well.
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific motion implementations. This class cannot be used on its own.
class Motion : public Service {
   public:
    /// @enum PlanState
    /// @brief Describes the possible states a plan can be in.
    /// @ingroup Motion
    enum class PlanState : uint8_t {
        in_progress = 0,
        stopped = 1,
        succeeded = 2,
        failed = 3,
    };

    static PlanState from_proto(const service::motion::v1::PlanState& proto);
    static service::motion::v1::PlanState to_proto(const PlanState& state);

    /// @struct plan_status
    /// @brief Describes the state of a given plan at a point in time.
    /// @ingroup Motion
    struct plan_status {
        /// @brief The state of the plan execution
        PlanState state;

        /// @brief The time the executing plan transitioned to the state.
        time_pt timestamp;

        /// @brief The reason for the state change. The error message if the plan failed, or the
        /// re-plan reason if re-planning was necessary.
        boost::optional<std::string> reason;

        static plan_status from_proto(const service::motion::v1::PlanStatus& proto);
        static std::vector<plan_status> from_proto(
            const gp::RepeatedPtrField<service::motion::v1::PlanStatus>& proto);
        service::motion::v1::PlanStatus to_proto() const;
        friend bool operator==(const plan_status& lhs, const plan_status& rhs);
    };

    /// @struct plan_status_with_id
    /// @brief The motion plan status, plus plan ID, component name, and execution ID.
    /// @ingroup Motion
    struct plan_status_with_id {
        /// @brief The unique ID of the plan
        std::string plan_id;

        /// @brief The component to be moved. Used for tracking and stopping.
        Name component_name;

        /// @brief The unique ID which identifies the plan execution.
        std::string execution_id;

        /// @brief The plan status.
        plan_status status;

        static plan_status_with_id from_proto(const service::motion::v1::PlanStatusWithID& proto);
        service::motion::v1::PlanStatusWithID to_proto() const;
        friend bool operator==(const plan_status_with_id& lhs, const plan_status_with_id& rhs);
    };

    /// @struct steps
    /// @brief An ordered list of plan steps.
    /// @ingroup Motion
    struct steps {
        /// @brief An individual "step", representing the state each component (keyed as a fully
        /// qualified component name) should reach while executing that step of the plan.
        typedef std::unordered_map<std::string, pose> step;

        /// @brief The ordered list of steps.
        std::vector<step> steps;

        static struct steps from_proto(
            const gp::RepeatedPtrField<service::motion::v1::PlanStep>& proto);

        static service::motion::v1::PlanStep to_proto(const step& step);
        friend bool operator==(const struct steps& lhs, const struct steps& rhs);
    };

    /// @struct plan
    /// @brief Describes a motion plan.
    /// @ingroup Motion
    struct plan {
        /// @brief The plan's unique ID.
        std::string id;

        /// @brief The component requested to be moved. Used for tracking and stopping.
        Name component_name;

        /// @brief The unique ID which identifies the execution.
        /// Multiple plans can share the same execution_id if they were generated due to replanning.
        std::string execution_id;

        /// @brief An ordered list of plan steps.
        steps steps;

        static plan from_proto(const service::motion::v1::Plan& proto);
        service::motion::v1::Plan to_proto() const;
        friend bool operator==(const plan& lhs, const plan& rhs);
    };

    /// @struct plan_with_status
    /// @brief Describes a plan, its current status, and all status changes that have occurred
    /// previously on that plan.
    /// @ingroup Motion
    struct plan_with_status {
        /// @brief The plan.
        plan plan;

        /// @brief The current status of the plan.
        plan_status status;

        /// @brief The prior status changes that have happened during plan execution.
        std::vector<plan_status> status_history;

        static plan_with_status from_proto(const service::motion::v1::PlanWithStatus& proto);
        static std::vector<plan_with_status> from_proto(
            const gp::RepeatedPtrField<service::motion::v1::PlanWithStatus>& proto);
        service::motion::v1::PlanWithStatus to_proto() const;
        friend bool operator==(const plan_with_status& lhs, const plan_with_status& rhs);
    };

    /// @struct linear_constraint
    /// @brief Specifies that the component being moved should move linearly to its goal.
    struct linear_constraint {
        float line_tolerance_mm;
        float orientation_tolerance_degs;
    };

    /// @struct orientation_constraint
    /// @brief Specifies that the component being moved will not deviate its orientation beyond the
    /// specified threshold.
    struct orientation_constraint {
        float orientation_tolerance_degs;
    };

    /// @struct collision_specification
    /// @brief used to selectively apply obstacle avoidance to specific parts of the robot.
    struct collision_specification {
        struct allowed_frame_collisions {
            std::string frame1;
            std::string frame2;
        };
        std::vector<allowed_frame_collisions> allows;
    };

    /// @struct constraints
    /// @brief Specifies all constraints to be passed to Viam's motion planning, along
    /// with any optional parameters.
    struct constraints {
        std::vector<linear_constraint> linear_constraints;
        std::vector<orientation_constraint> orientation_constraints;
        std::vector<collision_specification> collision_specifications;

        static constraints from_proto(const service::motion::v1::Constraints& proto);
        service::motion::v1::Constraints to_proto() const;
    };

    static API static_api();
    API dynamic_api() const override;

    /// @brief Creates a `ResourceRegistration` for the `Motion` service.
    static std::shared_ptr<ResourceRegistration> resource_registration();

    /// @brief Moves any compononent on the robot to a specified destination.
    /// @param destination Where to move the component to.
    /// @param name Name of the component to be moved.
    /// @param world_state Obstacles to avoid and transforms to add to the robot for the duration of
    /// the move.
    /// @param constraints Constraints to apply to how the robot will move.
    /// @return Whether or not the move was successful.
    inline bool move(const pose_in_frame& destination,
                     const Name& name,
                     std::shared_ptr<WorldState> world_state,
                     std::shared_ptr<constraints> constraints) {
        return move(destination, name, world_state, constraints, {});
    }

    /// @brief Moves any compononent on the robot to a specified destination.
    /// @param destination Where to move the component to.
    /// @param name Name of the component to be moved.
    /// @param world_state Obstacles to avoid and transforms to add to the robot for the duration of
    /// the move.
    /// @param constraints Constraints to apply to how the robot will move.
    /// @extra Any additional arguments to the method.
    /// @return Whether or not the move was successful.
    virtual bool move(const pose_in_frame& destination,
                      const Name& name,
                      std::shared_ptr<WorldState> world_state,
                      std::shared_ptr<constraints> constraints,
                      const AttributeMap& extra) = 0;

    /// @brief Moves any component on the robot to a specific destination on a SLAM map.
    /// @param destination The destination to move to.
    /// @param component_name The component to move.
    /// @param slam_name The name of the slam service from which the SLAM map is requested.
    /// @return Whether or not the move was successful.
    inline bool move_on_map(const pose& destination,
                            const Name& component_name,
                            const Name& slam_name) {
        return move_on_map(destination, component_name, slam_name, {});
    }

    /// @brief Moves any component on the robot to a specific destination on a SLAM map.
    /// @param destination The destination to move to.
    /// @param component_name The component to move.
    /// @param slam_name The name of the slam service from which the SLAM map is requested.
    /// @param extra Any additional arguments to the method.
    /// @return Whether or not the move was successful.
    virtual bool move_on_map(const pose& destination,
                             const Name& component_name,
                             const Name& slam_name,
                             const AttributeMap& extra) = 0;

    /// @brief Moves any component on the robot to a specific destination on a globe.
    /// @param destination The destination to move to.
    /// @param heading Optional compass heading to achieve at the destination in degrees [0-360).
    /// @param component_name The name of the component to move.
    /// @param movement_sensor_name The name of the movement sensor used to check robot location.
    /// @param obstacles Obstacles to be considered for motion planning.
    /// @param motion_configuration Optional set of motion configuration options.
    /// @return Whether or not the move was successful.
    inline bool move_on_globe(const geo_point& destination,
                              const boost::optional<double>& heading,
                              const Name& component_name,
                              const Name& movement_sensor_name,
                              const std::vector<geo_obstacle>& obstacles,
                              std::shared_ptr<motion_configuration> motion_configuration) {
        return move_on_globe(destination,
                             heading,
                             component_name,
                             movement_sensor_name,
                             obstacles,
                             motion_configuration,
                             {});
    }

    /// @brief Moves any component on the robot to a specific destination on a globe.
    /// @param destination The destination to move to.
    /// @param heading Optional compass heading to achieve at the destination in degrees [0-360).
    /// @param component_name The name of the component to move.
    /// @param movement_sensor_name The name of the movement sensor used to check robot location.
    /// @param obstacles Obstacles to be considered for motion planning.
    /// @param motion_configuration Optional set of motion configuration options.
    /// @param extra Any additional arguments to the method.
    /// @return Whether or not the move was successful.
    virtual bool move_on_globe(const geo_point& destination,
                               const boost::optional<double>& heading,
                               const Name& component_name,
                               const Name& movement_sensor_name,
                               const std::vector<geo_obstacle>& obstacles,
                               std::shared_ptr<motion_configuration> motion_configuration,
                               const AttributeMap& extra) = 0;

    /// @brief Get the pose of any component on the robot.
    /// @param component_name The component whose pose is being requested.
    /// @param destination_frame The reference frame in which the component's pose should be
    /// provided.
    /// @param supplemental_transforms Pose information on any additional reference frames that are
    /// needed to compute the component's pose.
    /// @return The pose of the component.
    inline pose_in_frame get_pose(
        const Name& component_name,
        const std::string& destination_frame,
        const std::vector<WorldState::transform>& supplemental_transforms) {
        return get_pose(component_name, destination_frame, supplemental_transforms, {});
    }

    /// @brief Get the pose of any component on the robot.
    /// @param component_name The component whose pose is being requested.
    /// @param destination_frame The reference frame in which the component's pose should be
    /// provided.
    /// @param supplemental_transforms Pose information on any additional reference frames that are
    /// needed to compute the component's pose.
    /// @param extra Any additional arguments to the method.
    /// @return The pose of the component.
    virtual pose_in_frame get_pose(
        const Name& component_name,
        const std::string& destination_frame,
        const std::vector<WorldState::transform>& supplemental_transforms,
        const AttributeMap& extra) = 0;

    /// @brief Stop a currently executing motion plan.
    /// @param component_name the component of the currently executing plan to stop.
    inline void stop_plan(const Name& component_name) {
        return stop_plan(component_name, {});
    }

    /// @brief Stop a currently executing motion plan.
    /// @param component_name the component of the currently executing plan to stop.
    /// @param extra Any additional arguments to the method.
    virtual void stop_plan(const Name& component_name, const AttributeMap& extra) = 0;

    /// @brief Returns the plan and state history of the most recent execution to move a component.
    /// Returns a result if the last execution is still executing, or changed state within the last
    /// 24 hours without an intervening robot reinitialization.
    /// @param component_name The name of the component which the MoveOnGlobe request asked to move.
    /// @param last_plan_only If true, causes only the last plan for the component/execution to be
    /// returned. Defaults to false.
    /// @param execution_id Optional execution id of a previous plan, if you want to know about it.
    inline std::pair<plan_with_status, std::vector<plan_with_status>> get_plan(
        Name component_name, bool last_plan_only, boost::optional<std::string> execution_id) {
        return get_plan(component_name, {}, last_plan_only = false, execution_id = {});
    }

    /// @brief Returns the plan and state history of the most recent execution to move a component.
    /// Returns a result if the last execution is still executing, or changed state within the last
    /// 24 hours without an intervening robot reinitialization.
    /// @param component_name The name of the component which the MoveOnGlobe request asked to move.
    /// @param last_plan_only If true, causes only the last plan for the component/execution to be
    /// returned.
    /// @param execution_id Optional execution id of a previous plan, if you want to know about it.
    /// @param extra Any additional arguments to the method.
    virtual std::pair<plan_with_status, std::vector<plan_with_status>> get_plan(
        const Name& component_name,
        const AttributeMap& extra,
        bool last_plan_only = false,
        boost::optional<std::string> execution_id = {}) = 0;

    /// @brief Returns the status of plans created by MoveOnGlobe requests.
    /// Includes statuses of plans that are executing, or are part of an executing that changed
    /// its state within the last 24 hours.
    /// @param only_active_plans If true, filters returned plans to only those currently active.
    /// Defaults to false.
    inline std::vector<plan_status_with_id> list_plan_statuses(bool only_active_plans = false) {
        return list_plan_statuses({}, only_active_plans);
    }

    /// @brief Returns the status of plans created by MoveOnGlobe requests.
    /// Includes statuses of plans that are executing, or are part of an executing that changed
    /// its state within the last 24 hours.
    /// @param only_active_plans If true, filters returned plans to only those currently active.
    /// Defaults to false.
    /// @param extra Any additional arguments to the method.
    virtual std::vector<plan_status_with_id> list_plan_statuses(const AttributeMap& extra,
                                                                bool only_active_plans = false) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

   protected:
    explicit Motion(std::string name);
};

}  // namespace sdk
}  // namespace viam
