#include <viam/sdk/components/generic/server.hpp>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

GenericServer::GenericServer() : ResourceServer(std::make_shared<ResourceManager>()){};
GenericServer::GenericServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

::grpc::Status GenericServer::DoCommand(::grpc::ServerContext* context,
                                        const ::viam::common::v1::DoCommandRequest* request,
                                        ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Generic>(
        "GenericServer::DoCommand", this, request)([&](auto& helper, auto& generic) {
        const AttributeMap result = generic->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}
::grpc::Status GenericServer::GetGeometries(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Generic>(
        "GenericServer::GetGeometries", this, request)([&](auto& helper, auto& generic) {
        // CR erodkin: flyby adding extra
        const std::vector<GeometryConfig> geometries = generic->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

void GenericServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
