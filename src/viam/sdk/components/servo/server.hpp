/// @file components/servo/server.hpp
///
/// @brief Implements a gRPC server for the `Servo` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/servo/v1/servo.grpc.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {

/// @class ServoServer
/// @brief gRPC server implementation of a `Servo` componnet.
/// @ingroup Servo
class ServoServer : public ResourceServer,
                    public viam::component::servo::v1::ServoService::Service {
   public:
    ServoServer();
    explicit ServoServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status Move(::grpc::ServerContext* context,
                        const ::viam::component::servo::v1::MoveRequest* request,
                        ::viam::component::servo::v1::MoveResponse* response) noexcept override;

    ::grpc::Status GetPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::servo::v1::GetPositionRequest* request,
        ::viam::component::servo::v1::GetPositionResponse* response) noexcept override;

    ::grpc::Status Stop(::grpc::ServerContext* context,
                        const ::viam::component::servo::v1::StopRequest* request,
                        ::viam::component::servo::v1::StopResponse* response) noexcept override;

    ::grpc::Status IsMoving(
        ::grpc::ServerContext* context,
        const ::viam::component::servo::v1::IsMovingRequest* request,
        ::viam::component::servo::v1::IsMovingResponse* response) noexcept override;

    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;

    ::grpc::Status DoCommand(grpc::ServerContext* context,
                             const viam::common::v1::DoCommandRequest* request,
                             viam::common::v1::DoCommandResponse* response) noexcept override;

    void register_server(std::shared_ptr<Server> server) override;
};
}  // namespace sdk
}  // namespace viam