#include <viam/sdk/tests/mocks/mock_movementsensor.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/movementsensor/v1/movementsensor.grpc.pb.h>
#include <viam/api/component/movementsensor/v1/movementsensor.pb.h>

#include <viam/sdk/components/movementsensor/movementsensor.hpp>
#include <viam/sdk/components/movementsensor/server.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace movementsensor {

using namespace viam::sdk;

Vector3 MockMovementSensor::get_linear_velocity(const AttributeMap& extra) {
    return this->peek_return_vec;
};
Vector3 MockMovementSensor::get_angular_velocity(const AttributeMap& extra) {
    return this->peek_return_vec;
};
MovementSensor::compassheading MockMovementSensor::get_compass_heading(const AttributeMap& extra) {
    return this->peek_compass_heading;
};
MovementSensor::orientation MockMovementSensor::get_orientation(const AttributeMap& extra) {
    return this->peek_orientation;
};
MovementSensor::position MockMovementSensor::get_position(const AttributeMap& extra) {
    return this->peek_position;
};
MovementSensor::properties MockMovementSensor::get_properties(const AttributeMap& extra) {
    return this->peek_properties;
};
std::unordered_map<std::string, float> MockMovementSensor::get_accuracy(const AttributeMap& extra) {
    return this->peek_accuracy;
};
Vector3 MockMovementSensor::get_linear_acceleration(const AttributeMap& extra) {
    return this->peek_return_vec;
};
AttributeMap MockMovementSensor::do_command(const AttributeMap& command) {
    this->peek_do_command_command = command;
    return command;
};
std::vector<sdk::GeometryConfig> MockMovementSensor::get_geometries(const AttributeMap& extra) {
    return std::vector<sdk::GeometryConfig>();
};

std::shared_ptr<MockMovementSensor> MockMovementSensor::get_mock_movementsensor() {
    auto movementsensor = std::make_shared<MockMovementSensor>("mock_movementsensor");
    return movementsensor;
}

}  // namespace movementsensor
}  // namespace sdktests
}  // namespace viam
