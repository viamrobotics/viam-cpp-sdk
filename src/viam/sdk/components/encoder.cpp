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

Encoder::Encoder(std::string name) : Component(std::move(name)) {}

Encoder::Encoder(const ResourceConfig& cfg) : Component(cfg) {}

}  // namespace sdk
}  // namespace viam
