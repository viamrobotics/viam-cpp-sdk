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
    PowerSensorServer();
    explicit PowerSensorServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetVoltage(::grpc::ServerContext* context,
                              const GetVoltageRequest* request,
                              GetVoltageResponse* response) override;

    ::grpc::Status GetCurrent(::grpc::ServerContext* context,
                              const GetCurrentRequest* request,
                              GetCurrentResponse* response) override;

    ::grpc::Status GetPower(::grpc::ServerContext* context,
                            const GetPowerRequest* request,
                            GetPowerResponse* response) override;

    ::grpc::Status GetReadings(::grpc::ServerContext* context,
                               const viam::common::v1::GetReadingsRequest* request,
                               viam::common::v1::GetReadingsResponse* response) override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const viam::common::v1::DoCommandRequest* request,
                             viam::common::v1::DoCommandResponse* response) override;

    void register_server(std::shared_ptr<Server> server) override;

   private:
    template <typename Cls>
    struct ServerWrapperResult {
        ::grpc::Status status;
        AttributeMap extra;
        std::shared_ptr<Cls> powersensor;

        bool ok() const {
            return status.ok();
        }
    };

    // wrapper template for boilerplate
    template <typename Cls, typename Req>
    ServerWrapperResult<Cls> server_wrapper(const Req* request) {
        if (!request) {
            auto status = ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                                         "Called PowerSensorServer method without a request");
            return ServerWrapperResult<Cls>{status};
        };
        const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
        if (!rb) {
            auto status = grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
            return ServerWrapperResult<Cls>{status};
        }
        ServerWrapperResult<Cls> ret;
        ret.powersensor = std::dynamic_pointer_cast<Cls>(rb);
        if (request->has_extra()) {
            ret.extra = struct_to_map(request->extra());
        }
        return ret;
    }
};

}  // namespace sdk
}  // namespace viam
