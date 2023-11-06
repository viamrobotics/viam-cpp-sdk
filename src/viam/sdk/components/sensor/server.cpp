#include <viam/sdk/components/sensor/server.hpp>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/sensor/sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

using namespace viam::common::v1;

namespace viam {
namespace sdk {

SensorServer::SensorServer() : ResourceServer(std::make_shared<ResourceManager>()){};
SensorServer::SensorServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

::grpc::Status SensorServer::GetReadings(::grpc::ServerContext* context,
                                         const GetReadingsRequest* request,
                                         GetReadingsResponse* response) {
    const auto status = server_wrapper<Sensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const AttributeMap result = status.sensor->get_readings(status.extra);
    for (const auto& r : *result) {
        response->mutable_readings()->insert({std::move(r.first), r.second->proto_value()});
    }
    return ::grpc::Status();
}

::grpc::Status SensorServer::DoCommand(grpc::ServerContext* context,
                                       const DoCommandRequest* request,
                                       DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Sensor::DoCommand] without a request");
    };

    auto rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Sensor> sensor = std::dynamic_pointer_cast<Sensor>(rb);
    const AttributeMap result = sensor->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

::grpc::Status SensorServer::GetGeometries(::grpc::ServerContext* context,
                                           const GetGeometriesRequest* request,
                                           GetGeometriesResponse* response) {
    const auto status = server_wrapper<Sensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const std::vector<GeometryConfig> geometries = status.sensor->get_geometries(status.extra);
    for (const auto& geometry : geometries) {
        *response->mutable_geometries()->Add() = geometry.to_proto();
    }
    return ::grpc::Status();
}

void SensorServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
