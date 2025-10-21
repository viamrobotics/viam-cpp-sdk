#pragma once

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/audio_in.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdktests {
namespace audioin {

using viam::sdk::AudioIn;
using namespace viam::sdk;

class MockAudioIn : public AudioIn {
   public:
    void get_audio(std::string const& codec,
                   std::function<bool(audio_chunk&& chunk)> const& chunk_handler,
                   double const& duration_seconds,
                   int64_t const& previous_timestamp,
                   const sdk::ProtoStruct& extra) override;

    AudioIn::properties get_properties(const sdk::ProtoStruct& extra) override;

    viam::sdk::ProtoStruct do_command(const viam::sdk::ProtoStruct& command) override;

    std::vector<GeometryConfig> get_geometries(const sdk::ProtoStruct& extra) override;

    static std::shared_ptr<MockAudioIn> get_mock_audio_in();

    MockAudioIn(std::string name) : AudioIn(std::move(name)) {}

    using AudioIn::get_audio;
    using AudioIn::get_properties;

   private:
    AudioIn::properties properties_;
    viam::sdk::ProtoStruct map_;
    std::vector<audio_chunk> mock_chunks_;
    std::vector<GeometryConfig> geometries_;
};

AudioIn::properties fake_properties();
std::vector<AudioIn::audio_chunk> fake_audio_chunks();

}  // namespace audioin
}  // namespace sdktests
}  // namespace viam
