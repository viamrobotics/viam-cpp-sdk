#define BOOST_TEST_MODULE test module test_motion

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/world_state.hpp>
#include <viam/sdk/services/motion.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/tests/mocks/mock_motion.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::WorldState)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::geo_geometry>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::Motion::plan_status_with_id)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::Motion::plan_status_with_id>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::Motion::plan_with_status)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::Motion::plan_with_status>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {
namespace motion {

using namespace viam::sdk;

WorldState mock_world_state() {
    WorldState::transform transform;
    transform.reference_frame = "fake-reference-frame";
    transform.pose_in_observer_frame = pose_in_frame("fake", {{2, 3, 4}, {5, 6, 7}, 8});
    WorldState::geometries_in_frame obstacle;
    obstacle.reference_frame = "ref";

    return WorldState({obstacle}, {transform});
}

BOOST_AUTO_TEST_SUITE(test_mock)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockMotion motion("mock_motion");
    auto api = motion.api();
    auto static_api = API::get<Motion>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "motion");
}

BOOST_AUTO_TEST_CASE(mock_move_and_get_pose) {
    std::shared_ptr<MockMotion> motion = MockMotion::get_mock_motion();

    BOOST_CHECK_EQUAL(motion->current_location, init_fake_pose());

    Name fake_name({"acme", "service", "motion"}, "fake-remote", "fake-motion");
    auto ws = std::make_shared<WorldState>(mock_world_state());

    bool success = motion->move(fake_pose(), fake_name, ws, 0, fake_map());

    BOOST_TEST(success);
    BOOST_CHECK_EQUAL(motion->current_location, fake_pose());
    BOOST_CHECK_EQUAL(motion->peek_component_name, fake_name);
    BOOST_CHECK_EQUAL(*(motion->peek_world_state), *ws);
}

BOOST_AUTO_TEST_CASE(mock_move_on_map) {
    std::shared_ptr<MockMotion> motion = MockMotion::get_mock_motion();

    BOOST_CHECK_EQUAL(motion->current_location, init_fake_pose());

    pose new_destination({{3, 4, 5}, {6, 7, 8}, 9});

    std::string execution_id = motion->move_on_map(new_destination,
                                                   fake_component_name(),
                                                   fake_slam_name(),
                                                   fake_motion_configuration(),
                                                   fake_geometries(),
                                                   fake_map());

    BOOST_CHECK_EQUAL(execution_id, "execution-id");
    BOOST_CHECK_EQUAL(motion->peek_current_pose, std::move(new_destination));
    BOOST_CHECK_EQUAL(motion->peek_component_name, fake_component_name());
    BOOST_CHECK_EQUAL(motion->peek_slam_name, fake_slam_name());
    BOOST_CHECK_EQUAL(*(motion->peek_motion_configuration), *(fake_motion_configuration()));
    BOOST_CHECK_EQUAL(motion->peek_map_obstacles, fake_geometries());
}

BOOST_AUTO_TEST_CASE(mock_move_on_globe) {
    std::shared_ptr<MockMotion> motion = MockMotion::get_mock_motion();

    std::string execution_id = motion->move_on_globe(fake_geo_point(),
                                                     15,
                                                     fake_component_name(),
                                                     fake_movement_sensor_name(),
                                                     fake_obstacles(),
                                                     fake_motion_configuration(),
                                                     fake_bounding_regions(),
                                                     fake_map());

    BOOST_CHECK_EQUAL(execution_id, "execution-id");
    BOOST_CHECK_EQUAL(motion->peek_destination, fake_geo_point());
    BOOST_CHECK_EQUAL(motion->peek_heading, 15);
    BOOST_CHECK_EQUAL(motion->peek_component_name, fake_component_name());
    BOOST_CHECK_EQUAL(motion->peek_movement_sensor_name, fake_movement_sensor_name());
    BOOST_CHECK_EQUAL(motion->peek_obstacles, fake_obstacles());
    BOOST_CHECK_EQUAL(*(motion->peek_motion_configuration), *(fake_motion_configuration()));
    BOOST_CHECK_EQUAL(motion->peek_bounding_regions, fake_bounding_regions());
}

