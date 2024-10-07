/// @file components/private/movement_sensor_client.hpp
///
/// @brief Implements a gRPC client for the `MovementSensor` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/movementsensor/v1/movementsensor.grpc.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/components/movement_sensor.hpp>
#include <viam/sdk/components/private/movement_sensor_server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class MovementSensorClient
/// @brief gRPC client implementation of a `MovementSensor` component.
/// @ingroup MovementSensor
class MovementSensorClient : public MovementSensor {
   public:
    using interface_type = MovementSensor;
    MovementSensorClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    Vector3 get_linear_velocity(const ProtoStruct& extra) override;
    Vector3 get_angular_velocity(const ProtoStruct& extra) override;
    compassheading get_compass_heading(const ProtoStruct& extra) override;
    orientation get_orientation(const ProtoStruct& extra) override;
    position get_position(const ProtoStruct& extra) override;
    properties get_properties(const ProtoStruct& extra) override;
    std::unordered_map<std::string, float> get_accuracy(const ProtoStruct& extra) override;
    Vector3 get_linear_acceleration(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;

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

}  // namespace impl
}  // namespace sdk
}  // namespace viam
