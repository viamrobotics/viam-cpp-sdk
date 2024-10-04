/// @file components/private/power_sensor_client.hpp
///
/// @brief Implements a gRPC client for the `PowerSensor` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/powersensor/v1/powersensor.grpc.pb.h>

#include <viam/sdk/components/power_sensor.hpp>
#include <viam/sdk/components/private/power_sensor_server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class PowerSensorClient
/// @brief gRPC client implementation of a `PowerSensor` component.
/// @ingroup PowerSensor
class PowerSensorClient : public PowerSensor {
   public:
    using interface_type = PowerSensor;
    PowerSensorClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    voltage get_voltage(const ProtoStruct& extra) override;
    current get_current(const ProtoStruct& extra) override;
    double get_power(const ProtoStruct& extra) override;
    ProtoStruct get_readings(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;

    using PowerSensor::get_current;
    using PowerSensor::get_power;
    using PowerSensor::get_readings;
    using PowerSensor::get_voltage;

   private:
    using StubType = viam::component::powersensor::v1::PowerSensorService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
