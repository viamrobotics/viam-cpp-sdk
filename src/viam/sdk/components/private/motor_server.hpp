/// @file components/private/motor_server.hpp
///
/// @brief Implements a gRPC server for the `Motor` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/motor/v1/motor.grpc.pb.h>

#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class MotorServer
/// @brief gRPC server implementation of a `Motor` component.
/// @ingroup Motor
class MotorServer : public ResourceServer,
                    public viam::component::motor::v1::MotorService::Service {
   public:
    using interface_type = Motor;
    using service_type = component::motor::v1::MotorService;
    explicit MotorServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status SetPower(
        ::grpc::ServerContext* context,
        const ::viam::component::motor::v1::SetPowerRequest* request,
        ::viam::component::motor::v1::SetPowerResponse* response) noexcept override;

    ::grpc::Status GoFor(::grpc::ServerContext* context,
                         const ::viam::component::motor::v1::GoForRequest* request,
                         ::viam::component::motor::v1::GoForResponse* response) noexcept override;

    ::grpc::Status GoTo(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::GoToRequest* request,
                        ::viam::component::motor::v1::GoToResponse* response) noexcept override;

    ::grpc::Status SetRPM(::grpc::ServerContext* context,
                          const ::viam::component::motor::v1::SetRPMRequest* request,
                          ::viam::component::motor::v1::SetRPMResponse* response) noexcept override;

    ::grpc::Status ResetZeroPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::motor::v1::ResetZeroPositionRequest* request,
        ::viam::component::motor::v1::ResetZeroPositionResponse* response) noexcept override;

    ::grpc::Status GetPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::motor::v1::GetPositionRequest* request,
        ::viam::component::motor::v1::GetPositionResponse* response) noexcept override;

    ::grpc::Status GetProperties(
        ::grpc::ServerContext* context,
        const ::viam::component::motor::v1::GetPropertiesRequest* request,
        ::viam::component::motor::v1::GetPropertiesResponse* response) noexcept override;

    ::grpc::Status Stop(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::StopRequest* request,
                        ::viam::component::motor::v1::StopResponse* response) noexcept override;

    ::grpc::Status IsPowered(
        ::grpc::ServerContext* context,
        const ::viam::component::motor::v1::IsPoweredRequest* request,
        ::viam::component::motor::v1::IsPoweredResponse* response) noexcept override;

    ::grpc::Status IsMoving(
        ::grpc::ServerContext* context,
        const ::viam::component::motor::v1::IsMovingRequest* request,
        ::viam::component::motor::v1::IsMovingResponse* response) noexcept override;

    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;

    ::grpc::Status DoCommand(grpc::ServerContext* context,
                             const viam::common::v1::DoCommandRequest* request,
                             viam::common::v1::DoCommandResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
