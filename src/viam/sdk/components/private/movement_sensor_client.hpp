/// @file components/movement_sensor/client.hpp
///
/// @brief Implements a gRPC client for the `MovementSensor` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/movementsensor/v1/movementsensor.grpc.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/components/movement_sensor/movement_sensor.hpp>
#include <viam/sdk/components/movement_sensor/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

/// @class MovementSensorClient
/// @brief gRPC client implementation of a `MovementSensor` component.
/// @ingroup MovementSensor
class MovementSensorClient : public MovementSensor {
   public:
    using interface_type = MovementSensor;
    MovementSensorClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    Vector3 get_linear_velocity(const AttributeMap& extra) override;
    Vector3 get_angular_velocity(const AttributeMap& extra) override;
    compassheading get_compass_heading(const AttributeMap& extra) override;
    orientation get_orientation(const AttributeMap& extra) override;
    position get_position(const AttributeMap& extra) override;
    properties get_properties(const AttributeMap& extra) override;
    std::unordered_map<std::string, float> get_accuracy(const AttributeMap& extra) override;
    Vector3 get_linear_acceleration(const AttributeMap& extra) override;
    AttributeMap do_command(const AttributeMap& command) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

    using MovementSensor::get_accuracy;
    using MovementSensor::get_angular_velocity;
    using MovementSensor::get_compass_heading;
    using MovementSensor::get_geometries;
    using MovementSensor::get_linear_acceleration;
    using MovementSensor::get_linear_velocity;
    using MovementSensor::get_orientation;
    using MovementSensor::get_position;
    using MovementSensor::get_properties;

   private:
    using StubType = viam::component::movementsensor::v1::MovementSensorService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
