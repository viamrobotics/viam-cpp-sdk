/// @file components/private/switch_server.hpp
///
/// @brief Implements a gRPC server for the `Switch` component
#pragma once

#include <viam/api/component/switch/v1/switch.grpc.pb.h>
#include <viam/api/component/switch/v1/switch.pb.h>

#include <viam/sdk/components/switch.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class SwitchServer
/// @brief gRPC server implementation of a `Switch` component.
/// @ingroup Switch
class SwitchServer : public ResourceServer,
                     public viam::component::switch_::v1::SwitchService::Service {
   public:
    using interface_type = Switch;
    using service_type = component::switch_::v1::SwitchService;

    explicit SwitchServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status SetPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::switch_::v1::SetPositionRequest* request,
        ::viam::component::switch_::v1::SetPositionResponse* response) noexcept override;

    ::grpc::Status GetPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::switch_::v1::GetPositionRequest* request,
        ::viam::component::switch_::v1::GetPositionResponse* response) noexcept override;

    ::grpc::Status GetNumberOfPositions(
        ::grpc::ServerContext* context,
        const ::viam::component::switch_::v1::GetNumberOfPositionsRequest* request,
        ::viam::component::switch_::v1::GetNumberOfPositionsResponse* response) noexcept override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
