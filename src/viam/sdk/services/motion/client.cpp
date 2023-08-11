#include "service/motion/v1/motion.pb.h"
#include "viam/sdk/common/proto_type.hpp"
#include <grpcpp/support/status.h>
#include <viam/sdk/services/motion/client.hpp>

#include "service/motion/v1/motion.grpc.pb.h"
#include <viam/sdk/services/motion/motion.hpp>

namespace viam {
namespace sdk {

MotionClient::MotionClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Motion(std::move(name)),
      stub_(service::motion::v1::MotionService::NewStub(channel)),
      channel_(std::move(channel)){};

bool MotionClient::move(const PoseInFrame& destination,
                        const Name& component_name,
                        std::unique_ptr<WorldState> ws,
                        std::unique_ptr<Motion::constraints> constraints,
                        const AttributeMap& extra) {
    service::motion::v1::MoveRequest request;
    service::motion::v1::MoveResponse response;
    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_constraints() = constraints->to_proto();
    *request.mutable_component_name() = component_name.to_proto();
    *request.mutable_destination() = destination.to_proto();
    *request.mutable_extra() = map_to_struct(extra);
    *request.mutable_world_state() = ws->to_proto();

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

PoseInFrame MotionClient::get_pose(const Name& component_name,
                                   const std::string& destination_frame,
                                   std::vector<WorldState::transform> supplemental_transforms,
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

    return PoseInFrame::from_proto(response.pose());
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
