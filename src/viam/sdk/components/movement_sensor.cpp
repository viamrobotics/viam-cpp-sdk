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

}  // namespace sdk
}  // namespace viam
