#pragma once

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/sensor.hpp>

namespace viam {
namespace sdktests {
namespace sensor {

class MockSensor : public sdk::Sensor {
   public:
    sdk::AttributeMap get_readings(const sdk::AttributeMap& extra) override;
    sdk::AttributeMap do_command(const sdk::AttributeMap& command) override;
    static std::shared_ptr<MockSensor> get_mock_sensor();
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::AttributeMap& extra) override;

    MockSensor(std::string name) : Sensor(std::move(name)){};
};

}  // namespace sensor
}  // namespace sdktests
}  // namespace viam
