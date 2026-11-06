#include <viam/sdk/components/motor.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API Motor::api() const {
    return API::get<Motor>();
}

API API::traits<Motor>::api() {
    return {kRDK, kComponent, "motor"};
}

Motor::Motor(std::string name) : Component(std::move(name)) {}

Motor::Motor(const ResourceConfig& cfg) : Component(cfg) {}

}  // namespace sdk
}  // namespace viam
