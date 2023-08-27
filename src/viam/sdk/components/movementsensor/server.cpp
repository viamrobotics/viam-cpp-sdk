#include <viam/sdk/components/movementsensor/server.hpp>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/movementsensor/movementsensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

MovementSensorServer::MovementSensorServer()
    : ResourceServer(std::make_shared<ResourceManager>()){};
MovementSensorServer::MovementSensorServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(manager){};

::grpc::Status MovementSensorServer::GetLinearVelocity(
    ::grpc::ServerContext* context,
    const ::viam::component::movementsensor::v1::GetLinearVelocityRequest* request,
    ::viam::component::movementsensor::v1::GetLinearVelocityResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MovementSensor::GetLinearVelocity] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<MovementSensor> movementsensor =
        std::dynamic_pointer_cast<MovementSensor>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    Vector3 result = movementsensor->get_linear_velocity(extra);
    *response->mutable_linear_velocity() = Vector3::to_proto(result);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetAngularVelocity(
    ::grpc::ServerContext* context,
    const ::viam::component::movementsensor::v1::GetAngularVelocityRequest* request,
    ::viam::component::movementsensor::v1::GetAngularVelocityResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MovementSensor::GetAngularVelocity] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<MovementSensor> movementsensor =
        std::dynamic_pointer_cast<MovementSensor>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    Vector3 result = movementsensor->get_angular_velocity(extra);
    *response->mutable_angular_velocity() = Vector3::to_proto(result);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetCompassHeading(
    ::grpc::ServerContext* context,
    const ::viam::component::movementsensor::v1::GetCompassHeadingRequest* request,
    ::viam::component::movementsensor::v1::GetCompassHeadingResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MovementSensor::GetCompassHeading] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<MovementSensor> movementsensor =
        std::dynamic_pointer_cast<MovementSensor>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    MovementSensor::compassheading result = movementsensor->get_compass_heading(extra);
    response->set_value(result.value);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetOrientation(
    ::grpc::ServerContext* context,
    const ::viam::component::movementsensor::v1::GetOrientationRequest* request,
    ::viam::component::movementsensor::v1::GetOrientationResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MovementSensor::GetOrientation] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<MovementSensor> movementsensor =
        std::dynamic_pointer_cast<MovementSensor>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    MovementSensor::orientation result = movementsensor->get_orientation(extra);
    *response->mutable_orientation() = MovementSensor::to_proto(result);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::movementsensor::v1::GetPositionRequest* request,
    ::viam::component::movementsensor::v1::GetPositionResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MovementSensor::GetPosition] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<MovementSensor> movementsensor =
        std::dynamic_pointer_cast<MovementSensor>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    MovementSensor::position result = movementsensor->get_position(extra);
    *response->mutable_coordinate() = result.coordinate.to_proto();
    response->set_altitude_m(result.altitude_m);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetProperties(
    ::grpc::ServerContext* context,
    const ::viam::component::movementsensor::v1::GetPropertiesRequest* request,
    ::viam::component::movementsensor::v1::GetPropertiesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MovementSensor::GetProperties] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<MovementSensor> movementsensor =
        std::dynamic_pointer_cast<MovementSensor>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    MovementSensor::properties result = movementsensor->get_properties(extra);
    response->set_linear_velocity_supported(result.linear_velocity_supported);
    response->set_angular_velocity_supported(result.angular_velocity_supported);
    response->set_orientation_supported(result.orientation_supported);
    response->set_position_supported(result.position_supported);
    response->set_compass_heading_supported(result.compass_heading_supported);
    response->set_linear_acceleration_supported(result.linear_acceleration_supported);

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetAccuracy(
    ::grpc::ServerContext* context,
    const ::viam::component::movementsensor::v1::GetAccuracyRequest* request,
    ::viam::component::movementsensor::v1::GetAccuracyResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MovementSensor::GetAccuracy] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<MovementSensor> movementsensor =
        std::dynamic_pointer_cast<MovementSensor>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    std::unordered_map<std::string, float> result = movementsensor->get_accuracy(extra);
    response->mutable_accuracy()->empty();
    for (const auto& i : result) {
        response->mutable_accuracy()->emplace(i.first, i.second);
    }

    return ::grpc::Status();
}

::grpc::Status MovementSensorServer::GetLinearAcceleration(
    ::grpc::ServerContext* context,
    const ::viam::component::movementsensor::v1::GetLinearAccelerationRequest* request,
    ::viam::component::movementsensor::v1::GetLinearAccelerationResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MovementSensor::GetLinearAcceleration] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<MovementSensor> movementsensor =
        std::dynamic_pointer_cast<MovementSensor>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    Vector3 result = movementsensor->get_linear_acceleration(extra);
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
