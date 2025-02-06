#include <viam/sdk/components/private/pose_tracker_server.hpp>

#include <viam/api/component/posetracker/v1/pose_tracker.pb.h>

#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/pose_tracker.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {
namespace impl {

PoseTrackerServer::PoseTrackerServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status PoseTrackerServer::GetPoses(
    ::grpc::ServerContext*,
    const ::viam::component::posetracker::v1::GetPosesRequest* request,
    ::viam::component::posetracker::v1::GetPosesResponse* response) noexcept {
    return make_service_helper<PoseTracker>(
        "PoseTrackerServer::GetPoses", this, request)([&](auto& helper, auto& pose_tracker) {
        const PoseTracker::pose_map result = pose_tracker->get_poses(
            {request->body_names().begin(), request->body_names().end()}, helper.getExtra());

        for (const auto& pair : result) {
            response->mutable_body_poses()->insert({pair.first, to_proto(pair.second)});
        }
    });
}

::grpc::Status PoseTrackerServer::DoCommand(
    grpc::ServerContext*,
    const viam::common::v1::DoCommandRequest* request,
    viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<PoseTracker>(
        "PoseTrackerServer::DoCommand", this, request)([&](auto&, auto& pose_tracker) {
        const ProtoStruct result = pose_tracker->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status PoseTrackerServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<PoseTracker>(
        "PoseTrackerServer::GetGeometries", this, request)([&](auto& helper, auto& pose_tracker) {
        const std::vector<GeometryConfig> geometries =
            pose_tracker->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = to_proto(geometry);
        }
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
