#include <viam/sdk/components/audio_in.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API AudioIn::api() const {
    return API::get<AudioIn>();
}

API API::traits<AudioIn>::api() {
    return {kRDK, kComponent, "audio_in"};
}

AudioIn::AudioIn(std::string name) : Component(std::move(name)) {}



bool operator==(const AudioIn::properties& lhs, const AudioIn::properties& rhs) {
    return lhs.supported_codecs == rhs.supported_codecs &&
           lhs.sample_rate_hz == rhs.sample_rate_hz &&
           lhs.sample_rate_hz == rhs.sample_rate_hz;
}


}  // namespace sdk
}  // namespace viam
