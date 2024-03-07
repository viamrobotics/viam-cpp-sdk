#include <viam/sdk/components/private/movement_sensor_client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/movementsensor/v1/movementsensor.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/movement_sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

MovementSensorClient::MovementSensorClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : MovementSensor(std::move(name)),
      stub_(viam::component::movementsensor::v1::MovementSensorService::NewStub(channel)),
      channel_(std::move(channel)){};

using namespace viam::component::movementsensor::v1;

Vector3 MovementSensorClient::get_linear_velocity(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetLinearVelocity)
        .with(extra)
        .invoke([](auto& response) { return Vector3::from_proto(response.linear_velocity()); });
}

Vector3 MovementSensorClient::get_angular_velocity(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetAngularVelocity)
        .with(extra)
        .invoke([](auto& response) { return Vector3::from_proto(response.angular_velocity()); });
}

MovementSensor::compassheading MovementSensorClient::get_compass_heading(
    const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetCompassHeading)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

MovementSensor::orientation MovementSensorClient::get_orientation(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetOrientation)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response.orientation()); });
}

MovementSensor::position MovementSensorClient::get_position(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPosition)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

MovementSensor::properties MovementSensorClient::get_properties(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

std::unordered_map<std::string, float> MovementSensorClient::get_accuracy(
    const AttributeMap& extra) {
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

Vector3 MovementSensorClient::get_linear_acceleration(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetLinearAcceleration)
        .with(extra)
        .invoke([](auto& response) { return Vector3::from_proto(response.linear_acceleration()); });
}

AttributeMap MovementSensorClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

std::vector<GeometryConfig> MovementSensorClient::get_geometries(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return GeometryConfig::from_proto(response); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
