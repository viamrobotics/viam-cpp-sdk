#include <viam/sdk/components/private/motor_server.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {
namespace impl {

MotorServer::MotorServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status MotorServer::SetPower(::grpc::ServerContext*,
                                     const ::viam::component::motor::v1::SetPowerRequest* request,
                                     ::viam::component::motor::v1::SetPowerResponse*) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::SetPower", this, request)([&](auto& helper, auto& motor) {
        motor->set_power(request->power_pct(), helper.getExtra());
    });
}

::grpc::Status MotorServer::GoFor(::grpc::ServerContext*,
                                  const ::viam::component::motor::v1::GoForRequest* request,
                                  ::viam::component::motor::v1::GoForResponse*) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GoFor", this, request)([&](auto& helper, auto& motor) {
        motor->go_for(request->rpm(), request->revolutions(), helper.getExtra());
    });
}

::grpc::Status MotorServer::GoTo(::grpc::ServerContext*,
                                 const ::viam::component::motor::v1::GoToRequest* request,
                                 ::viam::component::motor::v1::GoToResponse*) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GoTo", this, request)([&](auto& helper, auto& motor) {
        motor->go_to(request->rpm(), request->position_revolutions(), helper.getExtra());
    });
}

::grpc::Status MotorServer::SetRPM(::grpc::ServerContext*,
                                   const ::viam::component::motor::v1::SetRPMRequest* request,
                                   ::viam::component::motor::v1::SetRPMResponse*) noexcept {
    return make_service_helper<Motor>("MotorServer::SetRPM", this, request)(
        [&](auto& helper, auto& motor) { motor->set_rpm(request->rpm(), helper.getExtra()); });
}

::grpc::Status MotorServer::ResetZeroPosition(
    ::grpc::ServerContext*,
    const ::viam::component::motor::v1::ResetZeroPositionRequest* request,
    ::viam::component::motor::v1::ResetZeroPositionResponse*) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::ResetZeroPosition", this, request)([&](auto& helper, auto& motor) {
        motor->reset_zero_position(request->offset(), helper.getExtra());
    });
}

::grpc::Status MotorServer::GetPosition(
    ::grpc::ServerContext*,
    const ::viam::component::motor::v1::GetPositionRequest* request,
    ::viam::component::motor::v1::GetPositionResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GetPosition", this, request)([&](auto& helper, auto& motor) {
        const Motor::position result = motor->get_position(helper.getExtra());
        response->set_position(result);
    });
}

::grpc::Status MotorServer::GetProperties(
    ::grpc::ServerContext*,
    const ::viam::component::motor::v1::GetPropertiesRequest* request,
    ::viam::component::motor::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GetProperties", this, request)([&](auto& helper, auto& motor) {
        const Motor::properties result = motor->get_properties(helper.getExtra());
        response->set_position_reporting(result.position_reporting);
    });
}

::grpc::Status MotorServer::Stop(::grpc::ServerContext*,
                                 const ::viam::component::motor::v1::StopRequest* request,
                                 ::viam::component::motor::v1::StopResponse*) noexcept {
    return make_service_helper<Motor>("MotorServer::Stop", this, request)(
        [&](auto& helper, auto& motor) { motor->stop(helper.getExtra()); });
}

::grpc::Status MotorServer::IsPowered(
    ::grpc::ServerContext*,
    const ::viam::component::motor::v1::IsPoweredRequest* request,
    ::viam::component::motor::v1::IsPoweredResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::IsPowered", this, request)([&](auto& helper, auto& motor) {
        const Motor::power_status result = motor->get_power_status(helper.getExtra());
        response->set_is_on(result.is_on);
        response->set_power_pct(result.power_pct);
    });
}

::grpc::Status MotorServer::IsMoving(
    ::grpc::ServerContext*,
    const ::viam::component::motor::v1::IsMovingRequest* request,
    ::viam::component::motor::v1::IsMovingResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::IsMoving", this, request)([&](auto&, auto& motor) {
        const bool result = motor->is_moving();
        response->set_is_moving(result);
    });
}

::grpc::Status MotorServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GetGeometries", this, request)([&](auto& helper, auto& motor) {
        const std::vector<GeometryConfig> geometries = motor->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = v2::to_proto(geometry);
        }
    });
}

::grpc::Status MotorServer::DoCommand(grpc::ServerContext*,
                                      const viam::common::v1::DoCommandRequest* request,
                                      viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GetGeometries", this, request)([&](auto&, auto& motor) {
        const ProtoStruct result = motor->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
