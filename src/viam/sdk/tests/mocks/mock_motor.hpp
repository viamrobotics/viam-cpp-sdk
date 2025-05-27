#pragma once

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdktests {
namespace motor {

using viam::sdk::Motor;

class MockMotor : public Motor {
   public:
    void set_power(double power_pct, const sdk::ProtoStruct& extra) override;
    void go_for(double rpm, double revolutions, const sdk::ProtoStruct& extra) override;
    void go_to(double rpm, double position_revolutions, const sdk::ProtoStruct& extra) override;
    void set_rpm(double rpm, const sdk::ProtoStruct& extra) override;
    void reset_zero_position(double offset, const sdk::ProtoStruct& extra) override;
    Motor::position get_position(const sdk::ProtoStruct& extra) override;
    Motor::properties get_properties(const sdk::ProtoStruct& extra) override;
    void stop(const viam::sdk::ProtoStruct& extra) override;
    Motor::power_status get_power_status(const viam::sdk::ProtoStruct& extra) override;
    bool is_moving() override;
    viam::sdk::ProtoStruct do_command(const viam::sdk::ProtoStruct& command) override;
    static std::shared_ptr<MockMotor> get_mock_motor();
    virtual std::vector<sdk::GeometryConfig> get_geometries(const sdk::ProtoStruct& extra) override;

    MockMotor(std::string name) : Motor(std::move(name)) {}

    using Motor::get_geometries;
    using Motor::get_position;
    using Motor::get_power_status;
    using Motor::get_properties;
    using Motor::go_for;
    using Motor::go_to;
    using Motor::reset_zero_position;
    using Motor::set_power;
    using Motor::set_rpm;
    using Motor::stop;

   private:
    Motor::position position_;
    Motor::power_status power_status_;
    Motor::properties properties_;
    viam::sdk::ProtoStruct map_;
};

Motor::position fake_position();
Motor::power_status fake_power_status();
Motor::properties fake_properties();

}  // namespace motor
}  // namespace sdktests
}  // namespace viam
