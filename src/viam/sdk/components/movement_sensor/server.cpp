#include <viam/sdk/components/movement_sensor/server.hpp>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/movement_sensor/movement_sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

using namespace viam::component::movementsensor::v1;

namespace viam {
namespace sdk {

MovementSensorServer::MovementSensorServer()
    : ResourceServer(std::make_shared<ResourceManager>()){};
MovementSensorServer::MovementSensorServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(manager){};

::grpc::Status MovementSensorServer::GetLinearVelocity(::grpc::ServerContext* context,
                                                       const GetLinearVelocityRequest* request,
                                                       GetLinearVelocityResponse* response) {
    const auto status = server_wrapper<MovementSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const Vector3 result = status.movementsensor->get_linear_velocity(status.extra);
    *response->mutable_linear_velocity() = Vector3::to_proto(result);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetAngularVelocity(::grpc::ServerContext* context,
                                                        const GetAngularVelocityRequest* request,
                                                        GetAngularVelocityResponse* response) {
    const auto status = server_wrapper<MovementSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const Vector3 result = status.movementsensor->get_angular_velocity(status.extra);
    *response->mutable_angular_velocity() = Vector3::to_proto(result);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetCompassHeading(::grpc::ServerContext* context,
                                                       const GetCompassHeadingRequest* request,
                                                       GetCompassHeadingResponse* response) {
    const auto status = server_wrapper<MovementSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const MovementSensor::compassheading result =
        status.movementsensor->get_compass_heading(status.extra);
    response->set_value(result.value);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetOrientation(::grpc::ServerContext* context,
                                                    const GetOrientationRequest* request,
                                                    GetOrientationResponse* response) {
    const auto status = server_wrapper<MovementSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const MovementSensor::orientation result = status.movementsensor->get_orientation(status.extra);
    *response->mutable_orientation() = MovementSensor::to_proto(result);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetPosition(::grpc::ServerContext* context,
                                                 const GetPositionRequest* request,
                                                 GetPositionResponse* response) {
    const auto status = server_wrapper<MovementSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const MovementSensor::position result = status.movementsensor->get_position(status.extra);
    *response->mutable_coordinate() = result.coordinate.to_proto();
    response->set_altitude_m(result.altitude_m);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetProperties(::grpc::ServerContext* context,
                                                   const GetPropertiesRequest* request,
                                                   GetPropertiesResponse* response) {
    const auto status = server_wrapper<MovementSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const MovementSensor::properties result = status.movementsensor->get_properties(status.extra);
    response->set_linear_velocity_supported(result.linear_velocity_supported);
    response->set_angular_velocity_supported(result.angular_velocity_supported);
    response->set_orientation_supported(result.orientation_supported);
    response->set_position_supported(result.position_supported);
    response->set_compass_heading_supported(result.compass_heading_supported);
    response->set_linear_acceleration_supported(result.linear_acceleration_supported);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetAccuracy(::grpc::ServerContext* context,
                                                 const GetAccuracyRequest* request,
                                                 GetAccuracyResponse* response) {
    const auto status = server_wrapper<MovementSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const std::unordered_map<std::string, float> result =
        status.movementsensor->get_accuracy(status.extra);
    response->mutable_accuracy()->empty();
    for (const auto& i : result) {
        response->mutable_accuracy()->insert({i.first, i.second});
    }

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetLinearAcceleration(
    ::grpc::ServerContext* context,
    const GetLinearAccelerationRequest* request,
    GetLinearAccelerationResponse* response) {
    const auto status = server_wrapper<MovementSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const Vector3 result = status.movementsensor->get_linear_acceleration(status.extra);
    *response->mutable_linear_acceleration() = Vector3::to_proto(result);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::DoCommand(grpc::ServerContext* context,
                                               const viam::common::v1::DoCommandRequest* request,
                                               viam::common::v1::DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MovementSensor::DoCommand] without a request");
    };

    auto rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<MovementSensor> sensor = std::dynamic_pointer_cast<MovementSensor>(rb);
    const AttributeMap result = sensor->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetGeometries(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MovementSensor::GetGeometries] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const std::shared_ptr<MovementSensor> sensor = std::dynamic_pointer_cast<MovementSensor>(rb);
    const std::vector<GeometryConfig> geometries = sensor->get_geometries(extra);
    for (const auto& geometry : geometries) {
        *response->mutable_geometries()->Add() = geometry.to_proto();
    }

    return ::grpc::Status();
}

void MovementSensorServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
