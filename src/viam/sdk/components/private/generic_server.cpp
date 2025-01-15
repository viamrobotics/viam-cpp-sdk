#include <viam/sdk/components/private/generic_server.hpp>

#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/components/generic.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {
namespace impl {

GenericComponentServer::GenericComponentServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status GenericComponentServer::DoCommand(
    ::grpc::ServerContext*,
    const ::viam::common::v1::DoCommandRequest* request,
    ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<GenericComponent>(
        "GenericComponentServer::DoCommand", this, request)([&](auto&, auto& generic) {
        const ProtoStruct result = generic->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}
::grpc::Status GenericComponentServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<GenericComponent>(
        "GenericComponentServer::GetGeometries", this, request)([&](auto& helper, auto& generic) {
        const std::vector<GeometryConfig> geometries = generic->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = to_proto(geometry);
        }
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
