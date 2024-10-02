#define BOOST_TEST_MODULE test module test_servo

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/servo.hpp>
#include <viam/sdk/tests/mocks/mock_servo.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {
namespace servo {

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_mock)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockServo servo("mock_servo");
    auto api = servo.api();
    auto static_api = API::get<Servo>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "servo");
}

BOOST_AUTO_TEST_CASE(mock_move_and_get_position) {
    std::shared_ptr<MockServo> servo = MockServo::get_mock_servo();

    BOOST_CHECK(!servo->is_moving());
    servo->move(3);
    BOOST_CHECK_EQUAL(servo->get_position(), 3);
    BOOST_CHECK(servo->is_moving());

    servo->move(5);
    BOOST_CHECK_EQUAL(servo->get_position(), 5);
    BOOST_CHECK(servo->is_moving());
}

BOOST_AUTO_TEST_CASE(mock_stop) {
    std::shared_ptr<MockServo> servo = MockServo::get_mock_servo();
    servo->move(3);
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
    ProtoStruct expected = fake_map();

    ProtoStruct command = fake_map();
    ProtoStruct result_map = servo->do_command(command);

    BOOST_CHECK(result_map.at("test") == expected.at("test"));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_servo_client_server)

BOOST_AUTO_TEST_CASE(test_move_and_get_position) {
    std::shared_ptr<MockServo> mock = MockServo::get_mock_servo();
    client_to_mock_pipeline<Servo>(mock, [](Servo& client) {
        BOOST_CHECK(!client.is_moving());
        client.move(3);
        BOOST_CHECK_EQUAL(client.get_position(), 3);
        BOOST_CHECK(client.is_moving());

        client.move(5);
        BOOST_CHECK_EQUAL(client.get_position(), 5);
        BOOST_CHECK(client.is_moving());
    });
}

BOOST_AUTO_TEST_CASE(test_stop) {
    std::shared_ptr<MockServo> mock = MockServo::get_mock_servo();
    client_to_mock_pipeline<Servo>(mock, [](Servo& client) {
        client.move(3);
        BOOST_CHECK(client.is_moving());
        client.stop();
        BOOST_CHECK(!client.is_moving());
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockServo> mock = MockServo::get_mock_servo();
    client_to_mock_pipeline<Servo>(mock, [](Servo& client) {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockServo> mock = MockServo::get_mock_servo();
    client_to_mock_pipeline<Servo>(mock, [](Servo& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace servo
}  // namespace sdktests
}  // namespace viam
