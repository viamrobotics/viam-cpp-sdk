/// @file components/motor/client.hpp
///
/// @brief Implements a gRPC client for the `Motor` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/motor/v1/motor.grpc.pb.h>

#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/components/motor/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

/// @class MotorClient
/// @brief gRPC client implementation of a `Motor` component.
/// @ingroup Motor
class MotorClient : public Motor {
   public:
    MotorClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    void set_power(double power_pct, const AttributeMap& extra) override;
    void go_for(double rpm, double revolutions, const AttributeMap& extra) override;
    void go_to(double rpm, double position_revolutions, const AttributeMap& extra) override;
    void reset_zero_position(double offset, const AttributeMap& extra) override;
    position get_position(const AttributeMap& extra) override;
    properties get_properties(const AttributeMap& extra) override;
    grpc::StatusCode stop(const AttributeMap& extra) override;
    power_status get_power_status(const AttributeMap& extra) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;
    bool is_moving() override;
    AttributeMap do_command(const AttributeMap& command) override;

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
    std::unique_ptr<viam::component::motor::v1::MotorService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
