#include <viam/sdk/components/power_sensor.hpp>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API PowerSensor::api() const {
    return API::get<PowerSensor>();
}

API API::traits<PowerSensor>::api() {
    return {kRDK, kComponent, "power_sensor"};
}

}  // namespace sdk
}  // namespace viam
