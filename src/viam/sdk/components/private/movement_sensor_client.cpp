#include <viam/sdk/components/private/movement_sensor_client.hpp>

#include <grpcpp/channel.h>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/movement_sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

using sdk::from_proto;
using sdk::to_proto;

MovementSensor::compassheading from_proto(
    const viam::component::movementsensor::v1::GetCompassHeadingResponse& proto) {
    MovementSensor::compassheading compassheading;
    compassheading.value = proto.value();
    return compassheading;
}

MovementSensor::position from_proto(
    const viam::component::movementsensor::v1::GetPositionResponse& proto) {
    MovementSensor::position position;
    position.coordinate = from_proto(proto.coordinate());
    position.altitude_m = proto.altitude_m();
    return position;
}

MovementSensor::orientation from_proto(const viam::common::v1::Orientation& proto) {
    MovementSensor::orientation orientation;
    orientation.o_x = proto.o_x();
    orientation.o_y = proto.o_y();
    orientation.o_z = proto.o_z();
    orientation.theta = proto.theta();
    return orientation;
}

MovementSensor::properties from_proto(
    const viam::component::movementsensor::v1::GetPropertiesResponse& proto) {
    MovementSensor::properties properties;
    properties.linear_velocity_supported = proto.linear_velocity_supported();
    properties.angular_velocity_supported = proto.angular_velocity_supported();
    properties.orientation_supported = proto.orientation_supported();
    properties.position_supported = proto.position_supported();
    properties.compass_heading_supported = proto.compass_heading_supported();
    properties.linear_acceleration_supported = proto.linear_acceleration_supported();
    return properties;
}

MovementSensorClient::MovementSensorClient(std::string name, const ViamChannel& channel)
    : MovementSensor(std::move(name)),
      stub_(viam::component::movementsensor::v1::MovementSensorService::NewStub(channel.channel())),
      channel_(&channel) {}

using namespace viam::component::movementsensor::v1;

Vector3 MovementSensorClient::get_linear_velocity(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetLinearVelocity)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response.linear_velocity()); });
}

Vector3 MovementSensorClient::get_angular_velocity(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetAngularVelocity)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response.angular_velocity()); });
}

MovementSensor::compassheading MovementSensorClient::get_compass_heading(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetCompassHeading)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

MovementSensor::orientation MovementSensorClient::get_orientation(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetOrientation)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response.orientation()); });
}

MovementSensor::position MovementSensorClient::get_position(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPosition)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

MovementSensor::properties MovementSensorClient::get_properties(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

std::unordered_map<std::string, float> MovementSensorClient::get_accuracy(
    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetAccuracy)
        .with(extra)
        .invoke([](auto& response) {
            std::unordered_map<std::string, float> result;
            for (const auto& i : response.accuracy()) {
                result.emplace(i.first, i.second);
            }
            return result;
        });
}

Vector3 MovementSensorClient::get_linear_acceleration(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetLinearAcceleration)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response.linear_acceleration()); });
}

ProtoStruct MovementSensorClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

std::vector<GeometryConfig> MovementSensorClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
