/// @file components/private/gripper_client.hpp
///
/// @brief Implements a gRPC client for the `Gripper` component
#pragma once

#include <memory>

#include <viam/api/component/gripper/v1/gripper.grpc.pb.h>

#include <viam/sdk/components/gripper.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class GripperClient
/// @brief gRPC client implementation of a `Gripper` component
/// @ingroup Gripper
class GripperClient : public Gripper {
   public:
    using interface_type = Gripper;
    GripperClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    void open(const ProtoStruct& extra) override;

    bool grab(const ProtoStruct& extra) override;

    Gripper::holding_status is_holding_something(const ProtoStruct& extra) override;

    void stop(const ProtoStruct& extra) override;

    bool is_moving() override;

    ProtoStruct do_command(const ProtoStruct& command) override;

    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;

    using Gripper::get_geometries;
    using Gripper::grab;
    using Gripper::is_holding_something;
    using Gripper::open;
    using Gripper::stop;

   private:
    using StubType = viam::component::gripper::v1::GripperService::StubInterface;
    std::unique_ptr<StubType> stub_;
    const ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
