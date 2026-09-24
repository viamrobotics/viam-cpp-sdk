#pragma once

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/services/motion.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdktests {
namespace motion {

sdk::pose_in_frame init_fake_pose();
sdk::pose_in_frame fake_pose();

std::string fake_component_name();
std::string fake_slam_name();
std::string fake_movement_sensor_name();
sdk::geo_point fake_geo_point();
std::vector<sdk::geo_geometry> fake_obstacles();
std::shared_ptr<sdk::motion_configuration> fake_motion_configuration();
std::vector<sdk::geo_geometry> fake_bounding_regions();

class MockMotion : public sdk::Motion {
   public:
    bool move(const sdk::pose_in_frame& destination,
              const std::string& component_name,
              const std::shared_ptr<sdk::WorldState>& world_state,
              const std::shared_ptr<constraints>& constraints,
              const sdk::ProtoStruct& extra) override;

    std::string move_on_map(const sdk::pose& destination,
                            const std::string& component_name,
                            const std::string& slam_name,
                            const std::shared_ptr<sdk::motion_configuration>& motion_configuration,
                            const std::vector<sdk::GeometryConfig>& obstacles,
                            const sdk::ProtoStruct& extra) override;

    std::string move_on_globe(
        const sdk::geo_point& destination,
        const boost::optional<double>& heading,
        const std::string& component_name,
        const std::string& movement_sensor_name,
        const std::vector<sdk::geo_geometry>& obstacles,
        const std::shared_ptr<sdk::motion_configuration>& motion_configuration,
        const std::vector<sdk::geo_geometry>& bounding_regions,
        const sdk::ProtoStruct& extra) override;

    sdk::pose_in_frame get_pose(
        const std::string& component_name,
        const std::string& destination_frame,
        const std::vector<sdk::WorldState::transform>& supplemental_transforms,
        const sdk::ProtoStruct& extra) override;

    plan_with_status get_plan(const std::string& component_name,
                              const std::string& execution_id,
                              const sdk::ProtoStruct& extra) override;

    std::pair<plan_with_status, std::vector<plan_with_status>> get_plan_with_replan_history(
        const std::string& component_name,
        const std::string& execution_id,
        const sdk::ProtoStruct& extra) override;

    plan_with_status get_latest_plan(const std::string& component_name,
                                     const sdk::ProtoStruct& extra) override;

    std::pair<plan_with_status, std::vector<plan_with_status>> get_latest_plan_with_replan_history(
        const std::string& component_name, const sdk::ProtoStruct& extra) override;

    std::vector<plan_status_with_id> list_plan_statuses(const sdk::ProtoStruct& extra) override;

    std::vector<plan_status_with_id> list_active_plan_statuses(
        const sdk::ProtoStruct& extra) override;

    void stop_plan(const std::string& name, const sdk::ProtoStruct& extra) override;

    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;
    sdk::ProtoStruct get_status() override;
    static std::shared_ptr<MockMotion> get_mock_motion();
    static plan_status fake_plan_status();
    static plan_with_status fake_plan_with_status();
    static plan_status_with_id fake_plan_status_with_id();

    sdk::stream_outcome temp_stream_arm_joint_positions(
        const std::function<boost::optional<TempStreamArmJointPositionsRequest_Targets>()>& batch_source,
        const std::function<bool(TempStreamArmJointPositionsResponse)>& update_handler,
        const TempStreamArmJointPositionsRequest_Init& init_request) override;

    // These variables allow the testing infra to `peek` into the mock
    // and ensure that the correct values were passed
    sdk::pose_in_frame current_location;
    sdk::pose peek_current_pose;
    std::string peek_component_name;
    std::string peek_movement_sensor_name;
    std::string peek_slam_name;
    sdk::geo_point peek_destination;
    std::string peek_destination_frame;
    double peek_heading;
    bool peek_stop_plan_called = false;
    std::string peek_debug_key;
    std::vector<sdk::geo_geometry> peek_obstacles;
    std::vector<sdk::GeometryConfig> peek_map_obstacles;
    std::shared_ptr<constraints> peek_constraints;
    std::shared_ptr<sdk::motion_configuration> peek_motion_configuration;
    std::vector<sdk::geo_geometry> peek_bounding_regions;
    std::shared_ptr<sdk::WorldState> peek_world_state;

    Motion::TempStreamArmJointPositionsRequest_Init peek_temp_stream_init_request;
    std::vector<Motion::TempStreamArmJointPositionsRequest_Targets> peek_temp_stream_batches;
    int peek_temp_stream_ack_count = 0;
    enum class stream_fault : std::uint8_t { k_none = 0, k_runtime_error = 1, k_grpc_status = 2 };
    stream_fault temp_stream_fault = stream_fault::k_none;

    MockMotion(std::string name)
        : sdk::Motion(std::move(name)), current_location(init_fake_pose()) {}
};

}  // namespace motion
}  // namespace sdktests
}  // namespace viam

// This function is used to create a fake motion configuration.
// It is used in tests to provide a default motion configuration.
inline std::shared_ptr<sdk::motion_configuration> fake_motion_configuration() {
    auto mc = std::make_shared<sdk::motion_configuration>();
    mc->obstacle_segment_length = 1.0;
    mc->rotation_allowance = 0.1;
    mc->rotation_retries = 10;
    mc->linear_weight = 0.5;
    mc->angular_weight = 0.5;
    mc->default_elevation = 0.0;
    mc->ignore_theta = true;
    return mc;
}