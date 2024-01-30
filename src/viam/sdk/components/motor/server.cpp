#include <viam/sdk/components/motor/server.hpp>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

MotorServer::MotorServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)){};

::grpc::Status MotorServer::SetPower(
    ::grpc::ServerContext* context,
    const ::viam::component::motor::v1::SetPowerRequest* request,
    ::viam::component::motor::v1::SetPowerResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::SetPower", this, request)([&](auto& helper, auto& motor) {
        motor->set_power(request->power_pct(), helper.getExtra());
    });
}

::grpc::Status MotorServer::GoFor(::grpc::ServerContext* context,
                                  const ::viam::component::motor::v1::GoForRequest* request,
                                  ::viam::component::motor::v1::GoForResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GoFor", this, request)([&](auto& helper, auto& motor) {
        motor->go_for(request->rpm(), request->revolutions(), helper.getExtra());
    });
}

::grpc::Status MotorServer::GoTo(::grpc::ServerContext* context,
                                 const ::viam::component::motor::v1::GoToRequest* request,
                                 ::viam::component::motor::v1::GoToResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GoTo", this, request)([&](auto& helper, auto& motor) {
        motor->go_to(request->rpm(), request->position_revolutions(), helper.getExtra());
    });
}

::grpc::Status MotorServer::ResetZeroPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::motor::v1::ResetZeroPositionRequest* request,
    ::viam::component::motor::v1::ResetZeroPositionResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::ResetZeroPosition", this, request)([&](auto& helper, auto& motor) {
        motor->reset_zero_position(request->offset(), helper.getExtra());
    });
}

::grpc::Status MotorServer::GetPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::motor::v1::GetPositionRequest* request,
    ::viam::component::motor::v1::GetPositionResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GetPosition", this, request)([&](auto& helper, auto& motor) {
        const Motor::position result = motor->get_position(helper.getExtra());
        response->set_position(result);
    });
}

::grpc::Status MotorServer::GetProperties(
    ::grpc::ServerContext* context,
    const ::viam::component::motor::v1::GetPropertiesRequest* request,
    ::viam::component::motor::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GetProperties", this, request)([&](auto& helper, auto& motor) {
        const Motor::properties result = motor->get_properties(helper.getExtra());
        response->set_position_reporting(result.position_reporting);
    });
}

::grpc::Status MotorServer::Stop(::grpc::ServerContext* context,
                                 const ::viam::component::motor::v1::StopRequest* request,
                                 ::viam::component::motor::v1::StopResponse* response) noexcept {
    return make_service_helper<Motor>("MotorServer::Stop", this, request)(
        [&](auto& helper, auto& motor) { motor->stop(helper.getExtra()); });
}

::grpc::Status MotorServer::IsPowered(
    ::grpc::ServerContext* context,
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
    ::grpc::ServerContext* context,
    const ::viam::component::motor::v1::IsMovingRequest* request,
    ::viam::component::motor::v1::IsMovingResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::IsMoving", this, request)([&](auto& helper, auto& motor) {
        const bool result = motor->is_moving();
        response->set_is_moving(result);
    });
}

::grpc::Status MotorServer::GetGeometries(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GetGeometries", this, request)([&](auto& helper, auto& motor) {
        const std::vector<GeometryConfig> geometries = motor->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

::grpc::Status MotorServer::DoCommand(grpc::ServerContext* context,
                                      const viam::common::v1::DoCommandRequest* request,
                                      viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Motor>(
        "MotorServer::GetGeometries", this, request)([&](auto& helper, auto& motor) {
        const AttributeMap result = motor->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

API MotorServer::api() const {
    return API::get<Motor>();
}

}  // namespace sdk
}  // namespace viam
