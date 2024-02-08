/// @file components/movement_sensor/server.hpp
///
/// @brief Implements a gRPC server for the `MovementSensor` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/movementsensor/v1/movementsensor.grpc.pb.h>

#include <viam/sdk/components/movement_sensor/movement_sensor.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {

/// @class MovementSensorServer
/// @brief gRPC server implementation of a `MovementSensor` component.
/// @ingroup MovementSensor
class MovementSensorServer
    : public ResourceServer,
      public viam::component::movementsensor::v1::MovementSensorService::Service {
   public:
    using interface_type = MovementSensor;
    using service_type = component::movementsensor::v1::MovementSensorService;
    explicit MovementSensorServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetLinearVelocity(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetLinearVelocityRequest* request,
        ::viam::component::movementsensor::v1::GetLinearVelocityResponse* response) noexcept
        override;

    ::grpc::Status GetAngularVelocity(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetAngularVelocityRequest* request,
        ::viam::component::movementsensor::v1::GetAngularVelocityResponse* response) noexcept
        override;

    ::grpc::Status GetCompassHeading(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetCompassHeadingRequest* request,
        ::viam::component::movementsensor::v1::GetCompassHeadingResponse* response) noexcept
        override;

    ::grpc::Status GetOrientation(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetOrientationRequest* request,
        ::viam::component::movementsensor::v1::GetOrientationResponse* response) noexcept override;

    ::grpc::Status GetPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetPositionRequest* request,
        ::viam::component::movementsensor::v1::GetPositionResponse* response) noexcept override;

    ::grpc::Status GetProperties(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetPropertiesRequest* request,
        ::viam::component::movementsensor::v1::GetPropertiesResponse* response) noexcept override;

    ::grpc::Status GetAccuracy(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetAccuracyRequest* request,
        ::viam::component::movementsensor::v1::GetAccuracyResponse* response) noexcept override;

    ::grpc::Status GetLinearAcceleration(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetLinearAccelerationRequest* request,
        ::viam::component::movementsensor::v1::GetLinearAccelerationResponse* response) noexcept
        override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const viam::common::v1::DoCommandRequest* request,
                             viam::common::v1::DoCommandResponse* response) noexcept override;

    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;
};

}  // namespace sdk
}  // namespace viam
