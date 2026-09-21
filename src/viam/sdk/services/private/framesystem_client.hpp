/// @file services/private/framesystem_client.hpp
///
/// @brief Implements a gRPC client for the `FrameSystem` service
#pragma once

#include <viam/api/robot/v1/robot.grpc.pb.h>

#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/framesystem.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class FrameSystemClient
/// @brief gRPC client implementation of a `FrameSystem` service.
/// @ingroup FrameSystem
class FrameSystemClient : public FrameSystem {
   public:
    using interface_type = FrameSystem;
    FrameSystemClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    std::vector<frame_system_config> get_frame_system_config(
        const std::vector<WorldState::transform>& additional_transforms) override;
    pose_in_frame get_pose(const std::string& component_name,
                           const std::string& destination_frame,
                           const std::vector<WorldState::transform>& supplemental_transforms,
                           const ProtoStruct& extra) override;
    pose_in_frame transform_pose(
        const pose_in_frame& query,
        const std::string& destination,
        const std::vector<WorldState::transform>& additional_transforms) override;
    std::vector<unsigned char> transform_pcd(const std::vector<unsigned char>& pcd,
                                             const std::string& source,
                                             const std::string& destination) override;

   private:
    // The frame system RPCs still live on RobotService. This alias is the one place that names it,
    // so when a dedicated FrameSystemService lands only this line and the constructor change.
    using StubType = viam::robot::v1::RobotService::StubInterface;
    std::unique_ptr<StubType> stub_;
    const ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
