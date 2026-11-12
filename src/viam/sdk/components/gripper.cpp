#include <viam/sdk/components/gripper.hpp>

#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {

API Gripper::api() const {
    return API::get<Gripper>();
}

API API::traits<Gripper>::api() {
    return {kRDK, kComponent, "gripper"};
}

KinematicModel Gripper::get_kinematic_model(const kinematics_options&, const ProtoStruct&) {
    throw Exception(ErrorCondition::k_not_supported,
                    "gripper '" + name() + "' does not provide a kinematic model");
}

Gripper::Gripper(std::string name) : Component(std::move(name)) {}

Gripper::Gripper(const ResourceConfig& cfg) : Component(cfg) {}

}  // namespace sdk
}  // namespace viam
