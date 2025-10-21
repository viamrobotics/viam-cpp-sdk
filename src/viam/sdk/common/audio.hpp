#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace viam {
namespace sdk {

    /// @struct properties
    /// @brief properties of the AudioIn component
    struct properties {
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
    bool operator==(const properties& lhs, const properties& rhs);

    /// @brief Inequality operator for properties
    bool operator!=(const properties& lhs, const properties& rhs);

}  // namespace sdk
}  // namespace viam
