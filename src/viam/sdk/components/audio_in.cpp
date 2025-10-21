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

}  // namespace sdk
}  // namespace viam
