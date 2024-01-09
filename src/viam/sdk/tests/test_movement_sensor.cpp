#define BOOST_TEST_MODULE test module test_movementsensor
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/movementsensor/v1/movementsensor.grpc.pb.h>
#include <viam/api/component/movementsensor/v1/movementsensor.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/movement_sensor/client.hpp>
#include <viam/sdk/components/movement_sensor/movement_sensor.hpp>
#include <viam/sdk/components/movement_sensor/server.hpp>
#include <viam/sdk/tests/mocks/mock_movement_sensor.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {

using namespace movementsensor;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_movementsensor)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockMovementSensor movement_sensor("mock_movement_sensor");
    auto api = movement_sensor.api();
    auto static_api = API::get<MovementSensor>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "movement_sensor");
}

BOOST_AUTO_TEST_CASE(test_linear_vel) {
    std::shared_ptr<MockMovementSensor> mock = MockMovementSensor::get_mock_movementsensor();
    client_to_mock_pipeline<MovementSensorClient>(mock, [&](MovementSensor& client) {
        mock->peek_return_vec = Vector3(1, 2, 3);
        BOOST_CHECK(client.get_linear_velocity().data() == mock->peek_return_vec.data());
    });
}

BOOST_AUTO_TEST_CASE(test_angular_vel) {
    std::shared_ptr<MockMovementSensor> mock = MockMovementSensor::get_mock_movementsensor();
    client_to_mock_pipeline<MovementSensorClient>(mock, [&](MovementSensor& client) {
        mock->peek_return_vec = Vector3(1, 2, -3);
        BOOST_CHECK(client.get_angular_velocity().data() == mock->peek_return_vec.data());
    });
}

BOOST_AUTO_TEST_CASE(test_compass_hading) {
    std::shared_ptr<MockMovementSensor> mock = MockMovementSensor::get_mock_movementsensor();
    client_to_mock_pipeline<MovementSensorClient>(mock, [&](MovementSensor& client) {
        mock->peek_compass_heading.value = 57.23;
        BOOST_CHECK_CLOSE(client.get_compass_heading().value, 57.23, 0.1);
    });
}

BOOST_AUTO_TEST_CASE(test_orientation) {
    std::shared_ptr<MockMovementSensor> mock = MockMovementSensor::get_mock_movementsensor();
    client_to_mock_pipeline<MovementSensorClient>(mock, [&](MovementSensor& client) {
        mock->peek_orientation.o_x = 1.1;
        mock->peek_orientation.o_y = 2.1;
        mock->peek_orientation.o_z = 3.1;
        mock->peek_orientation.theta = 4.1;
        BOOST_CHECK_CLOSE(client.get_orientation().o_x, 1.1, 0.1);
        BOOST_CHECK_CLOSE(client.get_orientation().o_y, 2.1, 0.1);
        BOOST_CHECK_CLOSE(client.get_orientation().o_z, 3.1, 0.1);
        BOOST_CHECK_CLOSE(client.get_orientation().theta, 4.1, 0.1);
    });
}

BOOST_AUTO_TEST_CASE(test_position) {
    std::shared_ptr<MockMovementSensor> mock = MockMovementSensor::get_mock_movementsensor();
    client_to_mock_pipeline<MovementSensorClient>(mock, [&](MovementSensor& client) {
        mock->peek_position.altitude_m = 42.1;
        mock->peek_position.coordinate.latitude = 32.1;
        mock->peek_position.coordinate.longitude = 64.2;
        BOOST_CHECK_CLOSE(client.get_position().altitude_m, 42.1, 0.1);
        BOOST_CHECK_CLOSE(client.get_position().coordinate.latitude, 32.1, 0.1);
        BOOST_CHECK_CLOSE(client.get_position().coordinate.longitude, 64.2, 0.1);
    });
}

BOOST_AUTO_TEST_CASE(test_properties) {
    std::shared_ptr<MockMovementSensor> mock = MockMovementSensor::get_mock_movementsensor();
    client_to_mock_pipeline<MovementSensorClient>(mock, [&](MovementSensor& client) {
        mock->peek_compass_heading.value = 57.23;
        mock->peek_properties.position_supported = true;
        mock->peek_properties.linear_velocity_supported = true;
        mock->peek_properties.linear_acceleration_supported = true;
        mock->peek_properties.orientation_supported = false;
        mock->peek_properties.compass_heading_supported = false;
        mock->peek_properties.angular_velocity_supported = false;
        BOOST_CHECK_EQUAL(client.get_properties().position_supported, true);
        BOOST_CHECK_EQUAL(client.get_properties().linear_acceleration_supported, true);
        BOOST_CHECK_EQUAL(client.get_properties().linear_velocity_supported, true);
        BOOST_CHECK_EQUAL(client.get_properties().orientation_supported, false);
        BOOST_CHECK_EQUAL(client.get_properties().compass_heading_supported, false);
        BOOST_CHECK_EQUAL(client.get_properties().angular_velocity_supported, false);
    });
}

BOOST_AUTO_TEST_CASE(test_accuracy) {
    std::shared_ptr<MockMovementSensor> mock = MockMovementSensor::get_mock_movementsensor();
    client_to_mock_pipeline<MovementSensorClient>(mock, [&](MovementSensor& client) {
        mock->peek_accuracy.clear();
        mock->peek_accuracy.emplace("val1", 0.2);
        mock->peek_accuracy.emplace("val2", 0.4);
        BOOST_CHECK_CLOSE(client.get_accuracy().at("val1"), 0.2, 0.1);
        BOOST_CHECK_CLOSE(client.get_accuracy().at("val2"), 0.4, 0.1);
        BOOST_CHECK_EQUAL(client.get_accuracy().size(), 2);
    });
}

BOOST_AUTO_TEST_CASE(test_linear_accel) {
    std::shared_ptr<MockMovementSensor> mock = MockMovementSensor::get_mock_movementsensor();
    client_to_mock_pipeline<MovementSensorClient>(mock, [&](MovementSensor& client) {
        mock->peek_return_vec = Vector3(-1, 2.1, 3);
        BOOST_CHECK(client.get_linear_acceleration().data() == mock->peek_return_vec.data());
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockMovementSensor> mock = MockMovementSensor::get_mock_movementsensor();
    client_to_mock_pipeline<MovementSensorClient>(mock, [](MovementSensor& client) {
        AttributeMap expected = fake_map();

        AttributeMap command = fake_map();
        AttributeMap result_map = client.do_command(command);

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(result_map->at(std::string("test")));
        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockMovementSensor> mock = MockMovementSensor::get_mock_movementsensor();
    client_to_mock_pipeline<MovementSensorClient>(mock, [](MovementSensor& client) {
        std::vector<sdk::GeometryConfig> expected = fake_geometries();
        std::vector<sdk::GeometryConfig> geometries = client.get_geometries();
        BOOST_CHECK(expected == geometries);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
