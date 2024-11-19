#include <viam/sdk/components/arm.hpp>

namespace viam {
namespace sdk {

API Arm::api() const {
    return API::get<Arm>();
}

API API::traits<Arm>::api() {
    return {kRDK, kComponent, "arm"};
}

Arm::Arm(std::string name) : Component(std::move(name)) {}

}  // namespace sdk
}  // namespace viam
