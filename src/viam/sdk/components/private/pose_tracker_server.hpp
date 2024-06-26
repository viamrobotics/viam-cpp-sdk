/// @file components/private/pose_tracker_server.hpp
///
/// @brief Implements a gRPC server for the `PoseTracker` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/posetracker/v1/pose_tracker.grpc.pb.h>
#include <viam/api/component/posetracker/v1/pose_tracker.pb.h>

#include <viam/sdk/components/pose_tracker.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class PoseTrackerServer
/// @brief gRPC server implementation of a `PoseTracker` component.
/// @ingroup PoseTracker
class PoseTrackerServer : public ResourceServer,
                          public viam::component::posetracker::v1::PoseTrackerService::Service {
   public:
    using interface_type = PoseTracker;
    using service_type = component::posetracker::v1::PoseTrackerService;
    explicit PoseTrackerServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetPoses(
        ::grpc::ServerContext* context,
        const ::viam::component::posetracker::v1::GetPosesRequest* request,
        ::viam::component::posetracker::v1::GetPosesResponse* response) noexcept override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;

    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;
};
}  // namespace impl
}  // namespace sdk
}  // namespace viam
