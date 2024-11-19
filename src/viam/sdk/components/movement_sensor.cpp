#include <viam/sdk/components/movement_sensor.hpp>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API MovementSensor::api() const {
    return API::get<MovementSensor>();
}

API API::traits<MovementSensor>::api() {
    return {kRDK, kComponent, "movement_sensor"};
}

bool operator==(const MovementSensor::compassheading& lhs,
                const MovementSensor::compassheading& rhs) {
    return (lhs.value == rhs.value);
}

bool operator==(const MovementSensor::position& lhs, const MovementSensor::position& rhs) {
    return (lhs.coordinate == rhs.coordinate && lhs.altitude_m == rhs.altitude_m);
}

bool operator==(const MovementSensor::properties& lhs, const MovementSensor::properties& rhs) {
    return (lhs.linear_velocity_supported == rhs.linear_velocity_supported &&
            lhs.angular_velocity_supported == rhs.angular_velocity_supported &&
            lhs.orientation_supported == rhs.orientation_supported &&
            lhs.position_supported == rhs.position_supported &&
            lhs.compass_heading_supported == rhs.compass_heading_supported &&
            lhs.linear_acceleration_supported == rhs.linear_acceleration_supported);
}

}  // namespace sdk
}  // namespace viam
