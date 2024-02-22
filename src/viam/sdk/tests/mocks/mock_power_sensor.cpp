#include <viam/sdk/tests/mocks/mock_power_sensor.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.grpc.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.pb.h>

#include <viam/sdk/components/power_sensor/power_sensor.hpp>
#include <viam/sdk/components/power_sensor/server.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace powersensor {

using namespace viam::sdk;

PowerSensor::voltage MockPowerSensor::get_voltage(const AttributeMap&) {
    return this->peek_voltage;
};
PowerSensor::current MockPowerSensor::get_current(const AttributeMap&) {
    return this->peek_current;
};
double MockPowerSensor::get_power(const AttributeMap&) {
    return this->peek_power;
};
AttributeMap MockPowerSensor::get_readings(const AttributeMap&) {
    return fake_map();
};
AttributeMap MockPowerSensor::do_command(const AttributeMap& command) {
    return command;
};

std::shared_ptr<MockPowerSensor> MockPowerSensor::get_mock_powersensor() {
    return std::make_shared<MockPowerSensor>("mock_powersensor");
}

}  // namespace powersensor
}  // namespace sdktests
}  // namespace viam
