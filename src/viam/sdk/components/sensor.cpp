#include <viam/sdk/components/sensor.hpp>

#include <viam/api/component/sensor/v1/sensor.grpc.pb.h>
#include <viam/api/component/sensor/v1/sensor.pb.h>

#include <viam/sdk/resource/resource.hpp>

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
