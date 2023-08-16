#include <viam/sdk/services/motion/client.hpp>

#include <grpcpp/support/status.h>

#include <viam/api/service/motion/v1/motion.grpc.pb.h>
#include <viam/api/service/motion/v1/motion.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/services/motion/motion.hpp>

namespace viam {
namespace sdk {

MotionClient::MotionClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Motion(std::move(name)),
      stub_(service::motion::v1::MotionService::NewStub(channel)),
      channel_(std::move(channel)){};

bool MotionClient::move(const pose_in_frame& destination,
                        const Name& component_name,
                        std::shared_ptr<WorldState> world_state,
                        std::shared_ptr<Motion::constraints> constraints,
                        const AttributeMap& extra) {
    service::motion::v1::MoveRequest request;
    service::motion::v1::MoveResponse response;
    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_component_name() = component_name.to_proto();
    *request.mutable_destination() = destination.to_proto();
    *request.mutable_extra() = map_to_struct(extra);
    if (constraints) {
        *request.mutable_constraints() = constraints->to_proto();
    }
    if (world_state) {
        *request.mutable_world_state() = world_state->to_proto();
    }

    const grpc::Status status = stub_->Move(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return response.success();
}

bool MotionClient::move_on_map(const pose& destination,
                               const Name& component_name,
                               const Name& slam_name,
                               const AttributeMap& extra) {
    service::motion::v1::MoveOnMapRequest request;
    service::motion::v1::MoveOnMapResponse response;
    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_destination() = destination.to_proto();
    *request.mutable_component_name() = component_name.to_proto();
    *request.mutable_slam_service_name() = slam_name.to_proto();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->MoveOnMap(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return response.success();
}

bool MotionClient::move_on_globe(const geo_point& destination,
                                 const boost::optional<double>& heading,
                                 const Name& component_name,
                                 const Name& movement_sensor_name,
                                 const std::vector<geo_obstacle>& obstacles,
                                 std::shared_ptr<motion_configuration> motion_configuration,
                                 const AttributeMap& extra) {
    service::motion::v1::MoveOnGlobeRequest request;
    service::motion::v1::MoveOnGlobeResponse response;
    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_destination() = destination.to_proto();
    *request.mutable_component_name() = component_name.to_proto();
    *request.mutable_movement_sensor_name() = movement_sensor_name.to_proto();

    if (heading) {
        request.set_heading(*heading);
    }

    for (const auto& obstacle : obstacles) {
        *request.mutable_obstacles()->Add() = obstacle.to_proto();
    }

    if (motion_configuration) {
        *request.mutable_motion_configuration() = motion_configuration->to_proto();
    }

    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->MoveOnGlobe(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return response.success();
}

pose_in_frame MotionClient::get_pose(
    const Name& component_name,
    const std::string& destination_frame,
    const std::vector<WorldState::transform>& supplemental_transforms,
    AttributeMap extra) {
    service::motion::v1::GetPoseRequest request;
    service::motion::v1::GetPoseResponse response;
    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_component_name() = component_name.to_proto();
    *request.mutable_destination_frame() = std::move(destination_frame);
    for (const auto& transform : supplemental_transforms) {
        *request.mutable_supplemental_transforms()->Add() = transform.to_proto();
    }
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetPose(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return pose_in_frame::from_proto(response.pose());
}

AttributeMap MotionClient::do_command(const AttributeMap& command) {
    viam::common::v1::DoCommandRequest request;
    viam::common::v1::DoCommandResponse response;
    grpc::ClientContext ctx;

    *request.mutable_command() = map_to_struct(command);
    *request.mutable_name() = this->name();

    const grpc::Status status = stub_->DoCommand(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return struct_to_map(response.result());
}

}  // namespace sdk
}  // namespace viam
