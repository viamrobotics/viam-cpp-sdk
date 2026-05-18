#pragma once

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/audio_out.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdktests {
namespace audioout {

using viam::sdk::AudioOut;
using namespace viam::sdk;

// Forward declarations
class MockAudioOutStreamWriter;
class MockAudioOutStreamReader;

class MockAudioOut : public AudioOut {
   public:
    void play(std::vector<uint8_t> const& audio_data,
              boost::optional<audio_info> info,
              const sdk::ProtoStruct& extra) override;
    audio_properties get_properties(const sdk::ProtoStruct& extra) override;
    viam::sdk::ProtoStruct do_command(const viam::sdk::ProtoStruct& command) override;
    sdk::ProtoStruct get_status() override;
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;

    // New overrides for stream play
    std::unique_ptr<AudioOutStreamWriter> play_stream(audio_info info, const sdk::ProtoStruct& extra) override;
    void play_stream(std::unique_ptr<AudioOutStreamReader> reader, audio_info info, const sdk::ProtoStruct& extra) override;

    static std::shared_ptr<MockAudioOut> get_mock_audio_out();

    MockAudioOut(std::string name) : AudioOut(std::move(name)) {}

    using AudioOut::get_geometries;
    using AudioOut::get_properties;
    using AudioOut::play;

    audio_properties properties_;
    viam::sdk::ProtoStruct map_;
    std::vector<GeometryConfig> geometries_;
    std::vector<uint8_t> last_played_audio_;
    boost::optional<audio_info> last_played_audio_info_;

    // New public members for testing and verification
    std::vector<std::vector<uint8_t>> streamed_audio_chunks_;
    boost::optional<audio_info> streamed_audio_info_;
    sdk::ProtoStruct streamed_extra_;
};

class MockAudioOutStreamWriter : public AudioOutStreamWriter {
   public:
    MockAudioOutStreamWriter(std::shared_ptr<MockAudioOut> parent);
    void write(std::vector<uint8_t> const& audio_data) override;
    void close() override;

   private:
    std::shared_ptr<MockAudioOut> parent_;
};

class MockAudioOutStreamReader : public AudioOutStreamReader {
   public:
    MockAudioOutStreamReader(std::shared_ptr<MockAudioOut> parent);
    boost::optional<std::vector<uint8_t>> read() override;

   private:
    std::shared_ptr<MockAudioOut> parent_;
    size_t current_chunk_index_;
};

audio_properties fake_properties();

}  // namespace audioout
}  // namespace sdktests
}  // namespace viam