/// @file components/private/sensor_server.hpp
///
/// @brief Implements a gRPC server for the `Sensor` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/sensor/v1/sensor.grpc.pb.h>

#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

using namespace viam::common::v1;

namespace viam {
namespace sdk {
namespace impl {

/// @class SensorServer
/// @brief gRPC server implementation of a `Sensor` component.
/// @ingroup Sensor
class SensorServer : public ResourceServer,
                     public viam::component::sensor::v1::SensorService::Service {
   public:
    using interface_type = Sensor;
    using service_type = component::sensor::v1::SensorService;
    explicit SensorServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetReadings(::grpc::ServerContext* context,
                               const GetReadingsRequest* request,
                               GetReadingsResponse* response) noexcept override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const DoCommandRequest* request,
                             DoCommandResponse* response) noexcept override;

    ::grpc::Status GetGeometries(::grpc::ServerContext* context,
                                 const GetGeometriesRequest* request,
                                 GetGeometriesResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
