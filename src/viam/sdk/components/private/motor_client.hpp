/// @file components/private/motor_client.hpp
///
/// @brief Implements a gRPC client for the `Motor` component.
#pragma once

#include <viam/api/component/motor/v1/motor.grpc.pb.h>

#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/components/private/motor_server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class MotorClient
/// @brief gRPC client implementation of a `Motor` component.
/// @ingroup Motor
class MotorClient : public Motor {
   public:
    using interface_type = Motor;
    MotorClient(std::string name, ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }
    void set_power(double power_pct, const ProtoStruct& extra) override;
    void go_for(double rpm, double revolutions, const ProtoStruct& extra) override;
    void go_to(double rpm, double position_revolutions, const ProtoStruct& extra) override;
    void set_rpm(double rpm, const ProtoStruct& extra) override;
    void reset_zero_position(double offset, const ProtoStruct& extra) override;
    position get_position(const ProtoStruct& extra) override;
    properties get_properties(const ProtoStruct& extra) override;
    void stop(const ProtoStruct& extra) override;
    power_status get_power_status(const ProtoStruct& extra) override;
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;
    bool is_moving() override;
    ProtoStruct do_command(const ProtoStruct& command) override;

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
    using Motor::set_rpm;
    using Motor::stop;

   private:
    using StubType = viam::component::motor::v1::MotorService::StubInterface;
    std::unique_ptr<StubType> stub_;
    ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
