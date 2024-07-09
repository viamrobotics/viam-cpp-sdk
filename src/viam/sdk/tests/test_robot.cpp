#include <algorithm>
#include <memory>
#include <ostream>
#include <typeinfo>
#include <unordered_set>
#include <utility>

#define BOOST_TEST_MODULE test module test_robot
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/camera.hpp>
#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/tests/mocks/camera_mocks.hpp>
#include <viam/sdk/tests/mocks/generic_mocks.hpp>
#include <viam/sdk/tests/mocks/mock_motor.hpp>
#include <viam/sdk/tests/mocks/mock_robot.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::RobotClient::discovery_query)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::RobotClient::discovery)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::RobotClient::frame_system_config)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::RobotClient::status)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::RobotClient::operation)

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
    auto test_server = TestServer(server);
    auto grpc_channel = test_server.grpc_in_process_channel();
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

BOOST_AUTO_TEST_CASE(test_resource_names) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            std::vector<Name> names = client->resource_names();
            auto mocks = mock_resource_names_response();
            BOOST_TEST(names == mocks, boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_get_status) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto mock_statuses = mock_status_response();

            // get all resource statuses
            auto statuses = client->get_status();

            // ensure we get statuses for all resources, and that they are as expected.
            BOOST_CHECK_EQUAL(statuses.size(), 3);
            BOOST_TEST(statuses == mock_statuses, boost::test_tools::per_element());

            // get only a subset of status responses
            auto names = mock_resource_names_response();
            std::vector<Name> some_names{names[0], names[1]};
            auto some_statuses = client->get_status(some_names);
            // ensure that we only get two of the three existing statuses
            BOOST_CHECK_EQUAL(some_statuses.size(), 2);

            std::vector<RobotClient::status> some_mock_statuses{mock_statuses[0], mock_statuses[1]};

            BOOST_TEST(some_statuses == some_mock_statuses, boost::test_tools::per_element());
        });
}

// This test ensures that the functions in the `mock_robot` files have the same fields for both
// the proto and custom type versions.
BOOST_AUTO_TEST_CASE(test_frame_system_config) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto configs = mock_config_response();
            auto config1 = configs[0];
            auto config2 = configs[1];
            auto protos = mock_proto_config_response();
            auto proto1 = protos[0];
            auto proto2 = protos[1];

            BOOST_CHECK_EQUAL(config1.frame.reference_frame, proto1.frame().reference_frame());
            BOOST_CHECK_EQUAL(config1.frame.pose_in_observer_frame.pose.coordinates.x,
                              proto1.frame().pose_in_observer_frame().pose().x());
            BOOST_CHECK_EQUAL(config1.frame.pose_in_observer_frame.pose.orientation.o_x,
                              proto1.frame().pose_in_observer_frame().pose().o_x());
            BOOST_CHECK_EQUAL(config1.frame.pose_in_observer_frame.pose.theta,
                              proto1.frame().pose_in_observer_frame().pose().theta());
            BOOST_CHECK_EQUAL(map_to_struct(config1.kinematics).SerializeAsString(),
                              proto1.kinematics().SerializeAsString());

            BOOST_CHECK_EQUAL(config2.frame.reference_frame, proto2.frame().reference_frame());
            BOOST_CHECK_EQUAL(config2.frame.pose_in_observer_frame.pose.coordinates.x,
                              proto2.frame().pose_in_observer_frame().pose().x());
            BOOST_CHECK_EQUAL(config2.frame.pose_in_observer_frame.pose.orientation.o_x,
                              proto2.frame().pose_in_observer_frame().pose().o_x());
            BOOST_CHECK_EQUAL(config2.frame.pose_in_observer_frame.pose.theta,
                              proto2.frame().pose_in_observer_frame().pose().theta());
            BOOST_CHECK_EQUAL(map_to_struct(config2.kinematics).SerializeAsString(),
                              proto2.kinematics().SerializeAsString());
        });
}

BOOST_AUTO_TEST_CASE(test_get_frame_system_config) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto mock_fs_config = mock_config_response();
            auto fs_config = client->get_frame_system_config();

            BOOST_TEST(mock_fs_config == fs_config, boost::test_tools::per_element());
        });
}

// This test ensures that the functions in the `mock_robot` files have the same fields for both
// the proto and custom type versions.
BOOST_AUTO_TEST_CASE(test_operation) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto ops = mock_operations_response();
            auto op1 = ops[0];
            auto op2 = ops[1];
            auto protos = mock_proto_operations_response();
            auto proto1 = protos[0];
            auto proto2 = protos[1];

            BOOST_CHECK_EQUAL(op1.id, proto1.id());
            BOOST_CHECK_EQUAL(op1.method, proto1.method());
            BOOST_CHECK_EQUAL(*op1.session_id, proto1.session_id());
            BOOST_CHECK_EQUAL(op2.id, proto2.id());
            BOOST_CHECK_EQUAL(op2.method, proto2.method());
            BOOST_CHECK_EQUAL(*op2.session_id, proto2.session_id());
        });
}

BOOST_AUTO_TEST_CASE(test_get_operations) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto ops = client->get_operations();
            auto mock_ops = mock_operations_response();

            BOOST_TEST(ops == mock_ops, boost::test_tools::per_element());
        });
}

// This test ensures that the functions in the `mock_robot` files have the same fields for both
// the proto and custom type versions.
BOOST_AUTO_TEST_CASE(test_discovery) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto components = mock_discovery_response();
            auto component = components[0];
            auto results = component.results->begin();
            auto protos = mock_proto_discovery_response();
            auto proto = protos[0];
            auto proto_results = proto.results().fields().begin();

            BOOST_CHECK_EQUAL(component.query.subtype, proto.query().subtype());
            BOOST_CHECK_EQUAL(component.query.model, proto.query().model());
            BOOST_CHECK_EQUAL(results->first, proto_results->first);
            // the `Value` type in our mock responses is a `list` type so we can comprehensively
            // test `ProtoType` conversions. Unfortunately the protobuf `ListValue` type doesn't
            // seem to have `==` defined, so we convert to a `DebugString` here to verify
            // comparison and to provide helpful printing of differences in case of an error.
            BOOST_CHECK_EQUAL(results->second->proto_value().DebugString(),
                              proto_results->second.DebugString());
        });
}

BOOST_AUTO_TEST_CASE(test_discover_components) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            auto components = client->discover_components({});
            auto mock_components = mock_discovery_response();

            BOOST_TEST(components == mock_components, boost::test_tools::per_element());
        });
}

BOOST_AUTO_TEST_CASE(test_transform_pose) {
    robot_client_to_mocks_pipeline(
        [](std::shared_ptr<RobotClient> client, MockRobotService& service) -> void {
            pose_in_frame pif;
            auto pose = client->transform_pose(pif, "", {});
            auto mock_pose = mock_transform_response();

            BOOST_CHECK_EQUAL(pose, mock_pose);
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
