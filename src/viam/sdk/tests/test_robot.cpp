#include <algorithm>
#include <memory>
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

namespace viam {
namespace sdktests {
namespace robot {

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_robot)

template <typename Lambda>
void server_to_client_pipeline(Lambda&& func) {
    MockRobotService service;
    auto manager = service.resource_manager();

    manager->add(std::string("mock_generic"), generic::MockGeneric::get_mock_generic());
    manager->add(std::string("mock_motor"), motor::MockMotor::get_mock_motor());
    manager->add(std::string("mock_camera"), camera::MockCamera::get_mock_camera());

    ::grpc::ServerBuilder builder;
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();

    grpc::ChannelArguments args;
    auto grpc_channel = server->InProcessChannel(args);
    auto viam_channel = std::make_shared<ViamChannel>(grpc_channel, "", nullptr);
    auto client = RobotClient::with_channel(viam_channel, Options(0, boost::none));

    // Run the passed test on the created std::stack
    std::forward<Lambda>(func)(client, service);

    //  shutdown afterwards
    server->Shutdown();
}

BOOST_AUTO_TEST_CASE(test_registering_resources) {
    // To test with mock resources we need to be able to create them, which means registering
    // constructors. This tests that we register correctly.
    Model camera_model("fake", "fake", "mock_camera");
    std::shared_ptr<ModelRegistration> cr = std::make_shared<ModelRegistration>(
        ResourceType("Camera"),
        Camera::static_subtype(),
        camera_model,
        [](Dependencies, ResourceConfig cfg) { return camera::MockCamera::get_mock_camera(); },
        [](ResourceConfig cfg) -> std::vector<std::string> { return {}; });
    Registry::register_resource(cr);

    Model generic_model("fake", "fake", "mock_generic");
    std::shared_ptr<ModelRegistration> gr = std::make_shared<ModelRegistration>(
        ResourceType("Generic"),
        Generic::static_subtype(),
        generic_model,
        [](Dependencies, ResourceConfig cfg) { return generic::MockGeneric::get_mock_generic(); },
        [](ResourceConfig cfg) -> std::vector<std::string> { return {}; });
    Registry::register_resource(gr);

    Model motor_model("fake", "fake", "mock_motor");
    std::shared_ptr<ModelRegistration> mr = std::make_shared<ModelRegistration>(
        ResourceType("Motor"),
        Motor::static_subtype(),
        motor_model,
        [](Dependencies, ResourceConfig cfg) { return motor::MockMotor::get_mock_motor(); },
        [](ResourceConfig cfg) -> std::vector<std::string> { return {}; });
    Registry::register_resource(mr);

    BOOST_CHECK(Registry::lookup_resource(Camera::static_subtype(), camera_model));
    BOOST_CHECK(Registry::lookup_resource(Generic::static_subtype(), generic_model));
    BOOST_CHECK(Registry::lookup_resource(Motor::static_subtype(), motor_model));
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
    server_to_client_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            std::vector<ResourceName>* resource_names = client->resource_names();
            auto names = vec_to_string_util(*resource_names);
            auto mocks = mock_resource_names_response();

            auto mock_resp = vec_to_string_util(mocks);
            BOOST_TEST(names == mock_resp, boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_get_status) {
    server_to_client_pipeline(
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
    server_to_client_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto mock_fs_config = mock_config_response();
            auto fs_config = client->get_frame_system_config();

            BOOST_TEST(vec_to_string_util(mock_fs_config) == vec_to_string_util(fs_config),
                       boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_get_operations) {
    server_to_client_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto ops = client->get_operations();
            auto mock_ops = mock_operations_response();

            BOOST_TEST(vec_to_string_util(ops) == vec_to_string_util(mock_ops),
                       boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_discover_components) {
    server_to_client_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto components = client->discover_components({});
            auto mock_components = mock_discovery_response();

            BOOST_TEST(vec_to_string_util(components) == vec_to_string_util(mock_components),
                       boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_transform_pose) {
    server_to_client_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            viam::common::v1::PoseInFrame pif;
            auto pose = client->transform_pose(pif, "", {});
            auto mock_pose = mock_transform_response();

            BOOST_CHECK_EQUAL(pose.DebugString(), mock_pose.DebugString());
        });
}

BOOST_AUTO_TEST_CASE(test_stop_all) {
    server_to_client_pipeline(
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

BOOST_AUTO_TEST_SUITE_END()

}  // namespace robot
}  // namespace sdktests
}  // namespace viam
