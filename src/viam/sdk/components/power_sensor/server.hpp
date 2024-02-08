/// @file components/power_sensor/server.hpp
///
/// @brief Implements a gRPC server for the `PowerSensor` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.grpc.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

using namespace viam::component::powersensor::v1;

namespace viam {
namespace sdk {

/// @class PowerSensorServer
/// @brief gRPC server implementation of a `PowerSensor` component.
/// @ingroup PowerSensor
class PowerSensorServer : public ResourceServer, public PowerSensorService::Service {
   public:
    explicit PowerSensorServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetVoltage(::grpc::ServerContext* context,
                              const GetVoltageRequest* request,
                              GetVoltageResponse* response) noexcept override;

    ::grpc::Status GetCurrent(::grpc::ServerContext* context,
                              const GetCurrentRequest* request,
                              GetCurrentResponse* response) noexcept override;

    ::grpc::Status GetPower(::grpc::ServerContext* context,
                            const GetPowerRequest* request,
                            GetPowerResponse* response) noexcept override;

    ::grpc::Status GetReadings(::grpc::ServerContext* context,
                               const viam::common::v1::GetReadingsRequest* request,
                               viam::common::v1::GetReadingsResponse* response) noexcept override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const viam::common::v1::DoCommandRequest* request,
                             viam::common::v1::DoCommandResponse* response) noexcept override;
};

}  // namespace sdk
}  // namespace viam
