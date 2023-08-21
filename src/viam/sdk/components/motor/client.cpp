#include <viam/sdk/components/motor/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/motor/v1/motor.grpc.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

MotorClient::MotorClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Motor(std::move(name)),
      stub_(viam::component::motor::v1::MotorService::NewStub(channel)),
      channel_(std::move(channel)){};

void MotorClient::set_power(double power_pct) {
    return set_power(power_pct, nullptr);
}
void MotorClient::set_power(double power_pct, const AttributeMap& extra) {
    viam::component::motor::v1::SetPowerRequest request;
    viam::component::motor::v1::SetPowerResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);
    request.set_power_pct(power_pct);

    const grpc::Status status = stub_->SetPower(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

void MotorClient::go_for(double rpm, double revolutions) {
    return go_for(rpm, revolutions, nullptr);
}
void MotorClient::go_for(double rpm, double revolutions, const AttributeMap& extra) {
    viam::component::motor::v1::GoForRequest request;
    viam::component::motor::v1::GoForResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);
    request.set_rpm(rpm);
    request.set_revolutions(revolutions);

    const grpc::Status status = stub_->GoFor(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

void MotorClient::go_to(double rpm, double position_revolutions) {
    return go_to(rpm, position_revolutions, nullptr);
}
void MotorClient::go_to(double rpm, double position_revolutions, const AttributeMap& extra) {
    viam::component::motor::v1::GoToRequest request;
    viam::component::motor::v1::GoToResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);
    request.set_rpm(rpm);
    request.set_position_revolutions(position_revolutions);

    const grpc::Status status = stub_->GoTo(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

void MotorClient::reset_zero_position(double offset) {
    return reset_zero_position(offset, nullptr);
}
void MotorClient::reset_zero_position(double offset, const AttributeMap& extra) {
    viam::component::motor::v1::ResetZeroPositionRequest request;
    viam::component::motor::v1::ResetZeroPositionResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);
    request.set_offset(offset);

    const grpc::Status status = stub_->ResetZeroPosition(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

Motor::position MotorClient::get_position() {
    return get_position(nullptr);
}
Motor::position MotorClient::get_position(const AttributeMap& extra) {
    viam::component::motor::v1::GetPositionRequest request;
    viam::component::motor::v1::GetPositionResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetPosition(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
}

Motor::properties MotorClient::get_properties() {
    return get_properties(nullptr);
}
Motor::properties MotorClient::get_properties(const AttributeMap& extra) {
    viam::component::motor::v1::GetPropertiesRequest request;
    viam::component::motor::v1::GetPropertiesResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetProperties(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
}

grpc::StatusCode MotorClient::stop() {
    return stop(nullptr);
}
grpc::StatusCode MotorClient::stop(const AttributeMap& extra) {
    viam::component::motor::v1::StopRequest request;
    viam::component::motor::v1::StopResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->Stop(&ctx, request, &response);
    return status.error_code();
}

Motor::power_status MotorClient::get_power_status() {
    return get_power_status(nullptr);
}
Motor::power_status MotorClient::get_power_status(const AttributeMap& extra) {
    viam::component::motor::v1::IsPoweredRequest request;
    viam::component::motor::v1::IsPoweredResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->IsPowered(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
}

std::vector<GeometryConfig> MotorClient::get_geometries() {
    return get_geometries(nullptr);
}
std::vector<GeometryConfig> MotorClient::get_geometries(const AttributeMap& extra) {
    viam::common::v1::GetGeometriesRequest req;
    viam::common::v1::GetGeometriesResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_extra() = map_to_struct(extra);

    stub_->GetGeometries(&ctx, req, &resp);
    return GeometryConfig::from_proto(resp);
};

bool MotorClient::is_moving() {
    viam::component::motor::v1::IsMovingRequest request;
    viam::component::motor::v1::IsMovingResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    const grpc::Status status = stub_->IsMoving(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return response.is_moving();
}

AttributeMap MotorClient::do_command(const AttributeMap& command) {
    viam::common::v1::DoCommandRequest request;
    viam::common::v1::DoCommandResponse response;

    grpc::ClientContext ctx;

    *request.mutable_command() = map_to_struct(command);
    *request.mutable_name() = this->name();

    const grpc::Status status = stub_->DoCommand(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return struct_to_map(response.result());
}

}  // namespace sdk
}  // namespace viam
