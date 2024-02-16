#define BOOST_TEST_MODULE test module test_motor

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/motor/v1/motor.grpc.pb.h>
#include <viam/api/component/motor/v1/motor.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/private/motor_client.hpp>
#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/components/private/motor_server.hpp>
#include <viam/sdk/tests/mocks/mock_motor.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {
namespace motor {

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_mock)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockMotor motor("mock_motor");
    auto api = motor.api();
    auto static_api = API::get<Motor>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "motor");
}

BOOST_AUTO_TEST_CASE(mock_set_power) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    motor->set_power(1.0);
    BOOST_CHECK(motor->get_power_status().power_pct == 1.0);
    BOOST_CHECK(motor->get_power_status().is_on);
    motor->set_power(0.0);
    BOOST_CHECK(motor->get_power_status().power_pct == 0.0);
    BOOST_CHECK(!motor->get_power_status().is_on);
}

BOOST_AUTO_TEST_CASE(mock_go_for) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    motor->go_for(1.0, 1.0);
    motor->go_for(1.0, 1.5);
    BOOST_CHECK(motor->get_position() == 2.5);
}

BOOST_AUTO_TEST_CASE(mock_go_to) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    motor->go_to(1.0, 1.0);
    BOOST_CHECK(motor->get_position() == 1.0);
}

BOOST_AUTO_TEST_CASE(mock_reset_zero_position) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    motor->go_to(1.0, 1.0);
    BOOST_CHECK(motor->get_position() == 1.0);
    motor->reset_zero_position(1.5);
    BOOST_CHECK(motor->get_position() == -0.5);
}

BOOST_AUTO_TEST_CASE(mock_get_properties) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    BOOST_CHECK(motor->get_properties().position_reporting);
}

BOOST_AUTO_TEST_CASE(mock_stop) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    AttributeMap extra_map = fake_map();
    motor->stop(std::move(extra_map));
    BOOST_CHECK(motor->get_power_status().power_pct == 0.0);
    BOOST_CHECK(!motor->is_moving());
}

BOOST_AUTO_TEST_CASE(mock_get_geometries) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    std::vector<GeometryConfig> expected_geometries = fake_geometries();
    std::vector<GeometryConfig> result_geometries = motor->get_geometries();

    BOOST_CHECK_EQUAL(result_geometries, expected_geometries);
}

BOOST_AUTO_TEST_CASE(mock_do_command) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    AttributeMap expected = fake_map();

    AttributeMap command = fake_map();
    AttributeMap result_map = motor->do_command(command);

    ProtoType expected_pt = *(expected->at(std::string("test")));
    ProtoType result_pt = *(result_map->at(std::string("test")));

    BOOST_CHECK(result_pt == expected_pt);
}

BOOST_AUTO_TEST_CASE(mock_exception_creation) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    BOOST_CHECK_THROW(motor->go_for(0.0, 1.0), Exception);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_motor_client_server)

BOOST_AUTO_TEST_CASE(test_set_power) {
    std::shared_ptr<MockMotor> mock = MockMotor::get_mock_motor();
    client_to_mock_pipeline<MotorClient>(mock, [](Motor& client) {
        client.set_power(1.0);
        BOOST_CHECK(client.get_power_status().power_pct == 1.0);
        client.set_power(0.0);
        BOOST_CHECK(client.get_power_status().power_pct == 0.0);
        BOOST_CHECK(!client.get_power_status().is_on);
        client.set_power(-0.5);
        BOOST_CHECK(client.get_power_status().power_pct == -0.5);
        BOOST_CHECK(client.get_power_status().is_on);
    });
}

BOOST_AUTO_TEST_CASE(test_go_for) {
    std::shared_ptr<MockMotor> mock = MockMotor::get_mock_motor();
    client_to_mock_pipeline<MotorClient>(mock, [](Motor& client) {
        client.go_for(1.0, 1.0);
        BOOST_CHECK(client.get_position() == 1.0);
        client.go_for(1.0, 1.5);
        BOOST_CHECK(client.get_position() == 2.5);
        client.go_for(1.0, -0.5);
        BOOST_CHECK(client.get_position() == 2.0);
    });
}

BOOST_AUTO_TEST_CASE(test_go_to) {
    std::shared_ptr<MockMotor> mock = MockMotor::get_mock_motor();
    client_to_mock_pipeline<MotorClient>(mock, [](Motor& client) {
        client.go_to(1.0, 1.0);
        BOOST_CHECK(client.get_position() == 1.0);
        client.go_to(0.1, -1.0);
        BOOST_CHECK(client.get_position() == -1.0);
    });
}

BOOST_AUTO_TEST_CASE(test_reset_zero_position) {
    std::shared_ptr<MockMotor> mock = MockMotor::get_mock_motor();
    client_to_mock_pipeline<MotorClient>(mock, [](Motor& client) {
        client.go_to(1.0, 1.0);
        BOOST_CHECK(client.get_position() == 1.0);
        client.reset_zero_position(1.5);
        BOOST_CHECK(client.get_position() == -0.5);
    });
}

BOOST_AUTO_TEST_CASE(test_get_properties) {
    std::shared_ptr<MockMotor> mock = MockMotor::get_mock_motor();
    client_to_mock_pipeline<MotorClient>(
        mock, [](Motor& client) { BOOST_CHECK(client.get_properties().position_reporting); });
}

BOOST_AUTO_TEST_CASE(test_stop) {
    std::shared_ptr<MockMotor> mock = MockMotor::get_mock_motor();
    client_to_mock_pipeline<MotorClient>(mock, [](Motor& client) {
        client.set_power(1.0);
        BOOST_CHECK(client.get_power_status().power_pct == 1.0);
        BOOST_CHECK(client.get_power_status().is_on);
        client.stop(AttributeMap());
        BOOST_CHECK(client.get_power_status().power_pct == 0.0);
        BOOST_CHECK(!client.get_power_status().is_on);
        // This test is a no-op for now because is_moving will always
        // return false
        BOOST_CHECK(!client.is_moving());
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockMotor> mock = MockMotor::get_mock_motor();
    client_to_mock_pipeline<MotorClient>(mock, [](Motor& client) {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockMotor> mock = MockMotor::get_mock_motor();
    client_to_mock_pipeline<MotorClient>(mock, [](Motor& client) {
        AttributeMap expected = fake_map();

        AttributeMap command = fake_map();
        AttributeMap result_map = client.do_command(command);

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(result_map->at(std::string("test")));

        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_CASE(test_exception_creation) {
    std::shared_ptr<MockMotor> mock = MockMotor::get_mock_motor();
    client_to_mock_pipeline<MotorClient>(mock, [](Motor& client) {
        BOOST_CHECK_THROW(client.go_for(0.0, 1.0), std::runtime_error);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace motor
}  // namespace sdktests
}  // namespace viam
