/// @file components/private/sensor_client.hpp
///
/// @brief Implements a gRPC client for the `Sensor` component.
#pragma once

#include <viam/api/component/sensor/v1/sensor.grpc.pb.h>

#include <viam/sdk/components/private/sensor_server.hpp>
#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class SensorClient
/// @brief gRPC client implementation of a `Sensor` component.
/// @ingroup Sensor
class SensorClient : public Sensor {
   public:
    using interface_type = Sensor;
    SensorClient(std::string name, ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }
    ProtoStruct get_readings(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;

    using Sensor::get_geometries;
    using Sensor::get_readings;

   private:
    using StubType = viam::component::sensor::v1::SensorService::StubInterface;
    std::unique_ptr<StubType> stub_;
    ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
