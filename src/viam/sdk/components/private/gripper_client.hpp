/// @file components/private/gripper_client.hpp
///
/// @brief Implements a gRPC client for the `Gripper` component
#pragma once

#include <memory>

#include <grpcpp/channel.h>

#include <viam/api/component/gripper/v1/gripper.grpc.pb.h>

#include <viam/sdk/components/gripper.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class GripperClient
/// @brief gRPC client implementation of a `Gripper` component
/// @ingroup Gripper
class GripperClient : public Gripper {
   public:
    using interface_type = Gripper;
    GripperClient(std::string names, std::shared_ptr<grpc::Channel> channel);

    void open(const AttributeMap& extra) override;
    bool grab(const AttributeMap& extra) override;
    void stop(const AttributeMap& extra) override;
    bool is_moving() override;
    AttributeMap do_command(const AttributeMap& command) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

    using Gripper::get_geometries;
    using Gripper::grab;
    using Gripper::open;
    using Gripper::stop;

   private:
    using StubType = viam::component::gripper::v1::GripperService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
