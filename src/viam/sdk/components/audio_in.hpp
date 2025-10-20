/// @file components/audio_in.hpp
///
/// @brief Defines a `AudioIn` component.
#pragma once

#include <string>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>


namespace viam {
namespace sdk {

/// @defgroup Base Classes related to the AudioIn component.

/// @class AudioIn audio_in.hpp "components/audio_in.hpp"
/// @brief An `AudioIn` is a deivce that can take audio input.
/// @ingroup AudioIn
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific AudioIn implementations. This class cannot be used on its own.
class AudioIn : public Component {
    public:
    /// @struct properties
    /// @brief Information about the AudioIn component
    struct properties {
        std::vector<std::string> supported_codecs;
        int sample_rate_hz;
        int num_channels;
    };


    // AudioInfo describes information about a piece of audio data
    struct AudioInfo {
        std::string codec;
        int sample_rate_hz;
        int num_channels;
    };


    struct AudioChunk {
        std::vector<std::byte> audio_data;
        AudioInfo audio_info;
        int start_timestamp_ns;
        int end_timestamp_ns;
        int sequence;
        std::string request_id;
    };

    /// @brief Get a stream of audio from the device
    /// until completed or cancelled
    /// @param codec requested codec of the audio data
    /// @param chunk_handler callback function to call when an audio response is received.
    /// For an infinite stream this should return true to keep streaming audio and false to indicate that the stream should terminate.
    /// The callback function should not be blocking.
    /// @param duration_seconds duration of audio stream. If not set, stream duration is indefinite.
    /// @param previous_timestamp timestamp to start the audio stream from for continuity between multiple calls. If not set, will stream data
    // starting from the time the request was received by the server.
    inline void get_audio(std::string const& codec,
                            std::function<bool(AudioChunk&& chunk)> const& chunk_handler,
                            double const& duration_seconds,
                            int64_t const& previous_timestamp) {
        return get_audio(codec, chunk_handler, duration_seconds, previous_timestamp, {});
    }


    /// @brief Get a stream of audio from the device
    /// until completed or cancelled
    /// @param codec requested codec of the audio data
    /// @param chunk_handler callback function to call when an audio response is received.
    /// For an infinite stream this should return true to keep streaming audio and false to indicate that the stream should terminate.
    /// The callback function should not be blocking.
    /// @param duration_seconds duration of audio stream. If zero, stream duration is indefinite.
    /// @param previous_timestamp timestamp to start the audio stream from for continuity between multiple calls. If zero, will stream data
    //starting from the time the request was received by the server.
    /// @param extra Any additional arguments to the method
    virtual void get_audio(std::string const& codec,
                            std::function<bool(AudioChunk&& chunk)> const& chunk_handler,
                            double const& duration_seconds,
                            int64_t const& previous_timestamp,
                            const ProtoStruct& extra) = 0;

    /// @brief Returns properties of the audio in device (supported codecs, sample rate, number of channels)
    inline properties get_properties() {
        return get_properties({});
    }

    /// @brief Returns properties of the audio in device (supported codecs, sample rate, number of channels)
    /// @param extra Any additional arguments to the method
    virtual properties get_properties(const ProtoStruct& extra) = 0;

    // @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    API api() const override;

   protected:
    explicit AudioIn(std::string name);

    };


    template <>
    struct API::traits<AudioIn> {
        static API api();
    };

    bool operator==(const AudioIn::properties& lhs, const AudioIn::properties& rhs);



} // namespace sdk
} // namespace viam

