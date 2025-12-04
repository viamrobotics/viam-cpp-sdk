#include <viam/sdk/components/private/gripper_server.hpp>

#include <viam/sdk/common/private/service_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

GripperServer::GripperServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status GripperServer::Open(::grpc::ServerContext* context,
                                   const ::viam::component::gripper::v1::OpenRequest* request,
                                   ::viam::component::gripper::v1::OpenResponse*) noexcept {
    return make_service_helper<Gripper>("GripperServer::Open", this, context, request)(
        [&](auto& helper, auto& gripper) { gripper->open(helper.getExtra()); });
}

::grpc::Status GripperServer::Grab(
    ::grpc::ServerContext* context,
    const ::viam::component::gripper::v1::GrabRequest* request,
    ::viam::component::gripper::v1::GrabResponse* response) noexcept {
    return make_service_helper<Gripper>(
        "GripperServer::Grab", this, context, request)([&](auto& helper, auto& gripper) {
        response->set_success(gripper->grab(helper.getExtra()));
    });
}

::grpc::Status GripperServer::IsHoldingSomething(
    ::grpc::ServerContext* context,
    const ::viam::component::gripper::v1::IsHoldingSomethingRequest* request,
    ::viam::component::gripper::v1::IsHoldingSomethingResponse* response) noexcept {
    return make_service_helper<Gripper>(
        "GripperServer::IsHoldingSomething", this, context, request)(
        [&](auto& helper, auto& gripper) {
            const Gripper::holding_status res = gripper->is_holding_something(helper.getExtra());
            response->set_is_holding_something(res.is_holding_something);
            *(response->mutable_meta()) = to_proto(res.meta);
        });
}

::grpc::Status GripperServer::Stop(::grpc::ServerContext* context,
                                   const ::viam::component::gripper::v1::StopRequest* request,
                                   ::viam::component::gripper::v1::StopResponse*) noexcept {
    return make_service_helper<Gripper>("GripperServer::Stop", this, context, request)(
        [&](auto& helper, auto& gripper) { gripper->stop(helper.getExtra()); });
}

::grpc::Status GripperServer::IsMoving(
    ::grpc::ServerContext* context,
    const ::viam::component::gripper::v1::IsMovingRequest* request,
    ::viam::component::gripper::v1::IsMovingResponse* response) noexcept {
    return make_service_helper<Gripper>("GripperServer::IsMoving", this, context, request)(
        [&](auto&, auto& gripper) { response->set_is_moving(gripper->is_moving()); });
}

::grpc::Status GripperServer::DoCommand(::grpc::ServerContext* context,
                                        const ::viam::common::v1::DoCommandRequest* request,
                                        ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Gripper>(
        "GripperServer::DoCommand", this, context, request)([&](auto&, auto& gripper) {
        const ProtoStruct result = gripper->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status GripperServer::GetGeometries(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Gripper>(
        "GripperServer::GetGeometries", this, context, request)([&](auto& helper, auto& gripper) {
        const std::vector<GeometryConfig> geometries = gripper->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = to_proto(geometry);
        }
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
