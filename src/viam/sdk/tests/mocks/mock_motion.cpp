#include "viam/sdk/services/motion.hpp"
#include <viam/sdk/tests/mocks/mock_motion.hpp>

#include <chrono>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace motion {

using namespace viam::sdk;

bool MockMotion::move(const pose_in_frame& destination,
                      const Name& component_name,
                      const std::shared_ptr<WorldState>& world_state,
                      const std::shared_ptr<constraints>& constraints,
                      const AttributeMap&) {
    this->current_location = destination;
    this->peek_component_name = component_name;
    this->peek_world_state = world_state;
    this->peek_constraints = constraints;
    return true;
}

std::string MockMotion::move_on_map(
    const pose& destination,
    const Name& component_name,
    const Name& slam_name,
    const std::shared_ptr<motion_configuration>& motion_configuration,
    const std::vector<GeometryConfig>& obstacles,
    const AttributeMap&) {
    this->peek_current_pose = destination;
    this->peek_component_name = component_name;
    this->peek_slam_name = slam_name;
    this->current_location.pose = destination;
    this->peek_motion_configuration = motion_configuration;
    this->peek_map_obstacles = obstacles;

    return "execution-id";
}

std::string MockMotion::move_on_globe(
    const geo_point& destination,
    const boost::optional<double>& heading,
    const Name& component_name,
    const Name& movement_sensor_name,
    const std::vector<geo_geometry>& obstacles,
    const std::shared_ptr<motion_configuration>& motion_configuration,
    const std::vector<geo_geometry>& bounding_regions,
    const AttributeMap&) {
    this->peek_heading = *heading;
    this->peek_component_name = component_name;
    this->peek_movement_sensor_name = movement_sensor_name;
    this->peek_destination = destination;
    this->peek_obstacles = obstacles;
    this->peek_motion_configuration = motion_configuration;

    return "execution-id";
}

pose_in_frame MockMotion::get_pose(const Name&,
                                   const std::string&,
                                   const std::vector<WorldState::transform>&,
                                   const AttributeMap&) {
    return current_location;
}

Motion::plan_with_status MockMotion::get_plan(const sdk::Name&,
                                              const std::string&,
                                              const sdk::AttributeMap&) {
    return fake_plan_with_status();
}

std::pair<Motion::plan_with_status, std::vector<Motion::plan_with_status>>
MockMotion::get_plan_with_replan_history(const sdk::Name&,
                                         const std::string&,
                                         const sdk::AttributeMap&) {
    return {fake_plan_with_status(), {fake_plan_with_status()}};
}

Motion::plan_with_status MockMotion::get_latest_plan(const sdk::Name&, const sdk::AttributeMap&) {
    return fake_plan_with_status();
}

std::pair<Motion::plan_with_status, std::vector<Motion::plan_with_status>>
MockMotion::get_latest_plan_with_replan_history(const sdk::Name&, const sdk::AttributeMap&) {
    return {fake_plan_with_status(), {fake_plan_with_status()}};
}

std::vector<Motion::plan_status_with_id> MockMotion::list_active_plan_statuses(
    const sdk::AttributeMap&) {
    return {fake_plan_status_with_id()};
}

std::vector<Motion::plan_status_with_id> MockMotion::list_plan_statuses(const sdk::AttributeMap&) {
    return {fake_plan_status_with_id()};
}

void MockMotion::stop_plan(const sdk::Name&, const sdk::AttributeMap&) {
    this->peek_stop_plan_called = true;
}

AttributeMap MockMotion::do_command(const AttributeMap& command) {
    return command;
};

std::shared_ptr<MockMotion> MockMotion::get_mock_motion() {
    auto motion = std::make_shared<MockMotion>("mock_motion");
    motion->current_location = init_fake_pose();

    return motion;
};

Motion::plan_with_status MockMotion::fake_plan_with_status() {
    plan_with_status pws;
    pws.plan = {"id", fake_component_name(), "exec-id", {}};
    pws.status = fake_plan_status();
    pws.status_history = {fake_plan_status()};

    return pws;
}

pose_in_frame fake_pose() {
    return pose_in_frame("fake-reference-frame", {{0, 1, 2}, {3, 4, 5}, 6});
}

Motion::plan_status MockMotion::fake_plan_status() {
    return {Motion::plan_state::k_succeeded,
            std::chrono::time_point<long long, std::chrono::nanoseconds>::max(),
            boost::optional<std::string>("reason")};
}

Motion::plan_status_with_id MockMotion::fake_plan_status_with_id() {
    return {"plan_id", fake_component_name(), "execution_id", fake_plan_status()};
}

pose_in_frame init_fake_pose() {
    return pose_in_frame("", {{0, 0, 0}, {0, 0, 0}, 0});
}

Name fake_component_name() {
    return {{"acme", "component", "fake"}, "", "fake-component"};
}

Name fake_slam_name() {
    return {{"acme", "service", "slam"}, "", "fake-slam"};
}

Name fake_movement_sensor_name() {
    return {{"acme", "component", ""}, "", "fake-movement-sensor"};
}

geo_point fake_geo_point() {
    return {5, 10};
}

std::shared_ptr<motion_configuration> fake_motion_configuration() {
    auto mc = std::make_shared<motion_configuration>();
    mc->plan_deviation_m = 55;
    return mc;
}

std::vector<geo_geometry> fake_obstacles() {
    GeometryConfig gc;
    gc.set_pose({{20, 25, 30}, {35, 40, 45}, 50});
    gc.set_geometry_type(sdk::sphere);
    struct sphere sphere({1});
    gc.set_geometry_specifics(sphere);
    gc.set_label("label");
    gc.set_orientation_config({AxisAngles, {}, axis_angles{1, 2, 3, 4}});
    return {{fake_geo_point(), {std::move(gc)}}};
}

std::vector<geo_geometry> fake_bounding_regions() {
    GeometryConfig gc;
    gc.set_pose({{1, 2, 3}, {0, 0, 0}, 90});
    gc.set_geometry_type(sdk::sphere);
    struct sphere sphere({2});
    gc.set_geometry_specifics(sphere);
    gc.set_label("label");
    gc.set_orientation_config({AxisAngles, {}, axis_angles{1, 2, 3, 4}});
    return {{fake_geo_point(), {std::move(gc)}}};
}

}  // namespace motion
}  // namespace sdktests
}  // namespace viam
