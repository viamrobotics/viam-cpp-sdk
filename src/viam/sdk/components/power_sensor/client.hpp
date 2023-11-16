/// @file components/power_sensor/client.hpp
///
/// @brief Implements a gRPC client for the `PowerSensor` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/powersensor/v1/powersensor.grpc.pb.h>

#include <viam/sdk/components/power_sensor/power_sensor.hpp>
#include <viam/sdk/components/power_sensor/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

/// @class PowerSensorClient
/// @brief gRPC client implementation of a `PowerSensor` component.
/// @ingroup PowerSensor
class PowerSensorClient : public PowerSensor {
   public:
    PowerSensorClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    voltage get_voltage(const AttributeMap& extra) override;
    current get_current(const AttributeMap& extra) override;
    double get_power(const AttributeMap& extra) override;
    AttributeMap get_readings(const AttributeMap& extra) override;
    AttributeMap do_command(const AttributeMap& command) override;

    using PowerSensor::get_current;
    using PowerSensor::get_power;
    using PowerSensor::get_readings;
    using PowerSensor::get_voltage;

    typedef viam::component::powersensor::v1::PowerSensorService::StubInterface Stub;

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
