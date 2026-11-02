#define BOOST_TEST_MODULE test module test_framesystem
#include <viam/sdk/services/framesystem.hpp>

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/tests/mocks/mock_robot.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::FrameSystem::frame_system_config)

namespace viam {
namespace sdktests {
namespace framesystem {

using namespace viam::sdk;

// The frame system has no server of its own, its RPCs are served by RobotService, so we stand up a
// MockRobotService and build the FrameSystem client the same way RobotClient does. The test case
// also gets the mock so it can inspect what reached the server.
template <typename F>
void framesystem_client_to_mock_pipeline(F&& test_case) {
    auto rm = std::make_shared<ResourceManager>();
    auto server = std::make_shared<sdk::Server>();
    robot::MockRobotService service(rm, *server);
    server->start();

    auto test_server = TestServer(server);
    auto channel = ViamChannel(test_server.grpc_in_process_channel());
    auto client = Registry::get()
                      .lookup_resource_client(API::get<FrameSystem>())
                      ->create_rpc_client(FrameSystem::kPublicName, channel);

    std::forward<F>(test_case)(*std::dynamic_pointer_cast<FrameSystem>(client), service);

    server->shutdown();
}

BOOST_AUTO_TEST_CASE(test_api_and_name) {
    BOOST_CHECK_EQUAL(API::get<FrameSystem>().to_string(), "rdk-internal:service:frame_system");
    BOOST_CHECK_EQUAL(FrameSystem::kPublicName, "$framesystem");
    BOOST_CHECK(FrameSystem::public_name() ==
                Name::from_string("rdk-internal:service:frame_system/$framesystem"));

    framesystem_client_to_mock_pipeline([](FrameSystem& client, robot::MockRobotService&) {
        BOOST_CHECK_EQUAL(client.name(), FrameSystem::kPublicName);
        BOOST_CHECK(client.api() == API::get<FrameSystem>());
        BOOST_CHECK(client.get_resource_name() == FrameSystem::public_name());
    });
}

BOOST_AUTO_TEST_CASE(test_get_frame_system_config) {
    framesystem_client_to_mock_pipeline([](FrameSystem& client, robot::MockRobotService&) {
        auto configs = client.get_frame_system_config();
        BOOST_TEST(configs == robot::mock_config_response(), boost::test_tools::per_element());
    });
}

BOOST_AUTO_TEST_CASE(test_get_pose) {
    framesystem_client_to_mock_pipeline([](FrameSystem& client, robot::MockRobotService& service) {
        auto pose = client.get_pose("mock_motor");
        BOOST_CHECK_EQUAL(pose, robot::mock_get_pose_response("world"));

        auto request = service.last_get_pose_request();
        BOOST_CHECK_EQUAL(request.component_name(), "mock_motor");
        BOOST_CHECK_EQUAL(request.destination_frame(), "world");
        BOOST_CHECK_EQUAL(request.supplemental_transforms_size(), 0);
        BOOST_CHECK_EQUAL(request.extra().fields_size(), 0);

        WorldState::transform extra_frame;
        extra_frame.reference_frame = "extra-frame";
        extra_frame.pose_in_observer_frame = pose_in_frame("world", robot::default_pose());
        pose = client.get_pose("mock_motor", "mock_camera", {extra_frame}, fake_map());
        BOOST_CHECK_EQUAL(pose, robot::mock_get_pose_response("mock_camera"));

        request = service.last_get_pose_request();
        BOOST_CHECK_EQUAL(request.component_name(), "mock_motor");
        BOOST_CHECK_EQUAL(request.destination_frame(), "mock_camera");
        BOOST_REQUIRE_EQUAL(request.supplemental_transforms_size(), 1);
        BOOST_CHECK_EQUAL(request.supplemental_transforms(0).reference_frame(), "extra-frame");
        BOOST_CHECK(from_proto(request.extra()) == fake_map());
    });
}

BOOST_AUTO_TEST_CASE(test_transform_pose) {
    framesystem_client_to_mock_pipeline([](FrameSystem& client, robot::MockRobotService&) {
        pose_in_frame query("mock_motor", robot::default_pose());
        auto pose = client.transform_pose(query, "world");
        BOOST_CHECK_EQUAL(pose, robot::mock_transform_response());
    });
}

BOOST_AUTO_TEST_CASE(test_transform_pcd) {
    framesystem_client_to_mock_pipeline([](FrameSystem& client, robot::MockRobotService& service) {
        const std::vector<unsigned char> pcd = {'i', 'n', 'p', 'u', 't', 0x00, 0xff};
        auto transformed = client.transform_pcd(pcd, "mock_camera", "world");
        BOOST_CHECK(transformed == robot::mock_transform_pcd_response());

        auto request = service.last_transform_pcd_request();
        BOOST_CHECK_EQUAL(request.point_cloud_pcd(), bytes_to_string(pcd));
        BOOST_CHECK_EQUAL(request.source(), "mock_camera");
        BOOST_CHECK_EQUAL(request.destination(), "world");
    });
}

}  // namespace framesystem
}  // namespace sdktests
}  // namespace viam
