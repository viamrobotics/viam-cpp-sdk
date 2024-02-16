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
    using interface_type = PowerSensor;
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

   private:
    using StubType = viam::component::powersensor::v1::PowerSensorService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
