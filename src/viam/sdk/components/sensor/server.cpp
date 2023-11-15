#include <viam/sdk/components/sensor/server.hpp>

#include <viam/sdk/common/service_helper.hpp>
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
                                         GetReadingsResponse* response) noexcept {
    return make_service_helper<Sensor>(
        "SensorServer::GetReadings", this, request)([&](auto& helper, auto& sensor) {
        const AttributeMap result = sensor->get_readings(helper.getExtra());
        for (const auto& r : *result) {
            response->mutable_readings()->insert({r.first, r.second->proto_value()});
        }
    });
}

::grpc::Status SensorServer::DoCommand(grpc::ServerContext* context,
                                       const DoCommandRequest* request,
                                       DoCommandResponse* response) noexcept {
    return make_service_helper<Sensor>(
        "SensorServer::DoCommand", this, request)([&](auto& helper, auto& sensor) {
        const AttributeMap result = sensor->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

::grpc::Status SensorServer::GetGeometries(::grpc::ServerContext* context,
                                           const GetGeometriesRequest* request,
                                           GetGeometriesResponse* response) noexcept {
    return make_service_helper<Sensor>(
        "SensorServer::GetGeometries", this, request)([&](auto& helper, auto& sensor) {
        const auto geometries = sensor->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

void SensorServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
