#include "component/base/v1/base.grpc.pb.h"
#include "component/board/v1/board.grpc.pb.h"
#include "component/camera/v1/camera.grpc.pb.h"
#include "component/encoder/v1/encoder.grpc.pb.h"
#include "component/sensor/v1/sensor.grpc.pb.h"
#include "viam/sdk/components/base/base.hpp"
#include "viam/sdk/components/base/client.hpp"
#include "viam/sdk/components/base/server.hpp"
#include "viam/sdk/components/board/board.hpp"
#include "viam/sdk/components/board/client.hpp"
#include "viam/sdk/components/board/server.hpp"
#include "viam/sdk/components/camera/camera.hpp"
#include "viam/sdk/components/camera/client.hpp"
#include "viam/sdk/components/camera/server.hpp"
#include "viam/sdk/components/encoder/client.hpp"
#include "viam/sdk/components/encoder/encoder.hpp"
#include "viam/sdk/components/encoder/server.hpp"
#include "viam/sdk/components/motor/motor.hpp"
#include "viam/sdk/components/movement_sensor/movement_sensor.hpp"
#include "viam/sdk/components/power_sensor/power_sensor.hpp"
#include "viam/sdk/components/sensor/client.hpp"
#include "viam/sdk/components/sensor/sensor.hpp"
#include "viam/sdk/components/sensor/server.hpp"
#include "viam/sdk/components/servo/servo.hpp"
#include "viam/sdk/registry/registry.hpp"
#include <boost/mpl/vector.hpp>
#include <viam/sdk/common/viam.hpp>

namespace viam {
namespace sdk {

ViamInit::ViamInit() {
    Registry::register_resource<BaseClient, BaseServer, component::base::v1::BaseService>(
        API::get<Base>());
    Registry::register_resource<BoardClient, BoardServer, component::board::v1::BoardService>(
        API::get<Board>());
    Registry::register_resource<CameraClient, CameraServer, component::camera::v1::CameraService>(
        API::get<Camera>());
    Registry::register_resource<EncoderClient,
                                EncoderServer,
                                component::encoder::v1::EncoderService>(API::get<Encoder>());
    Registry::register_resource(API::get<GenericComponent>(),
                                GenericComponent::resource_registration());
    Registry::register_resource(API::get<Motor>(), Motor::resource_registration());
    Registry::register_resource(API::get<MovementSensor>(),
                                MovementSensor::resource_registration());
    Registry::register_resource(API::get<PowerSensor>(), PowerSensor::resource_registration());
    Registry::register_resource<SensorClient, SensorServer, component::sensor::v1::SensorService>(
        API::get<Sensor>());
    Registry::register_resource(API::get<Servo>(), Servo::resource_registration());
}

}  // namespace sdk
}  // namespace viam
