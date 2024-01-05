#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/servo/v1/servo.grpc.pb.h>
#include <viam/api/component/servo/v1/servo.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/servo/client.hpp>
#include <viam/sdk/components/servo/servo.hpp>
#include <viam/sdk/components/servo/server.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdktests {
namespace servo {

using viam::sdk::Servo;

class MockServo : public Servo {
   public:
    void move(uint32_t angle_deg, const viam::sdk::AttributeMap& extra) override;
    Servo::position get_position(const sdk::AttributeMap& extra) override;
    void stop(const viam::sdk::AttributeMap& extra) override;
    bool is_moving() override;
    viam::sdk::AttributeMap do_command(const viam::sdk::AttributeMap& command) override;
    static std::shared_ptr<MockServo> get_mock_servo();
    virtual std::vector<sdk::GeometryConfig> get_geometries(
        const sdk::AttributeMap& extra) override;

    MockServo(std::string name) : Servo(std::move(name)){};

    // the `extra` param is frequently unnecessary but needs to be supported. Ideally, we'd
    // like to live in a world where implementers of derived classes don't need to go out of
    // their way to support two versions of a method (an `extra` version and a non-`extra`
    // version), and users don't need to pass an unnecessary parameters to all method calls.
    //
    // To do this, we define in the parent resource class a non-virtual version of the methods
    // that calls the virtual method and passes a `nullptr` by default in place of the `extra`
    // param. In order to access these versions of the methods within the client code, however,
    // we need to include these `using` lines.
    using Servo::get_geometries;
    using Servo::get_position;
    using Servo::stop;

   private:
    Servo::position position_;
    // TODO(RSDK-2747) swap to AttributeMap
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<viam::sdk::ProtoType>>> map_;
};

Servo::position fake_position();

}  // namespace servo
}  // namespace sdktests
}  // namespace viam
