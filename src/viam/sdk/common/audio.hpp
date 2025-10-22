#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace viam {
namespace sdk {

/// @brief Common audio codecs
namespace audio_codecs {
constexpr const char* PCM_16 = "pcm16";
constexpr const char* PCM_32 = "pcm32";
constexpr const char* PCM_32_FLOAT = "pcm32_float";
constexpr const char* MP3 = "mp3";
constexpr const char* AAC = "aac";
constexpr const char* OPUS = "opus";
constexpr const char* FLAC = "flac";
}  // namespace audio_codecs

/// @struct audio_properties
/// @brief Properties of an audio component (input or output)
struct audio_properties {
    std::vector<std::string> supported_codecs;
    int sample_rate_hz;
    int num_channels;
};

/// @struct audio_info
/// @brief Information about a piece of audio data
struct audio_info {
    std::string codec;
    int sample_rate_hz;
    int num_channels;
};

/// @brief Equality operator for properties
bool operator==(const audio_properties& lhs, const audio_properties& rhs);

/// @brief Equality operator for audio_info
bool operator==(const audio_info& lhs, const audio_info& rhs);

uint16_t get_bits_per_sample(const std::string& codec);

void write_wav_file(const std::string& filename,
                    const std::vector<uint8_t>& audio_data,
                    const std::string& codec,
                    uint32_t sample_rate_hz,
                    uint16_t num_channels);

}  // namespace sdk
}  // namespace viam
