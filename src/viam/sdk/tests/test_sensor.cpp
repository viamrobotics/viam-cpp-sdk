#define BOOST_TEST_MODULE test module test_sensor
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/tests/mocks/mock_sensor.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {

using namespace sensor;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_sensor)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockSensor sensor("mock_sensor");
    auto api = sensor.api();
    auto static_api = API::get<Sensor>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "sensor");
}

BOOST_AUTO_TEST_CASE(test_get_readings) {
    std::shared_ptr<MockSensor> mock = MockSensor::get_mock_sensor();
    client_to_mock_pipeline<Sensor>(mock, [](Sensor& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct readings = client.get_readings();

        BOOST_CHECK(readings.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockSensor> mock = MockSensor::get_mock_sensor();
    client_to_mock_pipeline<Sensor>(mock, [](Sensor& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockSensor> mock = MockSensor::get_mock_sensor();
    client_to_mock_pipeline<Sensor>(mock, [](Sensor& client) {
        std::vector<sdk::GeometryConfig> expected = fake_geometries();
        std::vector<sdk::GeometryConfig> geometries = client.get_geometries();
        BOOST_CHECK(expected == geometries);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
