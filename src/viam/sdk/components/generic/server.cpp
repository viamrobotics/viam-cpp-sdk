#include <viam/sdk/components/generic/server.hpp>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

GenericComponentServer::GenericComponentServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)){};

::grpc::Status GenericComponentServer::DoCommand(
    ::grpc::ServerContext*,
    const ::viam::common::v1::DoCommandRequest* request,
    ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<GenericComponent>(
        "GenericComponentServer::DoCommand", this, request)([&](auto&, auto& generic) {
        const AttributeMap result = generic->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
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
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

}  // namespace sdk
}  // namespace viam
