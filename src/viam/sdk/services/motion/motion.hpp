/// @file services/motion/motion.hpp
///
/// @brief Defines a `Motion` service.
#pragma once

#include <string>

#include <viam/api/service/motion/v1/motion.pb.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/world_state.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/services/service.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

/// @defgroup Motion Classes related to the Motion service.

/// @class MotionRegistration
/// @brief Defines a `ResourceRegistration` for the `Motion` service.
/// @ingroup Motion
class MotionRegistration : public ResourceRegistration {
   public:
    explicit MotionRegistration(const google::protobuf::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

/// @struct motion_configuration
/// @brief Defines configuration options for certain `Motion` APIs.
/// @ingroup Motion
struct motion_configuration {
    /// @brief The name of the vision service(s) that will be used for obstacle detection.
    std::vector<Name> vision_services;

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
    virtual bool move(const pose_in_frame& destination,
                      const Name& name,
                      std::shared_ptr<WorldState> world_state,
                      std::shared_ptr<constraints> constraints) = 0;

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
    virtual bool move_on_map(const pose& destination,
                             const Name& component_name,
                             const Name& slam_name) = 0;

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
    virtual bool move_on_globe(const geo_point& destination,
                               const boost::optional<double>& heading,
                               const Name& component_name,
                               const Name& movement_sensor_name,
                               const std::vector<geo_obstacle>& obstacles,
                               std::shared_ptr<motion_configuration> motion_configuration) = 0;

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
    virtual pose_in_frame get_pose(
        const Name& component_name,
        const std::string& destination_frame,
        const std::vector<WorldState::transform>& supplemental_transforms) = 0;

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

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

   protected:
    explicit Motion(std::string name);
};

}  // namespace sdk
}  // namespace viam
