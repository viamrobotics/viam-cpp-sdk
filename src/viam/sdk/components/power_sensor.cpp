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

bool operator==(const PowerSensor::voltage& lhs, const PowerSensor::voltage& rhs) {
    return (lhs.volts == rhs.volts && lhs.is_ac == rhs.is_ac);
}

bool operator==(const PowerSensor::current& lhs, const PowerSensor::current& rhs) {
    return (lhs.amperes == rhs.amperes && lhs.is_ac == rhs.is_ac);
}

}  // namespace sdk
}  // namespace viam
