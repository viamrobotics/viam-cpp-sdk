#pragma once

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/movement_sensor.hpp>

namespace viam {
namespace sdktests {
namespace movementsensor {

class MockMovementSensor : public sdk::MovementSensor {
   public:
    sdk::Vector3 get_linear_velocity(const sdk::ProtoStruct& extra) override;
    sdk::Vector3 get_angular_velocity(const sdk::ProtoStruct& extra) override;
    sdk::MovementSensor::compassheading get_compass_heading(const sdk::ProtoStruct& extra) override;
    sdk::MovementSensor::orientation get_orientation(const sdk::ProtoStruct& extra) override;
    sdk::MovementSensor::position get_position(const sdk::ProtoStruct& extra) override;
    sdk::MovementSensor::properties get_properties(const sdk::ProtoStruct& extra) override;
    std::unordered_map<std::string, float> get_accuracy(const sdk::ProtoStruct& extra) override;
    sdk::Vector3 get_linear_acceleration(const sdk::ProtoStruct& extra) override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;
    static std::shared_ptr<MockMovementSensor> get_mock_movementsensor();
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::ProtoStruct& extra) override;

    // These variables allow the testing infra to `peek` into the mock
    // and ensure that the correct values were passed
    sdk::Vector3 peek_return_vec;
    sdk::MovementSensor::compassheading peek_compass_heading;
    sdk::MovementSensor::orientation peek_orientation;
    sdk::MovementSensor::position peek_position;
    sdk::MovementSensor::properties peek_properties;
    std::unordered_map<std::string, float> peek_accuracy;
    sdk::ProtoStruct peek_do_command_command;

    MockMovementSensor(std::string name) : MovementSensor(std::move(name)){};
};

}  // namespace movementsensor
}  // namespace sdktests
}  // namespace viam
