#pragma once

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/services/motion.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdktests {
namespace motion {

sdk::pose_in_frame init_fake_pose();
sdk::pose_in_frame fake_pose();

sdk::Name fake_component_name();
sdk::Name fake_slam_name();
sdk::Name fake_movement_sensor_name();
sdk::geo_point fake_geo_point();
std::vector<sdk::geo_obstacle> fake_obstacles();
std::shared_ptr<sdk::motion_configuration> fake_motion_configuration();

class MockMotion : public sdk::Motion {
   public:
    bool move(const sdk::pose_in_frame& destination,
              const sdk::Name& component_name,
              const std::shared_ptr<sdk::WorldState>& world_state,
              const std::shared_ptr<constraints>& constraints,
              const sdk::AttributeMap& extra) override;

    std::string move_on_map(const sdk::pose& destination,
                            const sdk::Name& component_name,
                            const sdk::Name& slam_name,
                            const std::shared_ptr<sdk::motion_configuration>& motion_configuration,
                            const std::vector<sdk::GeometryConfig>& obstacles,
                            const sdk::AttributeMap& extra) override;

    std::string move_on_globe(
        const sdk::geo_point& destination,
        const boost::optional<double>& heading,
        const sdk::Name& component_name,
        const sdk::Name& movement_sensor_name,
        const std::vector<sdk::geo_obstacle>& obstacles,
        const std::shared_ptr<sdk::motion_configuration>& motion_configuration,
        const sdk::AttributeMap& extra) override;

    sdk::pose_in_frame get_pose(
        const sdk::Name& component_name,
        const std::string& destination_frame,
        const std::vector<sdk::WorldState::transform>& supplemental_transforms,
        const sdk::AttributeMap& extra) override;

    plan_with_status get_plan(const sdk::Name& component_name,
                              const std::string& execution_id,
                              const sdk::AttributeMap& extra) override;

    std::pair<plan_with_status, std::vector<plan_with_status>> get_plan_with_replan_history(
        const sdk::Name& component_name,
        const std::string& execution_id,
        const sdk::AttributeMap& extra) override;

    plan_with_status get_latest_plan(const sdk::Name& component_name,
                                     const sdk::AttributeMap& extra) override;

    std::pair<plan_with_status, std::vector<plan_with_status>> get_latest_plan_with_replan_history(
        const sdk::Name& component_name, const sdk::AttributeMap& extra) override;

    std::vector<plan_status_with_id> list_plan_statuses(const sdk::AttributeMap& extra) override;

    std::vector<plan_status_with_id> list_active_plan_statuses(
        const sdk::AttributeMap& extra) override;

    void stop_plan(const sdk::Name& name, const sdk::AttributeMap& extra) override;

    sdk::AttributeMap do_command(const sdk::AttributeMap& command) override;
    static std::shared_ptr<MockMotion> get_mock_motion();
    static plan_status fake_plan_status();
    static plan_with_status fake_plan_with_status();
    static plan_status_with_id fake_plan_status_with_id();

    // These variables allow the testing infra to `peek` into the mock
    // and ensure that the correct values were passed
    sdk::pose_in_frame current_location;
    sdk::pose peek_current_pose;
    sdk::Name peek_component_name;
    sdk::Name peek_movement_sensor_name;
    sdk::Name peek_slam_name;
    sdk::geo_point peek_destination;
    std::string peek_destination_frame;
    double peek_heading;
    bool peek_stop_plan_called = false;
    std::vector<sdk::geo_obstacle> peek_obstacles;
    std::vector<sdk::GeometryConfig> peek_map_obstacles;
    std::shared_ptr<constraints> peek_constraints;
    std::shared_ptr<sdk::motion_configuration> peek_motion_configuration;
    std::shared_ptr<sdk::WorldState> peek_world_state;

    MockMotion(std::string name)
        : sdk::Motion(std::move(name)), current_location(init_fake_pose()){};
};

}  // namespace motion
}  // namespace sdktests
}  // namespace viam
