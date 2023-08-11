#include "viam/sdk/common/pose_in_frame.hpp"
#include "viam/sdk/services/motion/client.hpp"
#include "viam/sdk/services/motion/server.hpp"
#include "viam/sdk/spatialmath/geometry.hpp"
#define BOOST_TEST_MODULE test module test_motion

#include <boost/test/included/unit_test.hpp>

#include "viam/sdk/common/world_state.hpp"
#include "viam/sdk/tests/test_utils.hpp"
#include <viam/sdk/tests/mocks/mock_motion.hpp>

namespace viam {
namespace sdktests {
namespace motion {

using namespace viam::sdk;

WorldState mock_world_state() {
    WorldState::transform transform;
    transform.reference_frame = "fake-reference-frame";
    transform.pose_in_observer_frame = PoseInFrame("fake", {{2, 3, 4}, {5, 6, 7}, 8});
    WorldState::geometries_in_frame obstacle;
    obstacle.reference_frame = "ref";

    return WorldState({obstacle}, {transform});
}

BOOST_AUTO_TEST_SUITE(test_mock)

BOOST_AUTO_TEST_CASE(mock_move_and_get_pose) {
    std::shared_ptr<MockMotion> motion = MockMotion::get_mock_motion();
    BOOST_CHECK_EQUAL(motion->peek_map, 0);

    BOOST_CHECK_EQUAL(motion->current_location, std::move(init_fake_pose()));

    Name fake_name({"acme", "service", "motion"}, "fake-remote", "fake-motion");
    auto ws = std::make_unique<WorldState>(mock_world_state());

    bool success = motion->move(fake_pose(), fake_name, std::move(ws), 0, fake_map());

    BOOST_TEST(success);
    BOOST_CHECK_EQUAL(motion->current_location, fake_pose());
    BOOST_CHECK_EQUAL(motion->peek_component_name, std::move(fake_name));
    BOOST_CHECK_EQUAL(motion->peek_map, fake_map());
    BOOST_CHECK_EQUAL(motion->peek_world_state, std::move(ws));
}

BOOST_AUTO_TEST_CASE(mock_move_on_map) {
    std::shared_ptr<MockMotion> motion = MockMotion::get_mock_motion();
    BOOST_CHECK_EQUAL(motion->peek_map, 0);

    BOOST_CHECK_EQUAL(motion->current_location, std::move(init_fake_pose()));

    pose new_destination({{3, 4, 5}, {6, 7, 8}, 9});
    Name fake_component({"acme", "component", "fake"}, "", "fake-component");
    Name fake_slam({"acme", "service", "slam"}, "", "fake-slam");

    bool success = motion->move_on_map(new_destination, fake_component, fake_slam, fake_map());

    BOOST_TEST(success);
    BOOST_CHECK_EQUAL(motion->peek_current_pose, std::move(new_destination));
    BOOST_CHECK_EQUAL(motion->peek_component_name, std::move(fake_component));
    BOOST_CHECK_EQUAL(motion->peek_slam_name, std::move(fake_slam));
    BOOST_CHECK_EQUAL(motion->peek_map, fake_map());
}

BOOST_AUTO_TEST_CASE(mock_do_command) {
    std::shared_ptr<MockMotion> motion = MockMotion::get_mock_motion();
    AttributeMap expected = fake_map();

    AttributeMap command = fake_map();
    AttributeMap result_map = motion->do_command(command);

    ProtoType expected_pt = *(expected->at(std::string("test")));
    ProtoType result_pt = *(result_map->at(std::string("test")));

    BOOST_CHECK(result_pt == expected_pt);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_motion_client_server)

// This sets up the following architecture
// -- MockService
//        /\
//
//        | (function calls)
//
//        \/
// -- ServiceServer (Real)
//        /\
//
//        | (grpc InProcessChannel)
//
//        \/
// -- ServiceClient (Real)
//
// This is as close to a real setup as we can get
// without starting another process
//
// The passed in lambda function has access to the ServiceClient
template <typename Lambda>
void server_to_mock_pipeline(Lambda&& func) {
    MotionServer motion_server;
    motion_server.resource_manager()->add(std::string("mock_motion"),
                                          MockMotion::get_mock_motion());

    grpc::ServerBuilder builder;
    builder.RegisterService(&motion_server);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();

    auto grpc_channel = server->InProcessChannel(grpc::ChannelArguments());
    MotionClient client("mock_motion", grpc_channel);
    // Run the passed test on the created stack
    std::forward<Lambda>(func)(client);
    // shutdown afterwards
    server->Shutdown();
}

BOOST_AUTO_TEST_CASE(test_move_and_get_pose) {
    server_to_mock_pipeline([](Motion& client) -> void {
        Name component_name({"rdk", "service", "motion"}, "", "fake-motion");
        std::string destination_frame("destination");
        std::vector<WorldState::transform> transforms;
        AttributeMap extra = fake_map();
        PoseInFrame pose = client.get_pose(component_name, destination_frame, transforms, extra);
        BOOST_CHECK_EQUAL(pose, init_fake_pose());

        auto ws = std::make_unique<WorldState>(mock_world_state());
        bool success =
            client.move(fake_pose(), std::move(component_name), std::move(ws), 0, fake_map());
        BOOST_TEST(success);

        pose = client.get_pose(component_name, destination_frame, transforms, extra);
        BOOST_CHECK_EQUAL(pose, fake_pose());
    });
}

BOOST_AUTO_TEST_CASE(test_move_on_map) {
    server_to_mock_pipeline([](Motion& client) -> void {
        pose destination({{1, 2, 3}, {4, 5, 6}, 7});
        Name component_name({"rdk", "service", "motion"}, "", "fake-motion");
        Name slam_name({"rdk", "service", "slam"}, "", "fake-slam");
        bool success = client.move_on_map(destination, component_name, slam_name, fake_map());

        BOOST_TEST(success);

        PoseInFrame pose = client.get_pose(component_name, "", {}, fake_map());
        BOOST_CHECK_EQUAL(pose.pose, destination);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    server_to_mock_pipeline([](Motion& client) -> void {
        AttributeMap expected = fake_map();

        AttributeMap command = fake_map();
        AttributeMap result_map = client.do_command(command);

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(result_map->at(std::string("test")));

        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace motion
}  // namespace sdktests
}  // namespace viam
