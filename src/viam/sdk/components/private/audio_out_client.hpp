/// @file components/private/audio_out_client.hpp
///
/// @brief Implements a gRPC client for the `AudioOut` component
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/audioout/v1/audioout.grpc.pb.h>

#include <viam/sdk/components/audio_out.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class AudioOutClient
/// @brief gRPC client implementation of an `AudioOut` component.
/// @ingroup AudioOut
class AudioOutClient : public AudioOut {
   public:
    using interface_type = AudioOut;
    AudioOutClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    void play(std::vector<uint8_t> const& audio_data,
              boost::optional<audio_info> info,
              const ProtoStruct& extra) override;
    audio_properties get_properties(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;

    using AudioOut::get_geometries;
    using AudioOut::get_properties;
    using AudioOut::play;

   private:
    using StubType = viam::component::audioout::v1::AudioOutService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
