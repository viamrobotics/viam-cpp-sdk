#include <viam/sdk/components/private/servo_server.hpp>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/servo.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {
namespace impl {

ServoServer::ServoServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {};

::grpc::Status ServoServer::Move(::grpc::ServerContext*,
                                 const ::viam::component::servo::v1::MoveRequest* request,
                                 ::viam::component::servo::v1::MoveResponse*) noexcept {
    return make_service_helper<Servo>("ServoServer::Move", this, request)(
        [&](auto& helper, auto& servo) { servo->move(request->angle_deg(), helper.getExtra()); });
}

::grpc::Status ServoServer::GetPosition(
    ::grpc::ServerContext*,
    const ::viam::component::servo::v1::GetPositionRequest* request,
    ::viam::component::servo::v1::GetPositionResponse* response) noexcept {
    return make_service_helper<Servo>(
        "ServoServer::GetPosition", this, request)([&](auto& helper, auto& servo) {
        const Servo::position result = servo->get_position(helper.getExtra());
        response->set_position_deg(result);
    });
}

::grpc::Status ServoServer::Stop(::grpc::ServerContext*,
                                 const ::viam::component::servo::v1::StopRequest* request,
                                 ::viam::component::servo::v1::StopResponse*) noexcept {
    return make_service_helper<Servo>("ServoServer::Stop", this, request)(
        [&](auto& helper, auto& servo) { servo->stop(helper.getExtra()); });
}

::grpc::Status ServoServer::IsMoving(
    ::grpc::ServerContext*,
    const ::viam::component::servo::v1::IsMovingRequest* request,
    ::viam::component::servo::v1::IsMovingResponse* response) noexcept {
    return make_service_helper<Servo>(
        "ServoServer::IsMoving", this, request)([&](auto&, auto& servo) {
        const bool result = servo->is_moving();
        response->set_is_moving(result);
    });
}

::grpc::Status ServoServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Servo>(
        "ServoServer::GetGeometries", this, request)([&](auto& helper, auto& servo) {
        const std::vector<GeometryConfig> geometries = servo->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

::grpc::Status ServoServer::DoCommand(grpc::ServerContext*,
                                      const viam::common::v1::DoCommandRequest* request,
                                      viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Servo>(
        "ServoServer::GetGeometries", this, request)([&](auto&, auto& servo) {
        const ProtoStruct result = servo->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
