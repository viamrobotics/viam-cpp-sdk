/// @file components/servo/client.hpp
///
/// @brief Implements a gRPC client for the `Servo` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/servo/v1/servo.grpc.pb.h>

#include <viam/sdk/components/servo/server.hpp>
#include <viam/sdk/components/servo/servo.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

/// @class ServoClient
/// @brief gRPC client implementation of a `Servo` component.
/// @ingroup Servo
class ServoClient : public Servo {
   public:
    ServoClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    void move(uint32_t angle_deg, const AttributeMap& extra) override;
    position get_position(const AttributeMap& extra) override;
    void stop(const AttributeMap& extra) override;
    bool is_moving() override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;
    AttributeMap do_command(const AttributeMap& command) override;

    using Servo::get_geometries;
    using Servo::get_position;
    using Servo::move;
    using Servo::stop;

    using StubType = viam::component::servo::v1::ServoService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};
}  // namespace sdk
}  // namespace viam