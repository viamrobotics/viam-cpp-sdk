#include <viam/sdk/components/encoder.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API Encoder::api() const {
    return API::get<Encoder>();
}

API API::traits<Encoder>::api() {
    return {kRDK, kComponent, "encoder"};
}

Encoder::Encoder(std::string name) : Component(std::move(name)) {};

bool operator==(const Encoder::position& lhs, const Encoder::position& rhs) {
    return (lhs.value == rhs.value && lhs.type == rhs.type);
}

bool operator==(const Encoder::properties& lhs, const Encoder::properties& rhs) {
    return (lhs.ticks_count_supported == rhs.ticks_count_supported &&
            lhs.angle_degrees_supported == rhs.angle_degrees_supported);
}

}  // namespace sdk
}  // namespace viam
