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
    void set_power(double power_pct) override;
    void go_for(double rpm, double revolutions) override;
    void go_to(double rpm, double position_revolutions) override;
    void reset_zero_position(double offset) override;
    position get_position() override;
    properties get_properties() override;
    grpc::StatusCode stop(AttributeMap extra) override;
    power_status get_power_status() override;
    bool is_moving() override;
    AttributeMap do_command(AttributeMap command) override;

   private:
    std::unique_ptr<viam::component::motor::v1::MotorService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
