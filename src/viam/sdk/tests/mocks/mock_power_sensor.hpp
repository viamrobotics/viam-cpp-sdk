#pragma once

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/power_sensor.hpp>

namespace viam {
namespace sdktests {
namespace powersensor {

class MockPowerSensor : public sdk::PowerSensor {
   public:
    sdk::PowerSensor::voltage get_voltage(const sdk::AttributeMap& extra) override;
    sdk::PowerSensor::current get_current(const sdk::AttributeMap& extra) override;
    double get_power(const sdk::AttributeMap& extra) override;
    sdk::AttributeMap get_readings(const sdk::AttributeMap& extra) override;
    sdk::AttributeMap do_command(const sdk::AttributeMap& command) override;
    static std::shared_ptr<MockPowerSensor> get_mock_powersensor();

    // These variables allow the testing infra to `peek` into the mock
    // and ensure that the correct values were passed
    sdk::PowerSensor::voltage peek_voltage;
    sdk::PowerSensor::current peek_current;
    double peek_power;

    MockPowerSensor(std::string name) : PowerSensor(std::move(name)){};
};

}  // namespace powersensor
}  // namespace sdktests
}  // namespace viam
