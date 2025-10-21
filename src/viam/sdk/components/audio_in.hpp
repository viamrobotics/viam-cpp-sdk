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

    /// @struct audio_chunk
    /// @brief A sequential chunk of audio data with timing information for continuous audio streams.
    struct audio_chunk {
        std::vector<std::byte> audio_data;
        audio_info audio_info;
        int64_t start_timestamp_ns;
        int64_t end_timestamp_ns;
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
                            std::function<bool(audio_chunk&& chunk)> const& chunk_handler,
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
                            std::function<bool(audio_chunk&& chunk)> const& chunk_handler,
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

    /// @brief Returns `GeometryConfig`s associated with the calling AudioIn.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling AudioIn.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) = 0;

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

