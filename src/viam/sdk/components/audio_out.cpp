#include <viam/sdk/components/audio_out.hpp>

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API AudioOut::api() const {
    return API::get<AudioOut>();
}

API API::traits<AudioOut>::api() {
    return {kRDK, kComponent, "audio_out"};
}

AudioOut::AudioOut(std::string name) : Component(std::move(name)) {}

}  // namespace sdk
}  // namespace viam