BOOST_AUTO_TEST_CASE(mock_get_plan) {
    std::shared_ptr<MockMotion> mock = MockMotion::get_mock_motion();

    auto ret1 = mock->get_plan(fake_component_name(), "execution_id", fake_map());
    BOOST_CHECK_EQUAL(ret1, mock->fake_plan_with_status());

    auto ret2 = mock->get_latest_plan(fake_component_name(), fake_map());
    BOOST_CHECK_EQUAL(ret1, ret2);
}

BOOST_AUTO_TEST_CASE(mock_get_plan_with_replan_history) {
    std::shared_ptr<MockMotion> mock = MockMotion::get_mock_motion();
    auto ret1 =
        mock->get_plan_with_replan_history(fake_component_name(), "execution_id", fake_map());

    BOOST_CHECK_EQUAL(ret1.first, mock->fake_plan_with_status());
    BOOST_CHECK_EQUAL(ret1.second.size(), 1);
    BOOST_CHECK_EQUAL(ret1.second[0], mock->fake_plan_with_status());

    auto ret2 = mock->get_latest_plan_with_replan_history(fake_component_name(), fake_map());

    BOOST_CHECK_EQUAL(ret1.first, ret2.first);
    BOOST_CHECK_EQUAL(ret1.second, ret2.second);
}

BOOST_AUTO_TEST_CASE(mock_stop_plan) {
    std::shared_ptr<MockMotion> mock = MockMotion::get_mock_motion();
    BOOST_CHECK(!mock->peek_stop_plan_called);
    mock->stop_plan(fake_component_name(), fake_map());
    BOOST_CHECK(mock->peek_stop_plan_called);
}

BOOST_AUTO_TEST_CASE(mock_list_plan_statuses) {
    std::shared_ptr<MockMotion> mock = MockMotion::get_mock_motion();
    std::vector<Motion::plan_status_with_id> statuses1 = mock->list_plan_statuses(fake_map());
    BOOST_CHECK_EQUAL(statuses1.size(), 1);
    BOOST_CHECK_EQUAL(statuses1[0], mock->fake_plan_status_with_id());

    std::vector<Motion::plan_status_with_id> statuses2 =
        mock->list_active_plan_statuses(fake_map());

    BOOST_CHECK_EQUAL(statuses1, statuses2);
}

BOOST_AUTO_TEST_CASE(mock_do_command) {
    std::shared_ptr<MockMotion> motion = MockMotion::get_mock_motion();
    ProtoStruct expected = fake_map();

    ProtoStruct command = fake_map();
    ProtoStruct result_map = motion->do_command(command);

    BOOST_CHECK(result_map.at("test") == expected.at("test"));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_motion_client_server)

BOOST_AUTO_TEST_CASE(test_move_and_get_pose) {
    auto mock = std::make_shared<MockMotion>("mock_motion");
    client_to_mock_pipeline<Motion>(mock, [](Motion& client) {
        std::string destination_frame("destination");
        std::vector<WorldState::transform> transforms;
        ProtoStruct extra = fake_map();
        pose_in_frame pose = client.get_pose(fake_component_name(), destination_frame, {}, extra);
        BOOST_CHECK_EQUAL(pose, init_fake_pose());

        auto ws = std::make_shared<WorldState>(mock_world_state());
        bool success = client.move(fake_pose(), fake_component_name(), ws, nullptr, fake_map());
        BOOST_TEST(success);

        pose = client.get_pose(fake_component_name(), destination_frame, transforms, extra);
        BOOST_CHECK_EQUAL(pose, fake_pose());
    });
}

