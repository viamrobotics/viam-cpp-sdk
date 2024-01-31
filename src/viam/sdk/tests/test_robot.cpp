#include <algorithm>
#include <memory>
#include <ostream>
#include <typeinfo>
#include <unordered_set>
#include <utility>

#define BOOST_TEST_MODULE test module test_robot
#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/components/generic/client.hpp>
#include <viam/sdk/components/motor/client.hpp>
#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/tests/mocks/camera_mocks.hpp>
#include <viam/sdk/tests/mocks/generic_mocks.hpp>
#include <viam/sdk/tests/mocks/mock_motor.hpp>
#include <viam/sdk/tests/mocks/mock_robot.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace robot {

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_robot)

// Basically a copy of the client_to_mock_pipeline test utility. This version
// has multiple mock resources and gives the passed in test case access to both
// the robot client and the mock robot service.
template <typename F>
void robot_client_to_mocks_pipeline(F&& test_case) {
    Registry::initialize();
    // Create a ResourceManager. Add a few mock resources to the
    // ResourceManager. Create a Server. Create a MockRobotService from the
    // ResourceManager and Server. Start the Server.
    auto rm = std::make_shared<ResourceManager>();
    rm->add(std::string("mock_generic"), generic::MockGenericComponent::get_mock_generic());
    rm->add(std::string("mock_motor"), motor::MockMotor::get_mock_motor());
    rm->add(std::string("mock_camera"), camera::MockCamera::get_mock_camera());
    auto server = std::make_shared<sdk::Server>();
    MockRobotService service(rm, *server);
    server->start();

    // Create a RobotClient to the MockRobotService over an established
    // in-process gRPC channel.
    grpc::ChannelArguments args;
    auto test_server = TestServer(server);
    auto grpc_channel = test_server.grpc_in_process_channel(args);
    auto viam_channel = std::make_shared<ViamChannel>(grpc_channel, "", nullptr);
    auto client = RobotClient::with_channel(viam_channel, Options(0, boost::none));

    // Run the passed-in test case on the created stack and give access to the
    // created RobotClient and MockRobotService.
    std::forward<F>(test_case)(client, service);

    // Shutdown Server afterward.
    server->shutdown();
}

BOOST_AUTO_TEST_CASE(test_registering_resources) {
    // To test with mock resources we need to be able to create them, which means registering
    // constructors. This tests that we register correctly.
    Model camera_model("fake", "fake", "mock_camera");
    std::shared_ptr<ModelRegistration> cr = std::make_shared<ModelRegistration>(
        API::get<Camera>(),
        camera_model,
        [](Dependencies, ResourceConfig cfg) { return camera::MockCamera::get_mock_camera(); },
        [](ResourceConfig cfg) -> std::vector<std::string> { return {}; });
    Registry::register_model(cr);

    Model generic_model("fake", "fake", "mock_generic");
    std::shared_ptr<ModelRegistration> gr = std::make_shared<ModelRegistration>(
        API::get<GenericComponent>(),
        generic_model,
        [](Dependencies, ResourceConfig cfg) {
            return generic::MockGenericComponent::get_mock_generic();
        },
        [](ResourceConfig cfg) -> std::vector<std::string> { return {}; });
    Registry::register_model(gr);

    Model motor_model("fake", "fake", "mock_motor");
    std::shared_ptr<ModelRegistration> mr = std::make_shared<ModelRegistration>(
        API::get<Motor>(),
        motor_model,
        [](Dependencies, ResourceConfig cfg) { return motor::MockMotor::get_mock_motor(); },
        [](ResourceConfig cfg) -> std::vector<std::string> { return {}; });
    Registry::register_model(mr);

    BOOST_CHECK(Registry::lookup_model(API::get<Camera>(), camera_model));
    BOOST_CHECK(Registry::lookup_model(API::get<GenericComponent>(), generic_model));
    BOOST_CHECK(Registry::lookup_model(API::get<Motor>(), motor_model));
}

template <typename T>
// Sorts our vecs and converts to strings for consistent comparisons
std::vector<std::string> vec_to_string_util(std::vector<T>& vec) {
    std::vector<std::string> ret;
    for (auto& v : vec) {
        ret.push_back(v.SerializeAsString());
    }
    std::sort(ret.begin(), ret.end());
    return ret;
}

BOOST_AUTO_TEST_CASE(test_resource_names) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            std::vector<ResourceName>* resource_names = client->resource_names();
            auto names = vec_to_string_util(*resource_names);
            auto mocks = mock_resource_names_response();

            auto mock_resp = vec_to_string_util(mocks);
            BOOST_TEST(names == mock_resp, boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_get_status) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto mock_statuses = mock_status_response();

            // get all resource statuses
            auto statuses = client->get_status();

            auto status_strs = vec_to_string_util(statuses);
            auto mock_strs = vec_to_string_util(mock_statuses);

            // ensure we get statuses for all resources, and that they are as expected.
            BOOST_CHECK_EQUAL(statuses.size(), 3);
            BOOST_TEST(status_strs == mock_strs, boost::test_tools::per_element());

            // get only a subset of status responses
            auto names = mock_resource_names_response();
            std::vector<ResourceName> some_names{names[0], names[1]};
            auto some_statuses = client->get_status(some_names);
            auto some_status_strs = vec_to_string_util(some_statuses);

            // ensure that we only get two of the three existing statuses
            BOOST_CHECK_EQUAL(some_status_strs.size(), 2);

            // unfortunately the sorting is a bit odd so we end up with a mismatch of index,
            // but this ensures that the statuses we received do exist in the mocks, as
            // expected.
            std::vector<std::string> some_mock_strs{mock_strs[1], mock_strs[2]};

            BOOST_TEST(some_status_strs == some_mock_strs, boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_get_frame_system_config) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto mock_fs_config = mock_config_response();
            auto fs_config = client->get_frame_system_config();

            BOOST_TEST(vec_to_string_util(mock_fs_config) == vec_to_string_util(fs_config),
                       boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_get_operations) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto ops = client->get_operations();
            auto mock_ops = mock_operations_response();

            BOOST_TEST(vec_to_string_util(ops) == vec_to_string_util(mock_ops),
                       boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_discover_components) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto components = client->discover_components({});
            auto mock_components = mock_discovery_response();

            BOOST_TEST(vec_to_string_util(components) == vec_to_string_util(mock_components),
                       boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_transform_pose) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            viam::common::v1::PoseInFrame pif;
            auto pose = client->transform_pose(pif, "", {});
            auto mock_pose = mock_transform_response();

            BOOST_CHECK_EQUAL(pose.DebugString(), mock_pose.DebugString());
        });
}

BOOST_AUTO_TEST_CASE(test_stop_all) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            std::shared_ptr<Resource> rb = service.resource_manager()->resource("mock_motor");
            auto motor = std::dynamic_pointer_cast<motor::MockMotor>(rb);
            BOOST_CHECK(motor);

            motor->set_power(10.0);
            BOOST_CHECK_EQUAL(motor->get_power_status().power_pct, 10.0);

            // stop all should stop the motor, setting its power to 0.0
            client->stop_all();
            BOOST_CHECK_EQUAL(motor->get_power_status().power_pct, 0.0);
        });
}

BOOST_AUTO_TEST_CASE(test_get_resource) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto mock_motor = client->resource_by_name<Motor>("mock_motor");
            BOOST_CHECK(mock_motor);

            // test not just that we can get the motor, but that the motor service has been
            // appropriately registered such that we can actually use it.
            BOOST_CHECK(!mock_motor->is_moving());
        });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace robot
}  // namespace sdktests
}  // namespace viam
