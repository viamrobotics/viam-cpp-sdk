#include <viam/sdk/components/private/gripper_server.hpp>

#include "gripper_server.hpp"
#include <viam/sdk/common/service_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

GripperServer::GripperServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status GripperServer::Open(::grpc::ServerContext*,
                                   const ::viam::component::gripper::v1::OpenRequest* request,
                                   ::viam::component::gripper::v1::OpenResponse*) noexcept {
    return make_service_helper<Gripper>("GripperServer::Open", this, request)(
        [&](auto& helper, auto& gripper) { gripper->open(helper.getExtra()); });
}

::grpc::Status GripperServer::Grab(
    ::grpc::ServerContext*,
    const ::viam::component::gripper::v1::GrabRequest* request,
    ::viam::component::gripper::v1::GrabResponse* response) noexcept {
    return make_service_helper<Gripper>("GripperServer::Grab", this, request)(
        [&](auto& helper, auto& gripper) { response->set_success(gripper->grab()); });
}

::grpc::Status GripperServer::Stop(::grpc::ServerContext*,
                                   const ::viam::component::gripper::v1::StopRequest* request,
                                   ::viam::component::gripper::v1::StopResponse*) noexcept {
    return make_service_helper<Gripper>("GripperServer::Stop", this, request)(
        [&](auto& helper, auto& gripper) { gripper->stop(helper.getExtra()); });
}

::grpc::Status GripperServer::IsMoving(
    ::grpc::ServerContext*,
    const ::viam::component::gripper::v1::IsMovingRequest* request,
    ::viam::component::gripper::v1::IsMovingResponse* response) noexcept {
    return make_service_helper<Gripper>("GripperServer::IsMoving", this, request)(
        [&](auto&, auto& gripper) { response->set_is_moving(gripper->is_moving()); });
}

::grpc::Status GripperServer::DoCommand(::grpc::ServerContext*,
                                        const ::viam::common::v1::DoCommandRequest* request,
                                        ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Gripper>(
        "GripperServer::DoCommand", this, request)([&](auto&, auto& gripper) {
        const AttributeMap result = gripper->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

::grpc::Status GripperServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Gripper>(
        "GripperServer::GetGeometries", this, request)([&](auto& helper, auto& gripper) {
        const std::vector<GeometryConfig> geometries = gripper->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
