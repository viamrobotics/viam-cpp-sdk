#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/audio_in.hpp>
#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>

using namespace viam::sdk;

// Implements an AudioIn component that generates a sine wave for testing
class SineWaveAudioIn : public AudioIn, public Reconfigurable {
   public:
    SineWaveAudioIn(const ResourceConfig& cfg) : AudioIn(cfg.name()) {
        this->reconfigure({}, cfg);
    }

    static std::vector<std::string> validate(const ResourceConfig&);

    void reconfigure(const Dependencies&, const ResourceConfig&) override;

    ProtoStruct do_command(const ProtoStruct&) override;

    std::vector<GeometryConfig> get_geometries(const ProtoStruct&) {
        throw Exception("method not supported");
    }

    properties get_properties(const ProtoStruct&) override;

    void get_audio(std::string const& codec,
                   std::function<bool(audio_chunk&& chunk)> const& chunk_handler,
                   double const& duration_seconds,
                   int64_t const& previous_timestamp,
                   const ProtoStruct& extra) override;

   private:
    double frequency_{440.0};
};

std::vector<std::string> SineWaveAudioIn::validate(const ResourceConfig& cfg) {
    auto itr = cfg.attributes().find("frequency");
    if (itr != cfg.attributes().end()) {
        const double* freq = itr->second.get<double>();
        if (!freq) {
            throw Exception("frequency must be a number value");
        }
        if (*freq <= 0.0 || *freq > 20000.0) {
            throw Exception("frequency must be between 0 and 20000 Hz");
        }
    }
    return {};
}

void SineWaveAudioIn::reconfigure(const Dependencies&, const ResourceConfig& cfg) {
    auto itr = cfg.attributes().find("frequency");
    if (itr != cfg.attributes().end()) {
        const double* freq = itr->second.get<double>();
        if (freq) {
            frequency_ = *freq;
        }
    }
}

ProtoStruct SineWaveAudioIn::do_command(const ProtoStruct& command) {
    for (const auto& entry : command) {
        VIAM_RESOURCE_LOG(info) << "Command entry " << entry.first;
    }
    return command;
}

AudioIn::properties SineWaveAudioIn::get_properties(const ProtoStruct&) {
    AudioIn::properties props;
    props.supported_codecs = {"pcm16"};
    props.sample_rate_hz = 44100;
    props.num_channels = 1;
    return props;
}

void SineWaveAudioIn::get_audio(std::string const& codec,
                                std::function<bool(audio_chunk&& chunk)> const& chunk_handler,
                                double const& duration_seconds,
                                int64_t const& previous_timestamp,
                                const ProtoStruct& extra) {
    const int sample_rate = 44100;  // 44.1 kHz
    const double amplitude = 0.5;   // Half volume
    const int chunk_size = 1024;    // Samples per chunk

    int total_samples = static_cast<int>(duration_seconds * sample_rate);
    int num_chunks = (total_samples + chunk_size - 1) / chunk_size;

    VIAM_RESOURCE_LOG(info) << "Generating sine wave: " << frequency_ << "Hz, " << duration_seconds
                            << "s, " << num_chunks << " chunks";

    for (int chunk_idx = 0; chunk_idx < num_chunks; ++chunk_idx) {
        int samples_in_chunk = std::min(chunk_size, total_samples - (chunk_idx * chunk_size));
        std::vector<int16_t> samples;
        samples.reserve(samples_in_chunk);

        // Generate sine wave samples
        for (int i = 0; i < samples_in_chunk; ++i) {
            int sample_idx = chunk_idx * chunk_size + i;
            double t = static_cast<double>(sample_idx) / sample_rate;
            double sample_value = amplitude * std::sin(2.0 * M_PI * frequency_ * t);

            // Convert to 16-bit PCM
            int16_t pcm_sample = static_cast<int16_t>(sample_value * 32767.0);
            samples.push_back(pcm_sample);
        }

        audio_chunk chunk;

        // Convert int16_t samples to std::byte
        chunk.audio_data.resize(samples.size() * sizeof(int16_t));
        std::memcpy(chunk.audio_data.data(), samples.data(), chunk.audio_data.size());

        // Set audio info
        chunk.audio_info.codec = codec;
        chunk.audio_info.sample_rate_hz = sample_rate;
        chunk.audio_info.num_channels = 1;

        // Get current timestamp in nanoseconds
        auto now = std::chrono::system_clock::now();
        auto nanos =
            std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
        chunk.start_timestamp_ns = nanos;
        chunk.end_timestamp_ns = nanos;
        chunk.sequence = chunk_idx;

        // Call the chunk handler callback
        if (!chunk_handler(std::move(chunk))) {
            VIAM_RESOURCE_LOG(info) << "Chunk handler returned false, stopping";
            break;
        }
    }

    VIAM_RESOURCE_LOG(info) << "Finished generating sine wave";
}

int main(int argc, char** argv) try {
    // Every Viam C++ SDK program must have one and only one Instance object which is created before
    // any other C++ SDK objects and stays alive until all Viam C++ SDK objects are destroyed.
    Instance inst;

    Model sinewave_model("viam", "audio_in", "sinewave");

    std::shared_ptr<ModelRegistration> mr = std::make_shared<ModelRegistration>(
        API::get<AudioIn>(),
        sinewave_model,
        [](Dependencies, ResourceConfig cfg) { return std::make_unique<SineWaveAudioIn>(cfg); },
        &SineWaveAudioIn::validate);

    std::vector<std::shared_ptr<ModelRegistration>> mrs = {mr};
    auto my_mod = std::make_shared<ModuleService>(argc, argv, mrs);
    my_mod->serve();

    return EXIT_SUCCESS;
} catch (const viam::sdk::Exception& ex) {
    std::cerr << "main failed with exception: " << ex.what() << "\n";
    return EXIT_FAILURE;
}
