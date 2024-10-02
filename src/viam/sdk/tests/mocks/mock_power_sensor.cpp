#include <viam/sdk/tests/mocks/mock_power_sensor.hpp>

#include <viam/sdk/components/power_sensor.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace powersensor {

using namespace viam::sdk;

PowerSensor::voltage MockPowerSensor::get_voltage(const ProtoStruct&) {
    return this->peek_voltage;
};
PowerSensor::current MockPowerSensor::get_current(const ProtoStruct&) {
    return this->peek_current;
};
double MockPowerSensor::get_power(const ProtoStruct&) {
    return this->peek_power;
};
ProtoStruct MockPowerSensor::get_readings(const ProtoStruct&) {
    return fake_map();
};
ProtoStruct MockPowerSensor::do_command(const ProtoStruct& command) {
    return command;
};

std::shared_ptr<MockPowerSensor> MockPowerSensor::get_mock_powersensor() {
    return std::make_shared<MockPowerSensor>("mock_powersensor");
}

}  // namespace powersensor
}  // namespace sdktests
}  // namespace viam
