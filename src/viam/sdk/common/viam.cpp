#include "viam/sdk/components/base/base.hpp"
#include "viam/sdk/components/board/board.hpp"
#include "viam/sdk/components/camera/camera.hpp"
#include "viam/sdk/components/encoder/encoder.hpp"
#include "viam/sdk/components/motor/motor.hpp"
#include "viam/sdk/components/movement_sensor/movement_sensor.hpp"
#include "viam/sdk/components/power_sensor/power_sensor.hpp"
#include "viam/sdk/components/sensor/sensor.hpp"
#include "viam/sdk/components/servo/servo.hpp"
#include "viam/sdk/registry/registry.hpp"
#include <boost/mpl/vector.hpp>
#include <viam/sdk/common/viam.hpp>

namespace viam {
namespace sdk {

// CR erodkin: probably we can't make this work sadly.
typedef boost::mpl::vector<Base, Board, Camera> resources;

ViamInit::ViamInit() {
    Registry::register_resource(API::get<Base>(), Base::resource_registration());
    Registry::register_resource(API::get<Board>(), Board::resource_registration());
    Registry::register_resource(API::get<Camera>(), Camera::resource_registration());
    Registry::register_resource(API::get<Encoder>(), Encoder::resource_registration());
    Registry::register_resource(API::get<GenericComponent>(),
                                GenericComponent::resource_registration());
    Registry::register_resource(API::get<Motor>(), Motor::resource_registration());
    Registry::register_resource(API::get<MovementSensor>(),
                                MovementSensor::resource_registration());
    Registry::register_resource(API::get<PowerSensor>(), PowerSensor::resource_registration());
    Registry::register_resource(API::get<Sensor>(), Sensor::resource_registration());
    Registry::register_resource(API::get<Servo>(), Servo::resource_registration());
}

}  // namespace sdk
}  // namespace viam
