#include <viam/sdk/components/switch.hpp>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

API Switch::api() const {
    return API::get<Switch>();
}

API API::traits<Switch>::api() {
    return {kRDK, kComponent, "switch"};
}

Switch::Switch(std::string name) : Component(std::move(name)) {}

}  // namespace sdk
}  // namespace viam
