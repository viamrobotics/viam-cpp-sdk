#include <viam/sdk/tests/mocks/mock_audio_out.hpp>

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/audio_out.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace audioout {

using namespace viam::sdk;

void MockAudioOut::play(std::vector<uint8_t> const& audio_data,
                        std::shared_ptr<audio_info> info,
                        const ProtoStruct& extra) {
    last_played_audio_ = audio_data;
    last_played_audio_info_ = info;
}

audio_properties MockAudioOut::get_properties(const ProtoStruct& extra) {
    return properties_;
}

ProtoStruct MockAudioOut::do_command(const ProtoStruct& command) {
    return map_;
}

std::vector<GeometryConfig> MockAudioOut::get_geometries(const ProtoStruct&) {
    return geometries_;
}

std::shared_ptr<MockAudioOut> MockAudioOut::get_mock_audio_out() {
    auto audio_out = std::make_shared<MockAudioOut>("mock_audio_out");

    audio_out->properties_ = fake_properties();
    audio_out->map_ = fake_map();
    audio_out->geometries_ = fake_geometries();

    return audio_out;
}

audio_properties fake_properties() {
    audio_properties props;
    props.supported_codecs = {"pcm16", "pcm32"};
    props.sample_rate_hz = 48000;
    props.num_channels = 2;
    return props;
}

}  // namespace audioout
}  // namespace sdktests
}  // namespace viam
