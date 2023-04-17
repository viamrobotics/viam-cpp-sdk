#include <tests/mocks/mock_motor.hpp>

#include <common/v1/common.pb.h>
#include <component/motor/v1/motor.grpc.pb.h>
#include <component/motor/v1/motor.pb.h>

#include <components/motor/motor.hpp>
#include <components/motor/server.hpp>
#include <tests/test_utils.hpp>

AttributeMap MockMotor::do_command(AttributeMap command) {
    return map;
}
Motor::void MockMotor::set_power(double power_pct) {
    // TODO impl
    return; 
};
Motor::void MockMotor::go_for(double rpm, double revolutions) {
    // TODO impl
    return; 
};
Motor::void MockMotor::go_to(double rpm, double position_revolutions) {
    // TODO impl
    return; 
};
Motor::void MockMotor::reset_zero_position(double offset) {
    // TODO impl
    return; 
};
Motor::position MockMotor::get_position() {
    // TODO impl
    return; 
};
Motor::properties MockMotor::get_properties() {
    // TODO impl
    return; 
};
Motor::void MockMotor::stop() {
    // TODO impl
    return; 
};
Motor::power_status MockMotor::get_power_status() {
    // TODO impl
    return; 
};
Motor::moving_status MockMotor::is_moving() {
    // TODO impl
    return; 
};
Motor::AttributeMap MockMotor::do_command(ERROR TODO) {
    // TODO impl
    return; 
};
Motor::raw_image MockMotor::get_image(std::string mime_type) {
    return image;
}


std::shared_ptr<MockMotor> MockMotor::get_mock_motor() {
    auto motor = std::make_shared<MockMotor>("motor");
    // TODO motor->attr = fake_attr();

    return motor;
}
