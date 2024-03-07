#include <viam/sdk/tests/mocks/mock_sensor.hpp>

#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace sensor {

using namespace viam::sdk;

AttributeMap MockSensor::get_readings(const AttributeMap&) {
    return fake_map();
};
AttributeMap MockSensor::do_command(const AttributeMap& command) {
    return command;
};
std::vector<sdk::GeometryConfig> MockSensor::get_geometries(const AttributeMap&) {
    return fake_geometries();
};

std::shared_ptr<MockSensor> MockSensor::get_mock_sensor() {
    auto sensor = std::make_shared<MockSensor>("mock_sensor");
    return sensor;
}

}  // namespace sensor
}  // namespace sdktests
}  // namespace viam
