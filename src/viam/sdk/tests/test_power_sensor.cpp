#define BOOST_TEST_MODULE test module test_powersensor
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.grpc.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/private/power_sensor_client.hpp>
#include <viam/sdk/components/power_sensor.hpp>
#include <viam/sdk/components/private/power_sensor_server.hpp>
#include <viam/sdk/tests/mocks/mock_power_sensor.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {

using namespace powersensor;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_powersensor)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockPowerSensor power_sensor("mock_power_sensor");
    auto api = power_sensor.api();
    auto static_api = API::get<PowerSensor>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "power_sensor");
}

BOOST_AUTO_TEST_CASE(test_get_voltage) {
    std::shared_ptr<MockPowerSensor> mock = MockPowerSensor::get_mock_powersensor();
    client_to_mock_pipeline<PowerSensorClient>(mock, [&](PowerSensor& client) {
        mock->peek_voltage = {10.0, false};
        BOOST_CHECK(client.get_voltage() == mock->peek_voltage);
    });
}

BOOST_AUTO_TEST_CASE(test_get_current) {
    std::shared_ptr<MockPowerSensor> mock = MockPowerSensor::get_mock_powersensor();
    client_to_mock_pipeline<PowerSensorClient>(mock, [&](PowerSensor& client) {
        mock->peek_current = {10.0, false};
        BOOST_CHECK(client.get_current() == mock->peek_current);
    });
}

BOOST_AUTO_TEST_CASE(test_get_power) {
    std::shared_ptr<MockPowerSensor> mock = MockPowerSensor::get_mock_powersensor();
    client_to_mock_pipeline<PowerSensorClient>(mock, [&](PowerSensor& client) {
        mock->peek_power = 10.0;
        BOOST_CHECK(client.get_power() == mock->peek_power);
    });
}

BOOST_AUTO_TEST_CASE(test_get_readings) {
    std::shared_ptr<MockPowerSensor> mock = MockPowerSensor::get_mock_powersensor();
    client_to_mock_pipeline<PowerSensorClient>(mock, [](PowerSensor& client) {
        AttributeMap expected = fake_map();

        AttributeMap readings = client.get_readings();

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(readings->at(std::string("test")));
        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockPowerSensor> mock = MockPowerSensor::get_mock_powersensor();
    client_to_mock_pipeline<PowerSensorClient>(mock, [](PowerSensor& client) {
        AttributeMap expected = fake_map();

        AttributeMap command = fake_map();
        AttributeMap result_map = client.do_command(command);

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(result_map->at(std::string("test")));
        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
