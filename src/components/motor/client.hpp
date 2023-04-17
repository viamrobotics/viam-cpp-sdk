/// @file components/motor/client.hpp
///
/// @brief Implements a gRPC client for the `Motor` component.
#pragma once

#include <grpcpp/channel.h>

#include <component/motor/v1/motor.grpc.pb.h>

#include <components/motor/motor.hpp>
#include <components/motor/server.hpp>
#include <config/resource.hpp>
#include <robot/client.hpp>

namespace viam {
namespace cppsdk {

/// @class MotorClient
/// @brief gRPC client implementation of a `Motor` component.
/// @ingroup Motor
class MotorClient : public Motor {
   public:
    void set_power(double power_pct) override;
    void go_for(double rpm, double revolutions) override;
    void go_to(double rpm, double position_revolutions) override;
    void reset_zero_position(double offset) override;
    position get_position() override;
    properties get_properties() override;
    void stop_motor() override;
    power_status get_power_status() override;
    moving_status is_moving() override;
    AttributeMap do_command(AttributeMap command) override;
    MotorClient(std::string name, std::shared_ptr<grpc::Channel> channel)
        : Motor(std::move(name)),
          stub_(viam::component::motor::v1::MotorService::NewStub(channel)),
          channel_(std::move(channel)){};

   protected:
    // This constructor leaves the `channel_` as a nullptr. This is useful for testing
    // purposes, but renders it unusable for production use. Care should be taken to
    // avoid use of this constructor outside of tests.
    MotorClient(std::string name,
                std::unique_ptr<viam::component::motor::v1::MotorService::StubInterface> stub)
        : Motor(std::move(name)), stub_(std::move(stub)){};

   private:
    std::unique_ptr<viam::component::motor::v1::MotorService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace cppsdk
}  // namespace viam
