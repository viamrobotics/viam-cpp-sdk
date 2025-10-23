/// @file components/audio_out.hpp
///
/// @brief Defines an `AudioOut` component.
#pragma once

#include <string>

#include <boost/optional/optional.hpp>

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>

namespace viam {
namespace sdk {

/// @defgroup AudioOut Classes related to the AudioOut component.

/// @class AudioOut audio_out.hpp "components/audio_out.hpp"
/// @brief An `AudioOut` is a device that can output audio.
/// @ingroup AudioOut
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific AudioOut implementations. This class cannot be used on its own.
class AudioOut : public Component {
   public:
    using audio_info = viam::sdk::audio_info;

    /// @brief Play audio data
    /// @param audio_data The audio data to play as bytes
    /// @param info Optional info about the audio_data (codec, sample rate, channels). Required for
    /// raw PCM data.
    inline void play(std::vector<uint8_t> const& audio_data, boost::optional<audio_info> info) {
        return play(audio_data, info, {});
    }

    /// @brief Play audio through the audioout component
    /// @param audio_data The audio data to play
    /// @param info Optional info about the audio_data (codec, sample rate, channels). Required for
    /// raw PCM data.
    /// @param extra Any additional arguments to the method
    virtual void play(std::vector<uint8_t> const& audio_data,
                      boost::optional<audio_info> info,
                      const ProtoStruct& extra) = 0;

    /// @brief Returns properties of the audio out device (supported codecs, sample rate, number of
    /// channels)
    inline audio_properties get_properties() {
        return get_properties({});
    }

    /// @brief Returns properties of the audio out device (supported codecs, sample rate, number of
    /// channels)
    /// @param extra Any additional arguments to the method
    virtual audio_properties get_properties(const ProtoStruct& extra) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param command the command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    // @brief Returns `GeometryConfig`s associated with the calling audioout.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling audioout.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) = 0;

    API api() const override;

   protected:
    explicit AudioOut(std::string name);
};

template <>
struct API::traits<AudioOut> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
