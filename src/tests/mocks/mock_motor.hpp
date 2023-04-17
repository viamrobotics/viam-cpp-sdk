#pragma once

#include <common/v1/common.pb.h>
#include <component/motor/v1/motor.grpc.pb.h>
#include <component/motor/v1/motor.pb.h>

#include <components/motor/motor.hpp>
#include <components/motor/client.hpp>
#include <components/motor/server.hpp>

class MockMotor : public Motor {
    public:
    void set_power(double power_pct) override;
    void go_for(double rpm, double revolutions) override;
    void go_to(double rpm, double position_revolutions) override;
    void reset_zero_position(double offset) override;
    position get_position() override;
    properties get_properties() override;
    void stop() override;
    power_status get_power_status() override;
    moving_status is_moving() override;
    AttributeMap do_command(ERROR TODO) override;
    static std::shared_ptr<MockMotor> get_mock_motor();

    MockMotor(std::string name) : Motor(std::move(name)){};

   private:
    // TODO add private members for mock
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map;
};

// TODO create fake_xxx functions
