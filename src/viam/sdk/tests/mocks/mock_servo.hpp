#pragma once

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/servo.hpp>

namespace viam {
namespace sdktests {
namespace servo {

using viam::sdk::Servo;

class MockServo : public Servo {
   public:
    void move(int32_t angle_deg, const viam::sdk::ProtoStruct& extra) override;
    Servo::position get_position(const sdk::ProtoStruct& extra) override;
    void stop(const viam::sdk::ProtoStruct& extra) override;
    bool is_moving() override;
    viam::sdk::ProtoStruct do_command(const viam::sdk::ProtoStruct& command) override;
    static std::shared_ptr<MockServo> get_mock_servo();
    virtual std::vector<sdk::GeometryConfig> get_geometries(const sdk::ProtoStruct& extra) override;

    MockServo(std::string name) : Servo(std::move(name)) {};

    using Servo::get_geometries;
    using Servo::get_position;
    using Servo::move;
    using Servo::stop;

   private:
    Servo::position position_;
    viam::sdk::ProtoStruct map_;
    bool is_moving_;
};

Servo::position fake_position();

}  // namespace servo
}  // namespace sdktests
}  // namespace viam
