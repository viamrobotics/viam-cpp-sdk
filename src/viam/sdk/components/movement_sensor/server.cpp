#include <viam/sdk/components/movement_sensor/server.hpp>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/service_helper.hpp>
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
    return make_service_helper<MovementSensor>("MovementSensorServer::GetLinearVelocity",
                                               this,
                                               request)([&](auto& helper, auto& movementsensor) {
        const Vector3 result = movementsensor->get_linear_velocity(helper.getExtra());
        *response->mutable_linear_velocity() = Vector3::to_proto(result);
    });
}

::grpc::Status MovementSensorServer::GetAngularVelocity(::grpc::ServerContext* context,
                                                        const GetAngularVelocityRequest* request,
                                                        GetAngularVelocityResponse* response) {
    return make_service_helper<MovementSensor>("MovementSensorServer::GetAngularVelocity",
                                               this,
                                               request)([&](auto& helper, auto& movementsensor) {
        const Vector3 result = movementsensor->get_angular_velocity(helper.getExtra());
        *response->mutable_angular_velocity() = Vector3::to_proto(result);
    });
}

::grpc::Status MovementSensorServer::GetCompassHeading(::grpc::ServerContext* context,
                                                       const GetCompassHeadingRequest* request,
                                                       GetCompassHeadingResponse* response) {
    return make_service_helper<MovementSensor>("MovementSensorServer::GetCompassHeading",
                                               this,
                                               request)([&](auto& helper, auto& movementsensor) {
        const MovementSensor::compassheading result =
            movementsensor->get_compass_heading(helper.getExtra());
        response->set_value(result.value);
    });
}

::grpc::Status MovementSensorServer::GetOrientation(::grpc::ServerContext* context,
                                                    const GetOrientationRequest* request,
                                                    GetOrientationResponse* response) {
    return make_service_helper<MovementSensor>("MovementSensorServer::GetOrientation",
                                               this,
                                               request)([&](auto& helper, auto& movementsensor) {
        const MovementSensor::orientation result =
            movementsensor->get_orientation(helper.getExtra());
        *response->mutable_orientation() = MovementSensor::to_proto(result);
    });
}

::grpc::Status MovementSensorServer::GetPosition(::grpc::ServerContext* context,
                                                 const GetPositionRequest* request,
                                                 GetPositionResponse* response) {
    return make_service_helper<MovementSensor>("MovementSensorServer::GetPosition", this, request)(
        [&](auto& helper, auto& movementsensor) {
            const MovementSensor::position result = movementsensor->get_position(helper.getExtra());
            *response->mutable_coordinate() = result.coordinate.to_proto();
            response->set_altitude_m(result.altitude_m);
        });
}

::grpc::Status MovementSensorServer::GetProperties(::grpc::ServerContext* context,
                                                   const GetPropertiesRequest* request,
                                                   GetPropertiesResponse* response) {
    return make_service_helper<MovementSensor>("MovementSensorServer::GetProperties",
                                               this,
                                               request)([&](auto& helper, auto& movementsensor) {
        const MovementSensor::properties result = movementsensor->get_properties(helper.getExtra());
        response->set_linear_velocity_supported(result.linear_velocity_supported);
        response->set_angular_velocity_supported(result.angular_velocity_supported);
        response->set_orientation_supported(result.orientation_supported);
        response->set_position_supported(result.position_supported);
        response->set_compass_heading_supported(result.compass_heading_supported);
        response->set_linear_acceleration_supported(result.linear_acceleration_supported);
    });
}

::grpc::Status MovementSensorServer::GetAccuracy(::grpc::ServerContext* context,
                                                 const GetAccuracyRequest* request,
                                                 GetAccuracyResponse* response) {
    return make_service_helper<MovementSensor>("MovementSensorServer::GetAccuracy", this, request)(
        [&](auto& helper, auto& movementsensor) {
            const std::unordered_map<std::string, float> result =
                movementsensor->get_accuracy(helper.getExtra());
            for (const auto& i : result) {
                response->mutable_accuracy()->insert({i.first, i.second});
            }
        });
}

::grpc::Status MovementSensorServer::GetLinearAcceleration(
    ::grpc::ServerContext* context,
    const GetLinearAccelerationRequest* request,
    GetLinearAccelerationResponse* response) {
    return make_service_helper<MovementSensor>("MovementSensorServer::GetLinearAcceleration",
                                               this,
                                               request)([&](auto& helper, auto& movementsensor) {
        const Vector3 result = movementsensor->get_linear_acceleration(helper.getExtra());
        *response->mutable_linear_acceleration() = Vector3::to_proto(result);
    });
}

::grpc::Status MovementSensorServer::DoCommand(grpc::ServerContext* context,
                                               const viam::common::v1::DoCommandRequest* request,
                                               viam::common::v1::DoCommandResponse* response) {
    return make_service_helper<MovementSensor>(
        "MovementSensorServer::DoCommand", this, request)([&](auto& helper, auto& movementsensor) {
        const AttributeMap result = movementsensor->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

::grpc::Status MovementSensorServer::GetGeometries(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) {
    return make_service_helper<MovementSensor>("MovementSensorServer::GetGeometries",
                                               this,
                                               request)([&](auto& helper, auto& movementsensor) {
        const std::vector<GeometryConfig> geometries =
            movementsensor->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

void MovementSensorServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
