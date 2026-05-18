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

// MockAudioOutStreamWriter
MockAudioOutStreamWriter::MockAudioOutStreamWriter(std::weak_ptr<MockAudioOut> parent)
    : parent_(std::move(parent)) {}

void MockAudioOutStreamWriter::write(std::vector<uint8_t> const& audio_data) {
    if (auto parent = parent_.lock()) {
        parent->streamed_audio_chunks_.push_back(audio_data);
    }
}

void MockAudioOutStreamWriter::close() {}

// MockAudioOutStreamReader
MockAudioOutStreamReader::MockAudioOutStreamReader(std::weak_ptr<MockAudioOut> parent)
    : parent_(std::move(parent)), current_chunk_index_(0) {}

boost::optional<std::vector<uint8_t>> MockAudioOutStreamReader::read() {
    if (auto parent = parent_.lock()) {
        if (current_chunk_index_ < parent->streamed_audio_chunks_.size()) {
            return boost::make_optional(parent->streamed_audio_chunks_[current_chunk_index_++]);
        }
    }
    return boost::none;
}

// MockAudioOut
void MockAudioOut::play(std::vector<uint8_t> const& audio_data,
                        boost::optional<audio_info> info,
                        const ProtoStruct& extra) {
    last_played_audio_ = audio_data;
    last_played_audio_info_ = info;
}

std::unique_ptr<AudioOutStreamWriter> MockAudioOut::play_stream(
    audio_info info, const sdk::ProtoStruct& extra) {
    streamed_audio_chunks_.clear();
    streamed_audio_info_ = info;
    streamed_extra_ = extra;
    return std::make_unique<MockAudioOutStreamWriter>(shared_from_this());
}

void MockAudioOut::play_stream(std::unique_ptr<AudioOutStreamReader> reader,
                               audio_info info,
                               const sdk::ProtoStruct& extra) {
    streamed_audio_chunks_.clear();
    streamed_audio_info_ = info;
    streamed_extra_ = extra;
    while (true) {
        auto chunk = reader->read();
        if (!chunk) {
            break;
        }
        streamed_audio_chunks_.push_back(*chunk);
    }
}


audio_properties MockAudioOut::get_properties(const ProtoStruct& extra) {
    return properties_;
}

ProtoStruct MockAudioOut::do_command(const ProtoStruct& command) {
    return map_;
}

sdk::ProtoStruct MockAudioOut::get_status() {
    return fake_status();
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