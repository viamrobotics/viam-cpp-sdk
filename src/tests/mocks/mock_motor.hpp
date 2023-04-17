#pragma once

#include <common/v1/common.pb.h>
#include <component/motor/v1/motor.grpc.pb.h>
#include <component/motor/v1/motor.pb.h>

#include <components/motor/client.hpp>
#include <components/motor/motor.hpp>
#include <components/motor/server.hpp>

namespace viam {
namespace cppsdktests {
namespace motor {

class MockMotor : public viam::cppsdk::Motor {
   public:
    void set_power(double power_pct) override;
    void go_for(double rpm, double revolutions) override;
    void go_to(double rpm, double position_revolutions) override;
    void reset_zero_position(double offset) override;
    viam::cppsdk::Motor::position get_position() override;
    viam::cppsdk::Motor::properties get_properties() override;
    void stop_motor() override;
    viam::cppsdk::Motor::power_status get_power_status() override;
    viam::cppsdk::Motor::moving_status is_moving() override;
    AttributeMap do_command(AttributeMap command) override;
    static std::shared_ptr<MockMotor> get_mock_motor();

    MockMotor(std::string name) : Motor(std::move(name)){};

   private:
    // TODO add private members for mock
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map;
};

// TODO create fake_xxx functions

}  // namespace motor
}  // namespace cppsdktests
}  // namespace viam
