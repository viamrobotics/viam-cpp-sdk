/// @file components/private/base_server.hpp
///
/// @brief Implements a gRPC server for the `Base` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/base/v1/base.grpc.pb.h>
#include <viam/api/component/base/v1/base.pb.h>

#include <viam/sdk/components/base.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {

/// @class BaseServer
/// @brief gRPC server implementation of a `Base` component.
/// @ingroup Base
class BaseServer : public ResourceServer, public viam::component::base::v1::BaseService::Service {
   public:
    using interface_type = Base;
    using service_type = component::base::v1::BaseService;
    explicit BaseServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status MoveStraight(
        ::grpc::ServerContext* context,
        const ::viam::component::base::v1::MoveStraightRequest* request,
        ::viam::component::base::v1::MoveStraightResponse* response) noexcept override;

    ::grpc::Status Spin(::grpc::ServerContext* context,
                        const ::viam::component::base::v1::SpinRequest* request,
                        ::viam::component::base::v1::SpinResponse* response) noexcept override;

    ::grpc::Status SetPower(
        ::grpc::ServerContext* context,
        const ::viam::component::base::v1::SetPowerRequest* request,
        ::viam::component::base::v1::SetPowerResponse* response) noexcept override;

    ::grpc::Status SetVelocity(
        ::grpc::ServerContext* context,
        const ::viam::component::base::v1::SetVelocityRequest* request,
        ::viam::component::base::v1::SetVelocityResponse* response) noexcept override;

    ::grpc::Status Stop(::grpc::ServerContext* context,
                        const ::viam::component::base::v1::StopRequest* request,
                        ::viam::component::base::v1::StopResponse* response) noexcept override;

    ::grpc::Status IsMoving(
        ::grpc::ServerContext* context,
        const ::viam::component::base::v1::IsMovingRequest* request,
        ::viam::component::base::v1::IsMovingResponse* response) noexcept override;

    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;

    ::grpc::Status GetProperties(
        ::grpc::ServerContext* context,
        const ::viam::component::base::v1::GetPropertiesRequest* request,
        ::viam::component::base::v1::GetPropertiesResponse* response) noexcept override;

    ::grpc::Status DoCommand(grpc::ServerContext* context,
                             const viam::common::v1::DoCommandRequest* request,
                             viam::common::v1::DoCommandResponse* response) noexcept override;
};

}  // namespace sdk
}  // namespace viam
