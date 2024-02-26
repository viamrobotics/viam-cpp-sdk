/// @file components/private/encoder_server.hpp
///
/// @brief Implements a gRPC server for the `Encoder` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>

#include <viam/sdk/components/encoder.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class EncoderServer
/// @brief gRPC server implementation of a `Encoder` component.
/// @ingroup Encoder
class EncoderServer : public ResourceServer,
                      public viam::component::encoder::v1::EncoderService::Service {
   public:
    using interface_type = Encoder;
    using service_type = component::encoder::v1::EncoderService;
    explicit EncoderServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::encoder::v1::GetPositionRequest* request,
        ::viam::component::encoder::v1::GetPositionResponse* response) noexcept override;

    ::grpc::Status ResetPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::encoder::v1::ResetPositionRequest* request,
        ::viam::component::encoder::v1::ResetPositionResponse* response) noexcept override;

    ::grpc::Status GetProperties(
        ::grpc::ServerContext* context,
        const ::viam::component::encoder::v1::GetPropertiesRequest* request,
        ::viam::component::encoder::v1::GetPropertiesResponse* response) noexcept override;

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
