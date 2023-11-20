#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/motor/v1/motor.grpc.pb.h>
#include <viam/api/component/motor/v1/motor.pb.h>

#include <viam/sdk/common/proto_type.hpp>
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
    void set_power(double power_pct, const sdk::AttributeMap& extra) override;
    void go_for(double rpm, double revolutions, const sdk::AttributeMap& extra) override;
    void go_to(double rpm, double position_revolutions, const sdk::AttributeMap& extra) override;
    void reset_zero_position(double offset, const sdk::AttributeMap& extra) override;
    Motor::position get_position(const sdk::AttributeMap& extra) override;
    Motor::properties get_properties(const sdk::AttributeMap& extra) override;
    void stop(const viam::sdk::AttributeMap& extra) override;
    Motor::power_status get_power_status(const viam::sdk::AttributeMap& extra) override;
    bool is_moving() override;
    viam::sdk::AttributeMap do_command(const viam::sdk::AttributeMap& command) override;
    static std::shared_ptr<MockMotor> get_mock_motor();
    virtual std::vector<sdk::GeometryConfig> get_geometries(
        const sdk::AttributeMap& extra) override;

    MockMotor(std::string name) : Motor(std::move(name)){};

    // the `extra` param is frequently unnecessary but needs to be supported. Ideally, we'd
    // like to live in a world where implementers of derived classes don't need to go out of
    // their way to support two versions of a method (an `extra` version and a non-`extra`
    // version), and users don't need to pass an unnecessary parameters to all method calls.
    //
    // To do this, we define in the parent resource class a non-virtual version of the methods
    // that calls the virtual method and passes a `nullptr` by default in place of the `extra`
    // param. In order to access these versions of the methods within the client code, however,
    // we need to include these `using` lines.
    using Motor::get_geometries;
    using Motor::get_position;
    using Motor::get_power_status;
    using Motor::get_properties;
    using Motor::go_for;
    using Motor::go_to;
    using Motor::reset_zero_position;
    using Motor::set_power;
    using Motor::stop;

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