BOOST_AUTO_TEST_CASE(test_move_on_map) {
    auto mock = std::make_shared<MockMotion>("mock_motion");
    client_to_mock_pipeline<Motion>(mock, [](Motion& client) {
        pose destination({{1, 2, 3}, {4, 5, 6}, 7});
        std::string execution_id = client.move_on_map(destination,
                                                      fake_component_name(),
                                                      fake_slam_name(),
                                                      fake_motion_configuration(),
                                                      fake_geometries(),
                                                      fake_map());

        BOOST_CHECK_EQUAL(execution_id, "execution-id");

        pose_in_frame pose = client.get_pose(fake_component_name(), "", {}, fake_map());
        BOOST_CHECK_EQUAL(pose.pose, destination);
    });
}

BOOST_AUTO_TEST_CASE(test_move_on_globe) {
    auto mock = std::make_shared<MockMotion>("mock_motion");
    client_to_mock_pipeline<Motion>(mock, [](Motion& client) {
        std::string execution_id = client.move_on_globe(fake_geo_point(),
                                                        15,
                                                        fake_component_name(),
                                                        fake_movement_sensor_name(),
                                                        fake_obstacles(),
                                                        fake_motion_configuration(),
                                                        fake_bounding_regions(),
                                                        fake_map());

        BOOST_CHECK_EQUAL(execution_id, "execution-id");
    });
}

BOOST_AUTO_TEST_CASE(test_get_plan) {
    auto mock = std::make_shared<MockMotion>("mock_motion");
    client_to_mock_pipeline<Motion>(mock, [](Motion& client) {
        auto ret1 = client.get_plan(fake_component_name(), "execution_id", fake_map());
        BOOST_CHECK_EQUAL(ret1, MockMotion::fake_plan_with_status());

        auto ret2 = client.get_latest_plan(fake_component_name(), fake_map());
        BOOST_CHECK_EQUAL(ret1, ret2);
    });
}

BOOST_AUTO_TEST_CASE(test_get_plan_with_replan_history) {
    auto mock = std::make_shared<MockMotion>("mock_motion");
    client_to_mock_pipeline<Motion>(mock, [](Motion& client) {
        auto ret1 =
            client.get_plan_with_replan_history(fake_component_name(), "execution_id", fake_map());
        BOOST_CHECK_EQUAL(ret1.first, MockMotion::fake_plan_with_status());
        BOOST_CHECK_EQUAL(ret1.second.size(), 1);
        BOOST_CHECK_EQUAL(ret1.second[0], MockMotion::fake_plan_with_status());

        auto ret2 = client.get_latest_plan_with_replan_history(fake_component_name(), fake_map());
        BOOST_CHECK_EQUAL(ret1.first, ret2.first);
        BOOST_CHECK_EQUAL(ret1.second, ret2.second);
    });
}

BOOST_AUTO_TEST_CASE(test_list_plan_statuses) {
    auto mock = std::make_shared<MockMotion>("mock_motion");
    client_to_mock_pipeline<Motion>(mock, [](Motion& client) {
        std::vector<Motion::plan_status_with_id> statuses1 = client.list_plan_statuses(fake_map());
        BOOST_CHECK_EQUAL(statuses1.size(), 1);
        BOOST_CHECK_EQUAL(statuses1[0], MockMotion::fake_plan_status_with_id());

        std::vector<Motion::plan_status_with_id> statuses2 =
            client.list_active_plan_statuses(fake_map());

        BOOST_CHECK_EQUAL(statuses1, statuses2);
    });
}

BOOST_AUTO_TEST_CASE(test_stop_plan) {
    auto mock = std::make_shared<MockMotion>("mock_motion");
    client_to_mock_pipeline<Motion>(mock, [&](Motion& client) {
        client.stop_plan(fake_component_name());
        BOOST_CHECK(mock->peek_stop_plan_called);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    auto mock = std::make_shared<MockMotion>("mock_motion");
    client_to_mock_pipeline<Motion>(mock, [](Motion& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace motion
}  // namespace sdktests
}  // namespace viam
