#include <viam/sdk/components/sensor.hpp>

namespace viam {
namespace sdk {

API Sensor::api() const {
    return API::get<Sensor>();
}

API API::traits<Sensor>::api() {
    return {kRDK, kComponent, "sensor"};
}

}  // namespace sdk
}  // namespace viam
