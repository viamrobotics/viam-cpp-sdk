#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/motor/v1/motor.grpc.pb.h>
#include <viam/api/component/motor/v1/motor.pb.h>

#include <viam/sdk/components/motor/client.hpp>
#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/components/motor/server.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdktests {
namespace motor {

using viam::sdk::Motor;

class MockMotor : public Motor {
   public:
    virtual ~MockMotor();
    void set_power(double power_pct) override;
    void go_for(double rpm, double revolutions) override;
    void go_to(double rpm, double position_revolutions) override;
    void reset_zero_position(double offset) override;
    Motor::position get_position() override;
    Motor::properties get_properties() override;
    grpc::StatusCode stop(viam::sdk::AttributeMap extra) override;
    Motor::power_status get_power_status() override;
    bool is_moving() override;
    viam::sdk::AttributeMap do_command(viam::sdk::AttributeMap command) override;
    static std::shared_ptr<MockMotor> get_mock_motor();

    MockMotor(std::string name) : Motor(std::move(name)){};

   private:
    Motor::position position_;
    Motor::power_status power_status_;
    Motor::properties properties_;
    // TODO(RSDK-2747) swap to AttributeMap
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<viam::sdk::ProtoType>>> map_;
};

Motor::position fake_position();
Motor::power_status fake_power_status();
Motor::properties fake_properties();

}  // namespace motor
}  // namespace sdktests
}  // namespace viam
