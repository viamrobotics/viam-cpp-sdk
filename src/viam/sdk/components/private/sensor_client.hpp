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
    using interface_type = Sensor;
    SensorClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    AttributeMap get_readings(const AttributeMap& extra) override;
    AttributeMap do_command(const AttributeMap& command) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

    using Sensor::get_geometries;
    using Sensor::get_readings;

   private:
    using StubType = viam::component::sensor::v1::SensorService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
