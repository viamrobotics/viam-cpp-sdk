#pragma once

#include "viam/sdk/common/proto_type.hpp"
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
    void set_power(double power_pct) override;
    void set_power(double power_pct, const sdk::AttributeMap& extra) override;
    void go_for(double rpm, double revolutions) override;
    void go_for(double rpm, double revolutions, const sdk::AttributeMap& extra) override;
    void go_to(double rpm, double position_revolutions) override;
    void go_to(double rpm, double position_revolutions, const sdk::AttributeMap& extra) override;
    void reset_zero_position(double offset) override;
    void reset_zero_position(double offset, const sdk::AttributeMap& extra) override;
    Motor::position get_position() override;
    Motor::position get_position(const sdk::AttributeMap& extra) override;
    Motor::properties get_properties() override;
    Motor::properties get_properties(const sdk::AttributeMap& extra) override;
    grpc::StatusCode stop() override;
    grpc::StatusCode stop(const viam::sdk::AttributeMap& extra) override;
    Motor::power_status get_power_status() override;
    Motor::power_status get_power_status(const viam::sdk::AttributeMap& extra) override;
    bool is_moving() override;
    viam::sdk::AttributeMap do_command(const viam::sdk::AttributeMap& command) override;
    static std::shared_ptr<MockMotor> get_mock_motor();
    virtual std::vector<sdk::GeometryConfig> get_geometries() override;
    virtual std::vector<sdk::GeometryConfig> get_geometries(
        const sdk::AttributeMap& extra) override;

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
