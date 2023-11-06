/// @file components/sensor/server.hpp
///
/// @brief Implements a gRPC server for the `Sensor` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/sensor/v1/sensor.grpc.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

using namespace viam::common::v1;

namespace viam {
namespace sdk {

/// @class SensorServer
/// @brief gRPC server implementation of a `Sensor` component.
/// @ingroup Sensor
class SensorServer : public ResourceServer,
                     public viam::component::sensor::v1::SensorService::Service {
   public:
    SensorServer();
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

    void register_server(std::shared_ptr<Server> server) override;

   private:
    template <typename Cls>
    struct ServerWrapperResult {
        ::grpc::Status status;
        AttributeMap extra;
        std::shared_ptr<Cls> sensor;

        bool ok() const {
            return status.ok();
        }
    };

    // wrapper template for boilerplate
    template <typename Cls, typename Req>
    ServerWrapperResult<Cls> server_wrapper(const Req* request) {
        if (!request) {
            auto status = ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                                         "Called SensorServer method without a request");
            return ServerWrapperResult<Cls>{status};
        };
        const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
        if (!rb) {
            auto status = grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
            return ServerWrapperResult<Cls>{status};
        }
        ServerWrapperResult<Cls> ret;
        ret.sensor = std::dynamic_pointer_cast<Cls>(rb);
        if (request->has_extra()) {
            ret.extra = struct_to_map(request->extra());
        }
        return ret;
    }
};

}  // namespace sdk
}  // namespace viam
