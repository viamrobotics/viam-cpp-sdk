#include <viam/sdk/services/private/framesystem_client.hpp>

#include <grpcpp/channel.h>

#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/framesystem.hpp>

namespace viam {
namespace sdk {
namespace impl {

FrameSystemClient::FrameSystemClient(std::string name, const ViamChannel& channel)
    : FrameSystem(std::move(name)),
      stub_(viam::robot::v1::RobotService::NewStub(channel.channel())),
      channel_(&channel) {}

std::vector<FrameSystem::frame_system_config> FrameSystemClient::get_frame_system_config(
    const std::vector<WorldState::transform>& additional_transforms) {
    return make_client_helper(this, *stub_, &StubType::FrameSystemConfig)
        .with([&](auto& request) {
            *request.mutable_supplemental_transforms() = to_repeated_field(additional_transforms);
        })
        .invoke(
            [](auto& response) { return from_repeated_field(response.frame_system_configs()); });
}

pose_in_frame FrameSystemClient::get_pose(
    const std::string& component_name,
    const std::string& destination_frame,
    const std::vector<WorldState::transform>& supplemental_transforms,
    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPose)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_component_name() = component_name;
                  *request.mutable_destination_frame() = destination_frame;
                  *request.mutable_supplemental_transforms() =
                      to_repeated_field(supplemental_transforms);
              })
        .invoke([](auto& response) { return from_proto(response.pose()); });
}

pose_in_frame FrameSystemClient::transform_pose(
    const pose_in_frame& query,
    const std::string& destination,
    const std::vector<WorldState::transform>& additional_transforms) {
    return make_client_helper(this, *stub_, &StubType::TransformPose)
        .with([&](auto& request) {
            *request.mutable_source() = to_proto(query);
            *request.mutable_destination() = destination;
            *request.mutable_supplemental_transforms() = to_repeated_field(additional_transforms);
        })
        .invoke([](auto& response) { return from_proto(response.pose()); });
}

std::vector<unsigned char> FrameSystemClient::transform_pcd(const std::vector<unsigned char>& pcd,
                                                            const std::string& source,
                                                            const std::string& destination) {
    return make_client_helper(this, *stub_, &StubType::TransformPCD)
        .with([&](auto& request) {
            *request.mutable_point_cloud_pcd() = bytes_to_string(pcd);
            *request.mutable_source() = source;
            *request.mutable_destination() = destination;
        })
        .invoke([](auto& response) { return string_to_bytes(response.point_cloud_pcd()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
