#include <viam/sdk/components/private/gripper_server.hpp>

#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/components/gripper.hpp>

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

::grpc::Status GripperServer::GetStatus(::grpc::ServerContext* context,
                                        const ::viam::common::v1::GetStatusRequest* request,
                                        ::viam::common::v1::GetStatusResponse* response) noexcept {
    return make_service_helper<Gripper>(
        "GripperServer::GetStatus", this, context, request)([&](auto&, auto& gripper) {
        const ProtoStruct result = gripper->get_status();
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status GripperServer::SetPercentOpen(
    ::grpc::ServerContext* context,
    const ::viam::component::gripper::v1::SetPercentOpenRequest* request,
    ::viam::component::gripper::v1::SetPercentOpenResponse*) noexcept {
    return make_service_helper<Gripper>("GripperServer::SetPercentOpen", this, context, request)(
        [&](auto& helper, auto& gripper) { gripper->set_percent_open(request->percent_open(), helper.getExtra()); });
}

::grpc::Status GripperServer::GetPercentOpen(
    ::grpc::ServerContext* context,
    const ::viam::component::gripper::v1::GetPercentOpenRequest* request,
    ::viam::component::gripper::v1::GetPercentOpenResponse* response) noexcept {
    return make_service_helper<Gripper>("GripperServer::GetPercentOpen", this, context, request)(
        [&](auto& helper, auto& gripper) { response->set_percent_open(gripper->get_percent_open(helper.getExtra())); });
}

::grpc::Status GripperServer::GetProperties(
    ::grpc::ServerContext* context,
    const ::viam::component::gripper::v1::GetPropertiesRequest* request,
    ::viam::component::gripper::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<Gripper>("GripperServer::GetProperties", this, context, request)(
        [&](auto& helper, auto& gripper) {
            const Gripper::properties prop = gripper->get_properties(helper.getExtra());
            *(response->mutable_properties()) = to_proto(prop);
        });
}

// to_proto specializations

// to_proto for Gripper::holding_status
::viam::component::gripper::v1::IsHoldingSomethingResponse GripperServer::to_proto(
    const Gripper::holding_status& hs) {
    ::viam::component::gripper::v1::IsHoldingSomethingResponse response;
    response.set_is_holding_something(hs.is_holding_something);
    *response.mutable_meta() = to_proto(hs.meta);
    return response;
}

// to_proto for Gripper::properties
::viam::component::gripper::v1::GetPropertiesResponse GripperServer::to_proto(
    const Gripper::properties& prop) {
    ::viam::component::gripper::v1::GetPropertiesResponse response;
    response.set_holds_item(prop.holds_item);
    response.set_can_grasp(prop.can_grasp);
    return response;
}

// to_proto for Gripper::percent_open_status
::viam::component::gripper::v1::GetPercentOpenResponse GripperServer::to_proto(
    const Gripper::percent_open_status& pos) {
    ::viam::component::gripper::v1::GetPercentOpenResponse response;
    response.set_percent_open(pos.percent_open);
    return response;
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam