#include <viam/sdk/tests/mocks/mock_movement_sensor.hpp>

#include <viam/sdk/components/movement_sensor.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace movementsensor {

using namespace viam::sdk;

Vector3 MockMovementSensor::get_linear_velocity(const AttributeMap&) {
    return this->peek_return_vec;
};
Vector3 MockMovementSensor::get_angular_velocity(const AttributeMap&) {
    return this->peek_return_vec;
};
MovementSensor::compassheading MockMovementSensor::get_compass_heading(const AttributeMap&) {
    return this->peek_compass_heading;
};
MovementSensor::orientation MockMovementSensor::get_orientation(const AttributeMap&) {
    return this->peek_orientation;
};
MovementSensor::position MockMovementSensor::get_position(const AttributeMap&) {
    return this->peek_position;
};
MovementSensor::properties MockMovementSensor::get_properties(const AttributeMap&) {
    return this->peek_properties;
};
std::unordered_map<std::string, float> MockMovementSensor::get_accuracy(const AttributeMap&) {
    return this->peek_accuracy;
};
Vector3 MockMovementSensor::get_linear_acceleration(const AttributeMap&) {
    return this->peek_return_vec;
};
AttributeMap MockMovementSensor::do_command(const AttributeMap& command) {
    this->peek_do_command_command = command;
    return command;
};
std::vector<sdk::GeometryConfig> MockMovementSensor::get_geometries(const AttributeMap&) {
    return fake_geometries();
};

std::shared_ptr<MockMovementSensor> MockMovementSensor::get_mock_movementsensor() {
    auto movementsensor = std::make_shared<MockMovementSensor>("mock_movementsensor");
    return movementsensor;
}

}  // namespace movementsensor
}  // namespace sdktests
}  // namespace viam
