#include <viam/sdk/components/gripper.hpp>

namespace viam {
namespace sdk {

API Gripper::api() const {
    return API::get<Gripper>();
}

API API::traits<Gripper>::api() {
    return {kRDK, kComponent, "gripper"};
}

Gripper::Gripper(std::string name) : Component(std::move(name)) {}

Gripper::Gripper(const ResourceConfig& cfg) : Component(cfg) {}

}  // namespace sdk
}  // namespace viam
