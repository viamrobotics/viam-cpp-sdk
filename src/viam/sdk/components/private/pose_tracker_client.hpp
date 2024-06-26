/// @file components/private/pose_tracker_client.hpp
///
/// @brief Implements a gRPC client for the `PoseTracker` component
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/posetracker/v1/pose_tracker.grpc.pb.h>

#include <viam/sdk/components/pose_tracker.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class PoseTrackerClient
/// @brief gRPC client implementation of a `PoseTracker` component.
/// @ingroup PoseTracker
class PoseTrackerClient : public PoseTracker {
   public:
    using interface_type = PoseTracker;

    PoseTrackerClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    PoseTracker::pose_map get_poses(const std::vector<std::string>& body_names,
                                    const AttributeMap& extra) override;

    AttributeMap do_command(const AttributeMap& command) override;

    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

    using PoseTracker::get_geometries;
    using PoseTracker::get_poses;

   private:
    using StubType = viam::component::posetracker::v1::PoseTrackerService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
