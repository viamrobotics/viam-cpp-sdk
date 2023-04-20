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

using viam::cppsdk::Motor;

class MockMotor : public Motor {
   public:
    void set_power(double power_pct) override;
    void go_for(double rpm, double revolutions) override;
    void go_to(double rpm, double position_revolutions) override;
    void reset_zero_position(double offset) override;
    Motor::position get_position() override;
    Motor::properties get_properties() override;
    grpc::StatusCode stop(viam::cppsdk::AttributeMap extra) override;
    Motor::power_status get_power_status() override;
    bool is_moving() override;
    viam::cppsdk::AttributeMap do_command(viam::cppsdk::AttributeMap command) override;
    static std::shared_ptr<MockMotor> get_mock_motor();

    MockMotor(std::string name) : Motor(std::move(name)){};

   private:
    Motor::position position;
    Motor::power_status power_status;
    Motor::properties properties;
    // TODO(RSDK-2747) swap to AttributeMap
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<viam::cppsdk::ProtoType>>> map;
};

Motor::position fake_position();
Motor::power_status fake_power_status();
Motor::properties fake_properties();

}  // namespace motor
}  // namespace cppsdktests
}  // namespace viam
