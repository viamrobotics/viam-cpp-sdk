#include <viam/sdk/components/private/sensor_server.hpp>

#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

using namespace viam::common::v1;

namespace viam {
namespace sdk {
namespace impl {

SensorServer::SensorServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status SensorServer::GetReadings(::grpc::ServerContext* context,
                                         const GetReadingsRequest* request,
                                         GetReadingsResponse* response) noexcept {
    return make_service_helper<Sensor>(
        "SensorServer::GetReadings", this, context, request)([&](auto& helper, auto& sensor) {
        *(response->mutable_readings()) =
            to_proto(sensor->get_readings(helper.getExtra())).fields();
    });
}

::grpc::Status SensorServer::DoCommand(grpc::ServerContext* context,
                                       const DoCommandRequest* request,
                                       DoCommandResponse* response) noexcept {
    return make_service_helper<Sensor>(
        "SensorServer::DoCommand", this, context, request)([&](auto&, auto& sensor) {
        const ProtoStruct result = sensor->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status SensorServer::GetGeometries(::grpc::ServerContext* context,
                                           const GetGeometriesRequest* request,
                                           GetGeometriesResponse* response) noexcept {
    return make_service_helper<Sensor>(
        "SensorServer::GetGeometries", this, context, request)([&](auto& helper, auto& sensor) {
        const auto geometries = sensor->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = to_proto(geometry);
        }
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
