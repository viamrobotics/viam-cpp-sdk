#pragma once

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/power_sensor.hpp>

namespace viam {
namespace sdktests {
namespace powersensor {

class MockPowerSensor : public sdk::PowerSensor {
   public:
    sdk::PowerSensor::voltage get_voltage(const sdk::ProtoStruct& extra) override;
    sdk::PowerSensor::current get_current(const sdk::ProtoStruct& extra) override;
    double get_power(const sdk::ProtoStruct& extra) override;
    sdk::PowerSensor::battery_percent_status get_battery_percent(const sdk::ProtoStruct& extra) override;
    sdk::PowerSensor::properties get_properties(const sdk::ProtoStruct& extra) override;
    sdk::ProtoStruct get_readings(const sdk::ProtoStruct& extra) override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;
    sdk::ProtoStruct get_status() override;
    static std::shared_ptr<MockPowerSensor> get_mock_powersensor();

    // These variables allow the testing infra to `peek` into the mock
    // and ensure that the correct values were passed
    sdk::PowerSensor::voltage peek_voltage;
    sdk::PowerSensor::current peek_current;
    double peek_power;
    sdk::PowerSensor::battery_percent_status peek_battery_percent;
    bool peek_is_charging;
    bool peek_is_battery_percent_supported;
    bool peek_is_charging_supported;

    MockPowerSensor(std::string name) : PowerSensor(std::move(name)) {}
};

}  // namespace powersensor
}  // namespace sdktests
}  // namespace viam