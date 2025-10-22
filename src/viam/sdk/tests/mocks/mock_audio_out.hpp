#pragma once

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/audio_out.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdktests {
namespace audioout {

using viam::sdk::AudioOut;
using viam::sdk::properties;
using namespace viam::sdk;

class MockAudioOut : public AudioOut {
   public:
    void play(std::vector<uint8_t> const& audio_data,
               audio_info const* info,
              const sdk::ProtoStruct& extra) override;
    AudioOut::properties get_properties(const sdk::ProtoStruct& extra) override;
    viam::sdk::ProtoStruct do_command(const viam::sdk::ProtoStruct& command) override;
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;

    static std::shared_ptr<MockAudioOut> get_mock_audio_out();

    MockAudioOut(std::string name) : AudioOut(std::move(name)) {}

    using AudioOut::play;
    using AudioOut::get_properties;

    properties properties_;
    viam::sdk::ProtoStruct map_;
    std::vector<GeometryConfig> geometries_;
    std::vector<uint8_t> last_played_audio_;
    audio_info const* last_played_audio_info_;

};

properties fake_properties();

}  // namespace audioout
}  // namespace sdktests
}  // namespace viam
