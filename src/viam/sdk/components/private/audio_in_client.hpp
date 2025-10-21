/// @file components/private/audio_in_client.hpp
///
/// @brief Implements a gRPC client for the `AudioIn` component
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/audioin/v1/audioin.grpc.pb.h>

#include <viam/sdk/components/audio_in.hpp>
namespace viam {
namespace sdk {
namespace impl {

/// @class AudioInClient
/// @brief gRPC client implementation of a `AudioIn` component.
/// @ingroup AudioIn
class AudioInClient : public AudioIn {
   public:
    using interface_type = AudioIn;
    AudioInClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    void get_audio(std::string const& codec,
                    std::function<bool(audio_chunk&& chunk)> const& chunk_handler,
                    double const& duration_seconds,
                    int64_t const& previous_timestamp,
                    const ProtoStruct& extra) override;

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
    using AudioIn::get_geometries;
    using AudioIn::get_audio;
    using AudioIn::get_properties;

   private:

    using StubType = viam::component::audioin::v1::AudioInService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
