#include <viam/sdk/tests/mocks/mock_sensor.hpp>

#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace sensor {

using namespace viam::sdk;

ProtoStruct MockSensor::get_readings(const ProtoStruct&) {
    return fake_map();
};
ProtoStruct MockSensor::do_command(const ProtoStruct& command) {
    return command;
};
std::vector<sdk::GeometryConfig> MockSensor::get_geometries(const ProtoStruct&) {
    return fake_geometries();
};

std::shared_ptr<MockSensor> MockSensor::get_mock_sensor() {
    auto sensor = std::make_shared<MockSensor>("mock_sensor");
    return sensor;
}

}  // namespace sensor
}  // namespace sdktests
}  // namespace viam
