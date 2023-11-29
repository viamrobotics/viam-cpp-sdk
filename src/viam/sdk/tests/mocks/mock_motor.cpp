#include <viam/sdk/tests/mocks/mock_motor.hpp>

#include <stdexcept>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/motor/v1/motor.grpc.pb.h>
#include <viam/api/component/motor/v1/motor.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/components/motor/server.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace motor {

using namespace viam::sdk;

void MockMotor::set_power(double power_pct, const AttributeMap& extra) {
    power_status_.is_on = power_pct != 0.0;
    power_status_.power_pct = power_pct;
}

void MockMotor::go_for(double rpm, double revolutions, const AttributeMap& extra) {
    // This is the actual behavior from rdk:builtin:fake_motor
    if (rpm == 0.0) {
        throw std::runtime_error("Cannot move motor at 0 RPM");
    }
    position_ += revolutions;
}

void MockMotor::go_to(double rpm, double position_revolutions, const AttributeMap& extra) {
    position_ = position_revolutions;
}

void MockMotor::reset_zero_position(double offset, const AttributeMap& extra) {
    position_ -= offset;
}

Motor::position MockMotor::get_position(const AttributeMap& extra) {
    return position_;
}

Motor::properties MockMotor::get_properties(const AttributeMap& extra) {
    return properties_;
}

void MockMotor::stop(const AttributeMap& extra) {
    // None of these functions are async and this mock is not
    // thread-safe (Send, not Sync). The mock motor should never be
    // moving when this is called
    set_power(0.0);
}

Motor::power_status MockMotor::get_power_status(const AttributeMap& extra) {
    return power_status_;
}

std::vector<GeometryConfig> MockMotor::get_geometries(const sdk::AttributeMap& extra) {
    return fake_geometries();
}

bool MockMotor::is_moving() {
    // None of these functions are async and this mock is not
    // thread-safe (Send, not Sync)
    return false;
}

AttributeMap MockMotor::do_command(const AttributeMap& _command) {
    return map_;
}

std::shared_ptr<MockMotor> MockMotor::get_mock_motor() {
    auto motor = std::make_shared<MockMotor>("mock_motor");

    motor->power_status_ = fake_power_status();
    motor->position_ = fake_position();
    motor->properties_ = fake_properties();
    motor->map_ = fake_map();

    return motor;
}

Motor::power_status fake_power_status() {
    Motor::power_status power_status;
    power_status.is_on = true;
    power_status.power_pct = 0.5;
    return power_status;
}

Motor::position fake_position() {
    return 0.0;
}

Motor::properties fake_properties() {
    return {true};
}

}  // namespace motor
}  // namespace sdktests
}  // namespace viam
