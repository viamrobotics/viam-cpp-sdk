/// @file components/private/audio_out_server.hpp
///
/// @brief Implements a gRPC server for the `AudioOut` component
#pragma once

#include <viam/api/component/audioout/v1/audioout.grpc.pb.h>
#include <viam/api/component/audioout/v1/audioout.pb.h>

#include <viam/sdk/components/audio_out.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class AudioOutServer
/// @brief gRPC server implementation of an `AudioOut` component.
/// @ingroup AudioOut
class AudioOutServer : public ResourceServer,
                       public viam::component::audioout::v1::AudioOutService::Service {
   public:
    using interface_type = AudioOut;
    using service_type = viam::component::audioout::v1::AudioOutService;

    explicit AudioOutServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status Play(::grpc::ServerContext* context,
                        const ::viam::component::audioout::v1::PlayRequest* request,
                        ::viam::component::audioout::v1::PlayResponse* response) noexcept override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                              const ::viam::common::v1::DoCommandRequest* request,
                              ::viam::common::v1::DoCommandResponse* response) noexcept override;

    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;

    ::grpc::Status GetProperties(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetPropertiesRequest* request,
        ::viam::common::v1::GetPropertiesResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
