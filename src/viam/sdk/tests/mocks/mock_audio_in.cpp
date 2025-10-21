#include <viam/sdk/tests/mocks/mock_audio_in.hpp>

#include <chrono>
#include <thread>

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/audio_in.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace audioin {

using namespace viam::sdk;

void MockAudioIn::get_audio(std::string const& codec,
                            std::function<bool(audio_chunk&& chunk)> const& chunk_handler,
                            double const& duration_seconds,
                            int64_t const& previous_timestamp,
                            const ProtoStruct& extra) {
    // Simulate streaming audio chunks
    int chunk_count = 0;
    int max_chunks = (duration_seconds == 0) ? 100 : static_cast<int>(duration_seconds * 100);

    for (const auto& mock_chunk : mock_chunks_) {
        if (chunk_count >= max_chunks) {
            break;
        }

        // Create a copy of the chunk to pass to handler
        audio_chunk chunk = mock_chunk;
        chunk.sequence_number = chunk_count;

        if (!chunk_handler(std::move(chunk))) {
            break;  // Handler requested to stop
        }

        chunk_count++;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

properties MockAudioIn::get_properties(const ProtoStruct& extra) {
    return properties_;
}

ProtoStruct MockAudioIn::do_command(const ProtoStruct& command) {
    return map_;
}

std::shared_ptr<MockAudioIn> MockAudioIn::get_mock_audio_in() {
    auto audio_in = std::make_shared<MockAudioIn>("mock_audio_in");

    audio_in->properties_ = fake_properties();
    audio_in->mock_chunks_ = fake_audio_chunks();
    audio_in->map_ = fake_map();
    audio_in->geometries_ = fake_geometries();

    return audio_in;
}

properties fake_properties() {
    properties props;
    props.supported_codecs = {audio_codecs::PCM_16, audio_codecs::PCM_32};
    props.sample_rate_hz = 48000;
    props.num_channels = 1;
    return props;
}

std::vector<AudioIn::audio_chunk> fake_audio_chunks() {
    std::vector<AudioIn::audio_chunk> chunks;

    for (int i = 0; i < 5; ++i) {
        AudioIn::audio_chunk chunk;
        chunk.audio_data = std::vector<uint8_t>(1024, static_cast<uint8_t>(i + 1));
        chunk.info.codec = audio_codecs::PCM_16;
        chunk.info.sample_rate_hz = 48000;
        chunk.info.num_channels = 1;
        auto now = std::chrono::system_clock::now();
        chunk.start_timestamp_ns =
            std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
        chunk.end_timestamp_ns = chunk.start_timestamp_ns;
        chunk.sequence_number = i;
        chunks.push_back(chunk);
    }

    return chunks;
}

std::vector<GeometryConfig> MockAudioIn::get_geometries(const ProtoStruct&) {
    return geometries_;
}

}  // namespace audioin
}  // namespace sdktests
}  // namespace viam
