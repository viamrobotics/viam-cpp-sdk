#define BOOST_TEST_MODULE test module test_servo

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/servo/v1/servo.grpc.pb.h>
#include <viam/api/component/servo/v1/servo.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/servo/client.hpp>
#include <viam/sdk/components/servo/server.hpp>
#include <viam/sdk/components/servo/servo.hpp>
#include <viam/sdk/tests/mocks/mock_servo.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {
namespace servo {

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_mock)

BOOST_AUTO_TEST_CASE(mock_move_and_get_position) {
    std::shared_ptr<MockServo> servo = MockServo::get_mock_servo();

    BOOST_CHECK(!servo->is_moving());
    servo->move(static_cast<unsigned>(3));
    BOOST_CHECK_EQUAL(servo->get_position(), 3);
    BOOST_CHECK(servo->is_moving());

    servo->move(static_cast<unsigned>(5));
    BOOST_CHECK_EQUAL(servo->get_position(), 5);
    BOOST_CHECK(servo->is_moving());
}

BOOST_AUTO_TEST_CASE(mock_stop) {
    std::shared_ptr<MockServo> servo = MockServo::get_mock_servo();
    servo->move(static_cast<unsigned>(3));
    BOOST_CHECK(servo->is_moving());
    servo->stop();
    BOOST_CHECK(!servo->is_moving());
}

BOOST_AUTO_TEST_CASE(mock_get_geometries) {
    std::shared_ptr<MockServo> servo = MockServo::get_mock_servo();
    std::vector<GeometryConfig> expected_geometries = fake_geometries();
    std::vector<GeometryConfig> result_geometries = servo->get_geometries();

    BOOST_CHECK_EQUAL(result_geometries, expected_geometries);
}

BOOST_AUTO_TEST_CASE(mock_do_command) {
    std::shared_ptr<MockServo> servo = MockServo::get_mock_servo();
    AttributeMap expected = fake_map();

    AttributeMap command = fake_map();
    AttributeMap result_map = servo->do_command(command);

    ProtoType expected_pt = *(expected->at(std::string("test")));
    ProtoType result_pt = *(result_map->at(std::string("test")));

    BOOST_CHECK(result_pt == expected_pt);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_servo_client_server)

// This sets up the following architecture
// -- MockComponent
//        /\
//
//        | (function calls)
//
//        \/
// -- ComponentServer (Real)
//        /\
//
//        | (grpc InProcessChannel)
//
//        \/
// -- ComponentClient (Real)
//
// This is as close to a real setup as we can get
// without starting another process
//
// The passed in lambda function has access to the ComponentClient
//
template <typename Lambda>
void server_to_mock_pipeline(Lambda&& func) {
    ServoServer servo_server;
    std::shared_ptr<MockServo> mock = MockServo::get_mock_servo();
    servo_server.resource_manager()->add(std::string("mock_servo"), MockServo::get_mock_servo());

    grpc::ServerBuilder builder;
    builder.RegisterService(&servo_server);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();

    grpc::ChannelArguments args;
    auto grpc_channel = server->InProcessChannel(args);
    ServoClient client("mock_servo", grpc_channel);
    // Run the passed test on the created stack
    std::forward<Lambda>(func)(client);
    // shutdown afterwards
    server->Shutdown();
}

BOOST_AUTO_TEST_CASE(test_move_and_get_position) {
    server_to_mock_pipeline([](Servo& client) -> void {
        BOOST_CHECK(!client.is_moving());
        client.move(static_cast<unsigned>(3));
        BOOST_CHECK_EQUAL(client.get_position(), 3);
        BOOST_CHECK(client.is_moving());

        client.move(static_cast<unsigned>(5));
        BOOST_CHECK_EQUAL(client.get_position(), 5);
        BOOST_CHECK(client.is_moving());
    });
}

BOOST_AUTO_TEST_CASE(test_stop) {
    server_to_mock_pipeline([](Servo& client) -> void {
        client.move(static_cast<unsigned>(3));
        BOOST_CHECK(client.is_moving());
        client.stop();
        BOOST_CHECK(!client.is_moving());
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    server_to_mock_pipeline([](Servo& client) -> void {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    server_to_mock_pipeline([](Servo& client) -> void {
        AttributeMap expected = fake_map();

        AttributeMap command = fake_map();
        AttributeMap result_map = client.do_command(command);

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(result_map->at(std::string("test")));

        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace servo
}  // namespace sdktests
}  // namespace viam
