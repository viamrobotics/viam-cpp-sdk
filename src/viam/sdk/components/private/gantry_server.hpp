/// @file components/private/gantry_server.hpp
///
/// @brief Implements a gRPC server for the `Gantry` component
#pragma once

#include <viam/api/component/gantry/v1/gantry.grpc.pb.h>
#include <viam/api/component/gantry/v1/gantry.pb.h>

#include <viam/sdk/components/gantry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class GantryServer
/// @brief gRPC server implementation of a `Gantry` component.
/// @ingroup Gantry
class GantryServer : public ResourceServer,
                     public viam::component::gantry::v1::GantryService::Service {
   public:
    using interface_type = Gantry;
    using service_type = component::gantry::v1::GantryService;

    explicit GantryServer(std::shared_ptr<ResourceManager> manager);

    virtual ::grpc::Status GetPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::gantry::v1::GetPositionRequest* request,
        ::viam::component::gantry::v1::GetPositionResponse* response) noexcept;

    virtual ::grpc::Status MoveToPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::gantry::v1::MoveToPositionRequest* request,
        ::viam::component::gantry::v1::MoveToPositionResponse* response) noexcept;

    virtual ::grpc::Status Home(::grpc::ServerContext* context,
                                const ::viam::component::gantry::v1::HomeRequest* request,
                                ::viam::component::gantry::v1::HomeResponse* response) noexcept;

    virtual ::grpc::Status GetLengths(
        ::grpc::ServerContext* context,
        const ::viam::component::gantry::v1::GetLengthsRequest* request,
        ::viam::component::gantry::v1::GetLengthsResponse* response) noexcept;

    virtual ::grpc::Status Stop(::grpc::ServerContext* context,
                                const ::viam::component::gantry::v1::StopRequest* request,
                                ::viam::component::gantry::v1::StopResponse* response) noexcept;

    virtual ::grpc::Status IsMoving(
        ::grpc::ServerContext* context,
        const ::viam::component::gantry::v1::IsMovingRequest* request,
        ::viam::component::gantry::v1::IsMovingResponse* response) noexcept;

    virtual ::grpc::Status DoCommand(::grpc::ServerContext* context,
                                     const ::viam::common::v1::DoCommandRequest* request,
                                     ::viam::common::v1::DoCommandResponse* response) noexcept;

    virtual ::grpc::Status GetKinematics(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetKinematicsRequest* request,
        ::viam::common::v1::GetKinematicsResponse* response) noexcept;

    virtual ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
