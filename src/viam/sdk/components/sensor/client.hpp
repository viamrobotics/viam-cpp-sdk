/// @file components/sensor/client.hpp
///
/// @brief Implements a gRPC client for the `Sensor` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/sensor/v1/sensor.grpc.pb.h>

#include <viam/sdk/components/sensor/sensor.hpp>
#include <viam/sdk/components/sensor/server.hpp>
#include <viam/sdk/config/resource.hpp>

namespace viam {
namespace sdk {

/// @class SensorClient
/// @brief gRPC client implementation of a `Sensor` component.
/// @ingroup Sensor
class SensorClient : public Sensor {
   public:
    SensorClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    AttributeMap get_readings(const AttributeMap& extra) override;
    AttributeMap do_command(const AttributeMap& command) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

    typedef viam::component::sensor::v1::SensorService::StubInterface Stub;

   private:
    // template to wrap a stub
    template <typename Request, typename Response, typename Cls>
    void stub_wrapper(Cls* self,
                      Response& resp,
                      const AttributeMap& extra,
                      ::grpc::Status (Cls::Stub::*method)(::grpc::ClientContext*,
                                                          const Request&,
                                                          Response*)) {
        Request request;
        grpc::ClientContext ctx;
        *request.mutable_name() = self->name();
        *request.mutable_extra() = map_to_struct(extra);
        const auto status = (*(self->stub_).*method)(&ctx, request, &resp);
        if (!status.ok()) {
            throw std::runtime_error(status.error_message());
        }
    }
    std::unique_ptr<Stub> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
