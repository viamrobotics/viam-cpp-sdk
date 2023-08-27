#include <viam/sdk/components/movementsensor/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/movementsensor/v1/movementsensor.grpc.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/movementsensor/movementsensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

MovementSensorClient::MovementSensorClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : MovementSensor(std::move(name)),
      stub_(viam::component::movementsensor::v1::MovementSensorService::NewStub(channel)),
      channel_(std::move(channel)){};

Vector3 MovementSensorClient::get_linear_velocity(const AttributeMap& extra) {
    viam::component::movementsensor::v1::GetLinearVelocityRequest request;
    viam::component::movementsensor::v1::GetLinearVelocityResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetLinearVelocity(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return Vector3::from_proto(response.linear_velocity());
}

Vector3 MovementSensorClient::get_angular_velocity(const AttributeMap& extra) {
    viam::component::movementsensor::v1::GetAngularVelocityRequest request;
    viam::component::movementsensor::v1::GetAngularVelocityResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetAngularVelocity(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return Vector3::from_proto(response.angular_velocity());
}

MovementSensor::compassheading MovementSensorClient::get_compass_heading(
    const AttributeMap& extra) {
    viam::component::movementsensor::v1::GetCompassHeadingRequest request;
    viam::component::movementsensor::v1::GetCompassHeadingResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetCompassHeading(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
}

MovementSensor::orientation MovementSensorClient::get_orientation(const AttributeMap& extra) {
    viam::component::movementsensor::v1::GetOrientationRequest request;
    viam::component::movementsensor::v1::GetOrientationResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetOrientation(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response.orientation());
}

MovementSensor::position MovementSensorClient::get_position(const AttributeMap& extra) {
    viam::component::movementsensor::v1::GetPositionRequest request;
    viam::component::movementsensor::v1::GetPositionResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetPosition(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
}

MovementSensor::properties MovementSensorClient::get_properties(const AttributeMap& extra) {
    viam::component::movementsensor::v1::GetPropertiesRequest request;
    viam::component::movementsensor::v1::GetPropertiesResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetProperties(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
}

std::unordered_map<std::string, float> MovementSensorClient::get_accuracy(
    const AttributeMap& extra) {
    viam::component::movementsensor::v1::GetAccuracyRequest request;
    viam::component::movementsensor::v1::GetAccuracyResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetAccuracy(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    std::unordered_map<std::string, float> result;

    for (const auto& i : response.accuracy()) {
        result.emplace(i.first, i.second);
    }
    return result;
}

Vector3 MovementSensorClient::get_linear_acceleration(const AttributeMap& extra) {
    viam::component::movementsensor::v1::GetLinearAccelerationRequest request;
    viam::component::movementsensor::v1::GetLinearAccelerationResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetLinearAcceleration(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return Vector3::from_proto(response.linear_acceleration());
}

AttributeMap MovementSensorClient::do_command(const AttributeMap& command) {
    viam::common::v1::DoCommandRequest request;
    viam::common::v1::DoCommandResponse response;

    grpc::ClientContext ctx;

    const google::protobuf::Struct proto_command = map_to_struct(command);
    *request.mutable_command() = proto_command;
    *request.mutable_name() = this->name();

    const grpc::Status status = stub_->DoCommand(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return struct_to_map(response.result());
}

std::vector<GeometryConfig> MovementSensorClient::get_geometries(const AttributeMap& extra) {
    viam::common::v1::GetGeometriesRequest req;
    viam::common::v1::GetGeometriesResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_extra() = map_to_struct(extra);

    stub_->GetGeometries(&ctx, req, &resp);
    return GeometryConfig::from_proto(resp);
}

}  // namespace sdk
}  // namespace viam
