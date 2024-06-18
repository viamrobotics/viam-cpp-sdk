#include <viam/sdk/tests/mocks/mock_arm.hpp>

#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace arm {

sdk::Arm::KinematicsData fake_kinematics() {
    return sdk::Arm::KinematicsDataSVA{{std::vector<unsigned char>{1, 2, 3, 4}}};
}

sdk::pose MockArm::get_end_position(const sdk::AttributeMap& extra) {
    return current_location;
}

std::shared_ptr<MockArm> MockArm::get_mock_arm()
{
    return std::make_shared<MockArm>("mock_arm");
}

void MockArm::move_to_position(const sdk::pose& pose, const sdk::AttributeMap&) {
    current_location = pose;
}

std::vector<double> MockArm::get_joint_positions(const sdk::AttributeMap&) {
    return joint_positions;
}

void MockArm::move_to_joint_positions(const std::vector<double>& positions,
                                      const sdk::AttributeMap&) {
    joint_positions = positions;
}

void MockArm::stop(const sdk::AttributeMap&) {
    peek_stop_called = true;
}

bool MockArm::is_moving() {
    return false;
}

sdk::AttributeMap MockArm::do_command(const sdk::AttributeMap& command) {
    return (peek_command = command);
}

sdk::Arm::KinematicsData MockArm::get_kinematics(const sdk::AttributeMap&) {
    return fake_kinematics();
}

std::vector<sdk::GeometryConfig> MockArm::get_geometries(const sdk::AttributeMap&) {
    return fake_geometries();
}

}  // namespace arm
}  // namespace sdktests
}  // namespace viam
