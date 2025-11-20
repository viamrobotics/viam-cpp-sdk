/// @file components/private/arm_server.hpp
///
/// @brief Implements a gRPC server for the `Arm` component
#pragma once

#include <viam/api/component/arm/v1/arm.grpc.pb.h>
#include <viam/api/component/arm/v1/arm.pb.h>

#include <viam/sdk/components/arm.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class ArmServer
/// @brief gRPC server implementation of an `Arm` component.
/// @ingroup Arm
class ArmServer : public ResourceServer, public viam::component::arm::v1::ArmService::Service {
   public:
    using interface_type = Arm;
    using service_type = component::arm::v1::ArmService;

    explicit ArmServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetEndPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::arm::v1::GetEndPositionRequest* request,
        ::viam::component::arm::v1::GetEndPositionResponse* response) noexcept override;

    ::grpc::Status MoveToPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::arm::v1::MoveToPositionRequest* request,
        ::viam::component::arm::v1::MoveToPositionResponse* response) noexcept override;

    ::grpc::Status GetJointPositions(
        ::grpc::ServerContext* context,
        const ::viam::component::arm::v1::GetJointPositionsRequest* request,
        ::viam::component::arm::v1::GetJointPositionsResponse* response) noexcept override;

    ::grpc::Status MoveToJointPositions(
        ::grpc::ServerContext* context,
        const ::viam::component::arm::v1::MoveToJointPositionsRequest* request,
        ::viam::component::arm::v1::MoveToJointPositionsResponse* response) noexcept override;

    ::grpc::Status MoveThroughJointPositions(
        ::grpc::ServerContext* context,
        const ::viam::component::arm::v1::MoveThroughJointPositionsRequest* request,
        ::viam::component::arm::v1::MoveThroughJointPositionsResponse* response) noexcept override;

    ::grpc::Status Stop(::grpc::ServerContext* context,
                        const ::viam::component::arm::v1::StopRequest* request,
                        ::viam::component::arm::v1::StopResponse* response) noexcept override;

    ::grpc::Status IsMoving(
        ::grpc::ServerContext* context,
        const ::viam::component::arm::v1::IsMovingRequest* request,
        ::viam::component::arm::v1::IsMovingResponse* response) noexcept override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;

    ::grpc::Status GetKinematics(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetKinematicsRequest* request,
        ::viam::common::v1::GetKinematicsResponse* response) noexcept override;

    ::grpc::Status Get3DModels(::grpc::ServerContext* context,
                               const ::viam::common::v1::Get3DModelsRequest* request,
                               ::viam::common::v1::Get3DModelsResponse* response) noexcept override;

    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
