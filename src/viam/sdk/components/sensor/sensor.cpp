#include <viam/sdk/components/sensor/sensor.hpp>

#include <viam/api/component/sensor/v1/sensor.grpc.pb.h>
#include <viam/api/component/sensor/v1/sensor.pb.h>

#include <viam/sdk/components/sensor/client.hpp>
#include <viam/sdk/components/sensor/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API Sensor::api() const {
    return API::get<Sensor>();
}

API API::traits<Sensor>::api() {
    return {kRDK, kComponent, "sensor"};
}

namespace {
class SensorRegistration final
    : public ResourceRegistration2<SensorClient,
                                   SensorServer,
                                   viam::component::sensor::v1::SensorService,
                                   SensorRegistration> {
   public:
    using ResourceRegistration2::ResourceRegistration2;
};

bool init() {
    Registry::register_resource(API::get<Sensor>(), SensorRegistration::resource_registration());
    return true;
};

// NOLINTNEXTLINE(cert-err58-cpp)
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
