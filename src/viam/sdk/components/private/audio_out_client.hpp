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

    void play(std::vector<uint8_t> const& audio_data, audio_info const* info, const ProtoStruct& extra) override;
    properties get_properties(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;


    // the `extra` param is frequently unnecessary but needs to be supported. Ideally, we'd
    // like to live in a world where implementers of derived classes don't need to go out of
    // their way to support two versions of a method (an `extra` version and a non-`extra`
    // version), and users don't need to pass an unnecessary parameters to all method calls.
    //
    // To do this, we define in the parent resource class a non-virtual version of the methods
    // that calls the virtual method and passes a `nullptr` by default in place of the `extra`
    // param. In order to access these versions of the methods within the client code, however,
    // we need to include these `using` lines.
    using AudioOut::play;
    using AudioOut::get_properties;
    using AudioOut::get_geometries;

   private:
    using StubType = viam::component::audioout::v1::AudioOutService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
