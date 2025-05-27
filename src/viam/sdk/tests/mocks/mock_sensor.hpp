#pragma once

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/sensor.hpp>

namespace viam {
namespace sdktests {
namespace sensor {

class MockSensor : public sdk::Sensor {
   public:
    sdk::ProtoStruct get_readings(const sdk::ProtoStruct& extra) override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;
    static std::shared_ptr<MockSensor> get_mock_sensor();
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::ProtoStruct& extra) override;

    MockSensor(std::string name) : Sensor(std::move(name)) {}
};

}  // namespace sensor
}  // namespace sdktests
}  // namespace viam
