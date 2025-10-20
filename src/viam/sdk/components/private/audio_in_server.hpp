/// @file components/private/audio_in_server.hpp
///
/// @brief Implements a gRPC server for the `AudioIn` component
#pragma once

#include <viam/api/component/audioin/v1/audioin.grpc.pb.h>
#include <viam/api/component/audioin/v1/audioin.pb.h>

#include <viam/sdk/components/audio_in.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class AudioInServer
/// @brief gRPC server implementation of an `AudioIn` component.
/// @ingroup Arm

class AudioInServer : public ResourceServer, public viam::component::audioin::v1::AudioInService::Service {
   public:
    using interface_type = AudioIn;
    using service_type = component::audioin::v1::AudioInService;

    explicit AudioInServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetAudio(::grpc::ServerContext* context,
                            const ::viam::component::audioin::v1::GetAudioRequest*,
                            ::grpc::ServerWriter<::viam::component::audioin::v1::GetAudioResponse>* writer
                        ) noexcept override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                    const ::viam::common::v1::DoCommandRequest* request,
                    ::viam::common::v1::DoCommandResponse* response) noexcept override;


    ::grpc::Status GetProperties(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetPropertiesRequest* request,
        ::viam::common::v1::GetPropertiesResponse* response) noexcept override;
    };


} // namespace impl
} // namespace sdk
} // namespace viam
