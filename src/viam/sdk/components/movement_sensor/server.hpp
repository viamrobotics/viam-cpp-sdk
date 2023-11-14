/// @file components/movement_sensor/server.hpp
///
/// @brief Implements a gRPC server for the `MovementSensor` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/movementsensor/v1/movementsensor.grpc.pb.h>

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
    MovementSensorServer();
    explicit MovementSensorServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetLinearVelocity(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetLinearVelocityRequest* request,
        ::viam::component::movementsensor::v1::GetLinearVelocityResponse* response) override;

    ::grpc::Status GetAngularVelocity(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetAngularVelocityRequest* request,
        ::viam::component::movementsensor::v1::GetAngularVelocityResponse* response) override;

    ::grpc::Status GetCompassHeading(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetCompassHeadingRequest* request,
        ::viam::component::movementsensor::v1::GetCompassHeadingResponse* response) override;

    ::grpc::Status GetOrientation(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetOrientationRequest* request,
        ::viam::component::movementsensor::v1::GetOrientationResponse* response) override;

    ::grpc::Status GetPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetPositionRequest* request,
        ::viam::component::movementsensor::v1::GetPositionResponse* response) override;

    ::grpc::Status GetProperties(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetPropertiesRequest* request,
        ::viam::component::movementsensor::v1::GetPropertiesResponse* response) override;

    ::grpc::Status GetAccuracy(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetAccuracyRequest* request,
        ::viam::component::movementsensor::v1::GetAccuracyResponse* response) override;

    ::grpc::Status GetLinearAcceleration(
        ::grpc::ServerContext* context,
        const ::viam::component::movementsensor::v1::GetLinearAccelerationRequest* request,
        ::viam::component::movementsensor::v1::GetLinearAccelerationResponse* response) override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const viam::common::v1::DoCommandRequest* request,
                             viam::common::v1::DoCommandResponse* response) override;

    ::grpc::Status GetGeometries(::grpc::ServerContext* context,
                                 const ::viam::common::v1::GetGeometriesRequest* request,
                                 ::viam::common::v1::GetGeometriesResponse* response) override;

    void register_server(std::shared_ptr<Server> server) override;
};

}  // namespace sdk
}  // namespace viam
