#include <tests/mocks/mock_motor.hpp>

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
    // TODO impl
    return; 
};
void MockMotor::go_for(double rpm, double revolutions) {
    // TODO impl
    return; 
};
void MockMotor::go_to(double rpm, double position_revolutions) {
    // TODO impl
    return; 
};
void MockMotor::reset_zero_position(double offset) {
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
void MockMotor::stop_motor() {
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
AttributeMap MockMotor::do_command(ERROR TODO) {
    // TODO impl
    return; 
};

std::shared_ptr<MockMotor> MockMotor::get_mock_motor() {
    auto motor = std::make_shared<MockMotor>("mock_motor");
    // TODO motor->attr = fake_attr();

    return motor;
}

}  // namespace motor
}  // namespace cppsdktests
}  // namespace viam
