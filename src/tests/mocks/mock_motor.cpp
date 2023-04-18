#include <tests/mocks/mock_motor.hpp>

#include <stdexcept>

#include <common/v1/common.pb.h>
#include <component/motor/v1/motor.grpc.pb.h>
#include <component/motor/v1/motor.pb.h>

#include <components/motor/motor.hpp>
#include <components/motor/server.hpp>
#include <tests/test_utils.hpp>

namespace viam {
namespace cppsdktests {
namespace motor {

using namespace viam::cppsdk;

void MockMotor::set_power(double power_pct) {
    if (std::abs(power_pct) > 1.0) {
        throw std::range_error("power_pct");
    }
    power_status.is_on = power_pct != 0.0;
    power_status.power_pct = power_pct;
};
void MockMotor::go_for(double rpm, double revolutions) {
    position += revolutions;
};
void MockMotor::go_to(double rpm, double position_revolutions) {
    position = position_revolutions;
};
void MockMotor::reset_zero_position(double offset) {
    // If position = x
    // offset = x-y
    // new pos = y,
    // This is not idential to the real world but
    // is equivalent for this mock
    position -= offset;
};
Motor::position MockMotor::get_position() {
    return position;
};
Motor::properties MockMotor::get_properties() {
    return properties;
};
void MockMotor::stop_motor() {
    // None of these functions are async and this mock is not
    // thread-safe (Send, not Sync). The motor should never be
    // moving when this is called
    set_power(0.0);
};
Motor::power_status MockMotor::get_power_status() {
    return power_status;
};
bool MockMotor::is_moving() {
    // None of these functions are async and this mock is not
    // thread-safe (Send, not Sync)
    return false;
};
AttributeMap MockMotor::do_command(AttributeMap _command) {
    return map;
};

std::shared_ptr<MockMotor> MockMotor::get_mock_motor() {
    auto motor = std::make_shared<MockMotor>("mock_motor");

    motor->power_status = fake_power_status();
    motor->position = fake_position();
    motor->properties = fake_properties();
    motor->map = fake_map();

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
    Motor::properties properties;
    properties.position_reporting = true;
    return properties;
}

}  // namespace motor
}  // namespace cppsdktests
}  // namespace viam
