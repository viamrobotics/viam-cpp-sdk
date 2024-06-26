#include <viam/sdk/components/private/pose_tracker_client.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/posetracker/v1/pose_tracker.grpc.pb.h>
#include <viam/api/component/posetracker/v1/pose_tracker.pb.h>

#include <viam/sdk/common/client_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

PoseTrackerClient::PoseTrackerClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : PoseTracker(std::move(name)),
      stub_(viam::component::posetracker::v1::PoseTrackerService::NewStub(channel)),
      channel_(std::move(channel)) {}

PoseTracker::pose_map PoseTrackerClient::get_poses(const std::vector<std::string>& body_names,
                                                   const AttributeMap&) {
    return make_client_helper(this, *stub_, &StubType::GetPoses)
        .with([&](viam::component::posetracker::v1::GetPosesRequest& request) {
            *request.mutable_body_names() = {body_names.begin(), body_names.end()};
        })
        .invoke([](const viam::component::posetracker::v1::GetPosesResponse& response) {
            PoseTracker::pose_map result;

            for (const auto& pair : response.body_poses()) {
                result.emplace(pair.first, pose_in_frame::from_proto(pair.second));
            }

            return result;
        });
}

std::vector<GeometryConfig> PoseTrackerClient::get_geometries(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return GeometryConfig::from_proto(response); });
}

AttributeMap PoseTrackerClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
