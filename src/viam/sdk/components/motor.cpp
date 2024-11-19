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

Motor::Motor(std::string name) : Component(std::move(name)){};

bool operator==(const Motor::power_status& lhs, const Motor::power_status& rhs) {
    return (lhs.is_on == rhs.is_on && lhs.power_pct == rhs.power_pct);
}

bool operator==(const Motor::properties& lhs, const Motor::properties& rhs) {
    return (lhs.position_reporting == rhs.position_reporting);
}

}  // namespace sdk
}  // namespace viam
