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

/// helper to throw on !ok grpc status
void throw_status(const ::grpc::Status& status) {
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

using namespace viam::component::movementsensor::v1;

Vector3 MovementSensorClient::get_linear_velocity(const AttributeMap& extra) {
    // viam::component::movementsensor::v1::GetLinearVelocityRequest request;
    // viam::component::movementsensor::v1::GetLinearVelocityResponse response;

    // grpc::ClientContext ctx;

    // *request.mutable_name() = this->name();
    // *request.mutable_extra() = map_to_struct(extra);

    // const grpc::Status status = stub_->GetLinearVelocity(&ctx, request, &response);
    // if (!status.ok()) {
    //     throw std::runtime_error(status.error_message());
    // }
    GetLinearVelocityResponse response;
    stub_wrapper<GetLinearVelocityRequest>(this, response, extra, &MovementSensorClient::Stub::GetLinearVelocity);
    return Vector3::from_proto(response.linear_velocity());
}

Vector3 MovementSensorClient::get_angular_velocity(const AttributeMap& extra) {
    // GetAngularVelocityRequest request;
    GetAngularVelocityResponse response;
    stub_wrapper<GetAngularVelocityRequest>(this, response, extra, &MovementSensorClient::Stub::GetAngularVelocity);

    // grpc::ClientContext ctx;

    // *request.mutable_name() = this->name();
    // *request.mutable_extra() = map_to_struct(extra);

    // const grpc::Status status = stub_->GetAngularVelocity(&ctx, request, &response);
    // if (!status.ok()) {
    //     throw std::runtime_error(status.error_message());
    // }
    return Vector3::from_proto(response.angular_velocity());
}

MovementSensor::compassheading MovementSensorClient::get_compass_heading(
    const AttributeMap& extra) {
    // GetCompassHeadingRequest request;
    GetCompassHeadingResponse response;

    // grpc::ClientContext ctx;

    // *request.mutable_name() = this->name();
    // *request.mutable_extra() = map_to_struct(extra);

    // const grpc::Status status = stub_->GetCompassHeading(&ctx, request, &response);
    // if (!status.ok()) {
    //     throw std::runtime_error(status.error_message());
    // }
    stub_wrapper<GetCompassHeadingRequest>(this, response, extra, &MovementSensorClient::Stub::GetCompassHeading);
    return from_proto(response);
}

MovementSensor::orientation MovementSensorClient::get_orientation(const AttributeMap& extra) {
    // GetOrientationRequest request;
    GetOrientationResponse response;

    // grpc::ClientContext ctx;

    // *request.mutable_name() = this->name();
    // *request.mutable_extra() = map_to_struct(extra);

    // const grpc::Status status = stub_->GetOrientation(&ctx, request, &response);
    // if (!status.ok()) {
    //     throw std::runtime_error(status.error_message());
    // }
    stub_wrapper<GetOrientationRequest>(this, response, extra, &MovementSensorClient::Stub::GetOrientation);
    return from_proto(response.orientation());
}

MovementSensor::position MovementSensorClient::get_position(const AttributeMap& extra) {
    // GetPositionRequest request;
    GetPositionResponse response;

    // grpc::ClientContext ctx;

    // *request.mutable_name() = this->name();
    // *request.mutable_extra() = map_to_struct(extra);

    // const grpc::Status status = stub_->GetPosition(&ctx, request, &response);
    // if (!status.ok()) {
    //     throw std::runtime_error(status.error_message());
    // }
    stub_wrapper<GetPositionRequest>(this, response, extra, &MovementSensorClient::Stub::GetPosition);
    return from_proto(response);
}

MovementSensor::properties MovementSensorClient::get_properties(const AttributeMap& extra) {
    // GetPropertiesRequest request;
    GetPropertiesResponse response;

    // grpc::ClientContext ctx;

    // *request.mutable_name() = this->name();
    // *request.mutable_extra() = map_to_struct(extra);

    // const grpc::Status status = stub_->GetProperties(&ctx, request, &response);
    // if (!status.ok()) {
    //     throw std::runtime_error(status.error_message());
    // }
    stub_wrapper<GetPropertiesRequest>(this, response, extra, &MovementSensorClient::Stub::GetProperties);
    return from_proto(response);
}

std::unordered_map<std::string, float> MovementSensorClient::get_accuracy(
    const AttributeMap& extra) {
    // GetAccuracyRequest request;
    GetAccuracyResponse response;

    // grpc::ClientContext ctx;

    // *request.mutable_name() = this->name();
    // *request.mutable_extra() = map_to_struct(extra);

    // const grpc::Status status = stub_->GetAccuracy(&ctx, request, &response);
    // if (!status.ok()) {
    //     throw std::runtime_error(status.error_message());
    // }
    stub_wrapper<GetAccuracyRequest>(this, response, extra, &MovementSensorClient::Stub::GetAccuracy);
    std::unordered_map<std::string, float> result;

    for (const auto& i : response.accuracy()) {
        result.emplace(i.first, i.second);
    }
    return result;
}

Vector3 MovementSensorClient::get_linear_acceleration(const AttributeMap& extra) {
    // GetLinearAccelerationRequest request;
    GetLinearAccelerationResponse response;

    // grpc::ClientContext ctx;

    // *request.mutable_name() = this->name();
    // *request.mutable_extra() = map_to_struct(extra);

    // const grpc::Status status = stub_->GetLinearAcceleration(&ctx, request, &response);
    // if (!status.ok()) {
    //     throw std::runtime_error(status.error_message());
    // }
    stub_wrapper<GetLinearAccelerationRequest>(this, response, extra, &MovementSensorClient::Stub::GetLinearAcceleration);
    return Vector3::from_proto(response.linear_acceleration());
}

AttributeMap MovementSensorClient::do_command(const AttributeMap& command) {
    viam::common::v1::DoCommandRequest request;
    viam::common::v1::DoCommandResponse response;

    grpc::ClientContext ctx;

    const google::protobuf::Struct proto_command = map_to_struct(command);
    *request.mutable_command() = proto_command;
    *request.mutable_name() = this->name();

    throw_status(stub_->DoCommand(&ctx, request, &response));
    return struct_to_map(response.result());
}

// TODO this one didn't have a throw. is that intentional?
std::vector<GeometryConfig> MovementSensorClient::get_geometries(const AttributeMap& extra) {
    // viam::common::v1::GetGeometriesRequest req;
    viam::common::v1::GetGeometriesResponse resp;
    // grpc::ClientContext ctx;

    // *req.mutable_name() = this->name();
    // *req.mutable_extra() = map_to_struct(extra);

    // stub_->GetGeometries(&ctx, req, &resp);
    stub_wrapper<viam::common::v1::GetGeometriesRequest>(this, resp, extra, &MovementSensorClient::Stub::GetGeometries);
    return GeometryConfig::from_proto(resp);
}

}  // namespace sdk
}  // namespace viam
