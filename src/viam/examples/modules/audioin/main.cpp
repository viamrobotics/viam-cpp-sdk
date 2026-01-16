#define _USE_MATH_DEFINES
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <viam/sdk/common/audio.hpp>
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

    std::vector<GeometryConfig> get_geometries(const ProtoStruct&) override {
        throw Exception("method not supported");
    }

    audio_properties get_properties(const ProtoStruct&) override;

    void get_audio(std::string const& codec,
                   std::function<bool(audio_chunk&& chunk)> const& chunk_handler,
                   double const& duration_seconds,
                   int64_t const& previous_timestamp,
                   const ProtoStruct& extra) override;

   private:
    double frequency_{440.0};

    static double generate_sine_sample(double frequency, double amplitude, double time_seconds);
    static int16_t float_to_pcm16(double sample_value);
    static std::vector<uint8_t> pcm16_samples_to_bytes(const std::vector<int16_t>& samples);
    static audio_chunk create_audio_chunk(const std::vector<int16_t>& samples,
                                          const std::string& codec,
                                          int sample_rate_hz,
                                          int num_channels,
                                          int sequence_number);
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
        VIAM_RESOURCE_LOG_THIS(info) << "Command entry " << entry.first;
    }
    return command;
}

// Generates a single audio sample representing a sine wave at the given frequency, amplitude, and
// time.
double SineWaveAudioIn::generate_sine_sample(double frequency,
                                             double amplitude,
                                             double time_seconds) {
    return amplitude * std::sin(2.0 * M_PI * frequency * time_seconds);
}

// Converts a normalized floating-point sample (-1.0 to 1.0) to 16-bit PCM format.
int16_t SineWaveAudioIn::float_to_pcm16(double sample_value) {
    return static_cast<int16_t>(sample_value * 32767.0);
}

std::vector<uint8_t> SineWaveAudioIn::pcm16_samples_to_bytes(const std::vector<int16_t>& samples) {
    std::vector<uint8_t> bytes(samples.size() * sizeof(int16_t));
    std::copy(reinterpret_cast<const uint8_t*>(samples.data()),
              reinterpret_cast<const uint8_t*>(samples.data()) + bytes.size(),
              bytes.begin());
    return bytes;
}

AudioIn::audio_chunk SineWaveAudioIn::create_audio_chunk(const std::vector<int16_t>& samples,
                                                         const std::string& codec,
                                                         int sample_rate_hz,
                                                         int num_channels,
                                                         int sequence_number) {
    audio_chunk chunk;
    chunk.audio_data = pcm16_samples_to_bytes(samples);
    chunk.info.codec = codec;
    chunk.info.sample_rate_hz = sample_rate_hz;
    chunk.info.num_channels = num_channels;

    auto now = std::chrono::system_clock::now();
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
    // Set both start and end timestamps to the current system time in nanoseconds.
    // In a real application, start_timestamp_ns would mark the time the audio chunk begins,
    // and end_timestamp_ns would mark when it ends. Here they are equal for simplicity.
    chunk.start_timestamp_ns = nanos;
    chunk.end_timestamp_ns = nanos;
    chunk.sequence_number = sequence_number;

    return chunk;
}

audio_properties SineWaveAudioIn::get_properties(const ProtoStruct&) {
    audio_properties props;
    props.supported_codecs = {audio_codecs::PCM_16};
    props.sample_rate_hz = 44100;
    props.num_channels = 1;
    return props;
}

void SineWaveAudioIn::get_audio(std::string const& codec,
                                std::function<bool(audio_chunk&& chunk)> const& chunk_handler,
                                double const& duration_seconds,
                                int64_t const& previous_timestamp,
                                const ProtoStruct& extra) {
    const int sample_rate = 44100;
    const double amplitude = 0.5;
    const int chunk_size = 1024;

    int total_samples = static_cast<int>(duration_seconds * sample_rate);
    int num_chunks = (total_samples + chunk_size - 1) / chunk_size;

    VIAM_RESOURCE_LOG_THIS(info) << "Generating sine wave: " << frequency_ << "Hz, "
                                 << duration_seconds << "s, " << num_chunks << " chunks";

    for (int chunk_idx = 0; chunk_idx < num_chunks; ++chunk_idx) {
        int samples_in_chunk = std::min(chunk_size, total_samples - (chunk_idx * chunk_size));
        std::vector<int16_t> samples;
        samples.reserve(samples_in_chunk);

        // Create each sample and put in chunk.
        for (int i = 0; i < samples_in_chunk; ++i) {
            int sample_idx = chunk_idx * chunk_size + i;
            double time_seconds = static_cast<double>(sample_idx) / sample_rate;
            double sample_value = generate_sine_sample(frequency_, amplitude, time_seconds);
            samples.push_back(float_to_pcm16(sample_value));
        }

        audio_chunk chunk = create_audio_chunk(samples, codec, sample_rate, 1, chunk_idx);

        if (!chunk_handler(std::move(chunk))) {
            VIAM_RESOURCE_LOG_THIS(info) << "Chunk handler returned false, stopping";
            break;
        }
    }

    VIAM_RESOURCE_LOG_THIS(info) << "Finished generating sine wave";
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
