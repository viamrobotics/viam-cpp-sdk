#include <viam/sdk/components/generic/server.hpp>

#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

GenericServer::GenericServer() : ResourceServer(std::make_shared<ResourceManager>()){};
GenericServer::GenericServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

::grpc::Status GenericServer::DoCommand(::grpc::ServerContext* context,
                                        const ::viam::common::v1::DoCommandRequest* request,
                                        ::viam::common::v1::DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [DoCommand] without a request");
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Generic> generic = std::dynamic_pointer_cast<Generic>(rb);
    AttributeMap result = generic->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}
::grpc::Status GenericServer::GetGeometries(::grpc::ServerContext* context,
                                            const ::viam::common::v1::GetGeometriesRequest* request,
                                            ::viam::common::v1::GetGeometriesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetGeometries] without a request");
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Generic> generic = std::dynamic_pointer_cast<Generic>(rb);
    std::vector<GeometryConfig> geometries = generic->get_geometries(request->name());

    auto response_geometries = *response->mutable_geometries();
    for (const auto& geometry : geometries) {
        *response_geometries.Add() = geometry.to_proto();
    }
    return ::grpc::Status();
}

void GenericServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
