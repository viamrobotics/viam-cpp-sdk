/// @file components/private/gripper_server.hpp
///
/// @brief Implements a gRPC server for the `Gripper` component
#pragma once

#include <viam/api/component/gripper/v1/gripper.grpc.pb.h>
#include <viam/api/component/gripper/v1/gripper.pb.h>

#include <viam/sdk/components/gripper.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

class GripperServer : public ResourceServer,
                      public viam::component::gripper::v1::GripperService::Service {
   public:
    using interface_type = Gripper;
    using service_type = component::gripper::v1::GripperService;

    explicit GripperServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status Open(::grpc::ServerContext* context,
                        const ::viam::component::gripper::v1::OpenRequest* request,
                        ::viam::component::gripper::v1::OpenResponse* response) noexcept override;

    ::grpc::Status Grab(::grpc::ServerContext* context,
                        const ::viam::component::gripper::v1::GrabRequest* request,
                        ::viam::component::gripper::v1::GrabResponse* response) noexcept override;

    ::grpc::Status Stop(::grpc::ServerContext* context,
                        const ::viam::component::gripper::v1::StopRequest* request,
                        ::viam::component::gripper::v1::StopResponse* response) noexcept override;

    ::grpc::Status IsHoldingSomething(
        ::grpc::ServerContext* context,
        const ::viam::component::gripper::v1::IsHoldingSomethingRequest* request,
        ::viam::component::gripper::v1::IsHoldingSomethingResponse* response) noexcept override;

    ::grpc::Status IsMoving(
        ::grpc::ServerContext* context,
        const ::viam::component::gripper::v1::IsMovingRequest* request,
        ::viam::component::gripper::v1::IsMovingResponse* response) noexcept override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;

    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
