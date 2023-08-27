#include <viam/sdk/components/movementsensor/movementsensor.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/movementsensor/v1/movementsensor.grpc.pb.h>
#include <viam/api/component/movementsensor/v1/movementsensor.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/movementsensor/client.hpp>
#include <viam/sdk/components/movementsensor/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

MovementSensorRegistration::MovementSensorRegistration(
    const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceRegistration(service_descriptor){};

std::shared_ptr<ResourceServer> MovementSensorRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<MovementSensorServer>(manager);
};

std::shared_ptr<Resource> MovementSensorRegistration::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<MovementSensorClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceRegistration> MovementSensor::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd = p->FindServiceByName(
        viam::component::movementsensor::v1::MovementSensorService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the movementsensor service");
    }
    return std::make_shared<MovementSensorRegistration>(sd);
}

API MovementSensor::static_api() {
    return {kRDK, kComponent, "movementsensor"};
}

API MovementSensor::dynamic_api() const {
    return static_api();
}

MovementSensor::compassheading MovementSensor::from_proto(
    viam::component::movementsensor::v1::GetCompassHeadingResponse proto) {
    MovementSensor::compassheading compassheading;
    compassheading.value = proto.value();
    return compassheading;
}

MovementSensor::position MovementSensor::from_proto(
    viam::component::movementsensor::v1::GetPositionResponse proto) {
    MovementSensor::position position;
    position.coordinate = viam::sdk::geo_point::from_proto(proto.coordinate());
    position.altitude_m = proto.altitude_m();
    return position;
}

MovementSensor::orientation MovementSensor::from_proto(viam::common::v1::Orientation proto) {
    MovementSensor::orientation orientation;
    orientation.o_x = proto.o_x();
    orientation.o_y = proto.o_y();
    orientation.o_z = proto.o_z();
    orientation.theta = proto.theta();
    return orientation;
}

MovementSensor::properties MovementSensor::from_proto(
    viam::component::movementsensor::v1::GetPropertiesResponse proto) {
    MovementSensor::properties properties;
    properties.linear_velocity_supported = proto.linear_velocity_supported();
    properties.angular_velocity_supported = proto.angular_velocity_supported();
    properties.orientation_supported = proto.orientation_supported();
    properties.position_supported = proto.position_supported();
    properties.compass_heading_supported = proto.compass_heading_supported();
    properties.linear_acceleration_supported = proto.linear_acceleration_supported();
    return properties;
}

viam::component::movementsensor::v1::GetCompassHeadingResponse MovementSensor::to_proto(
    compassheading compassheading) {
    viam::component::movementsensor::v1::GetCompassHeadingResponse proto;
    proto.set_value(compassheading.value);
    return proto;
}

viam::common::v1::Orientation MovementSensor::to_proto(orientation orientation) {
    viam::common::v1::Orientation proto;
    proto.set_o_x(orientation.o_x);
    proto.set_o_y(orientation.o_y);
    proto.set_o_z(orientation.o_z);
    proto.set_theta(orientation.theta);
    return proto;
}

viam::component::movementsensor::v1::GetPropertiesResponse MovementSensor::to_proto(
    properties properties) {
    viam::component::movementsensor::v1::GetPropertiesResponse proto;
    proto.set_linear_velocity_supported(properties.linear_velocity_supported);
    proto.set_angular_velocity_supported(properties.angular_velocity_supported);
    proto.set_orientation_supported(properties.orientation_supported);
    proto.set_position_supported(properties.position_supported);
    proto.set_compass_heading_supported(properties.compass_heading_supported);
    proto.set_linear_acceleration_supported(properties.linear_acceleration_supported);
    return proto;
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

namespace {
bool init() {
    Registry::register_resource(MovementSensor::static_api(),
                                MovementSensor::resource_registration());
    return true;
};

// NOLINTNEXTLINE(cert-err58-cpp)
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
