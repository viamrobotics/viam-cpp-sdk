#include <viam/sdk/common/audio.hpp>

#include <fstream>
#include <stdexcept>
#include <tuple>

namespace viam {
namespace sdk {

namespace {
template <typename T>
void write_value(std::ofstream& out, const T& value) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}
}  // anonymous namespace

bool operator==(const audio_properties& lhs, const audio_properties& rhs) {
    return std::tie(lhs.supported_codecs, lhs.sample_rate_hz, lhs.num_channels) ==
           std::tie(rhs.supported_codecs, rhs.sample_rate_hz, rhs.num_channels);
}

uint16_t get_bits_per_sample(const std::string& codec) {
    if (codec == audio_codecs::PCM_16) {
        return 16;
    }
    if (codec == audio_codecs::PCM_32 || codec == audio_codecs::PCM_32_FLOAT) {
        return 32;
    }
    throw std::runtime_error("Unsupported codec for WAV file: " + codec);
}

void write_wav_file(const std::string& filename,
                    const std::vector<uint8_t>& audio_data,
                    const std::string& codec,
                    uint32_t sample_rate_hz,
                    uint16_t num_channels) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    const uint16_t bits_per_sample = get_bits_per_sample(codec);
    const uint32_t data_size = audio_data.size();
    const uint32_t byte_rate = sample_rate_hz * num_channels * (bits_per_sample / 8);
    const uint16_t block_align = num_channels * (bits_per_sample / 8);

    outfile.write("RIFF", 4);
    const uint32_t chunk_size = 36 + data_size;
    write_value(outfile, chunk_size);
    outfile.write("WAVE", 4);

    outfile.write("fmt ", 4);
    const uint32_t subchunk1_size = 16;
    write_value(outfile, subchunk1_size);
    const uint16_t audio_format = 1;
    write_value(outfile, audio_format);
    write_value(outfile, num_channels);
    write_value(outfile, sample_rate_hz);
    write_value(outfile, byte_rate);
    write_value(outfile, block_align);
    write_value(outfile, bits_per_sample);

    outfile.write("data", 4);
    write_value(outfile, data_size);
    outfile.write(reinterpret_cast<const char*>(audio_data.data()), data_size);

    outfile.close();
}

}  // namespace sdk
}  // namespace viam
