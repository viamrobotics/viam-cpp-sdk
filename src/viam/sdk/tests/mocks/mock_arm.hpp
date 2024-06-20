#pragma once

#include <viam/sdk/components/arm.hpp>

namespace viam {
namespace sdktests {
namespace arm {

sdk::Arm::KinematicsData fake_kinematics();

class MockArm : public sdk::Arm {
   public:
    MockArm(std::string name) : Arm(std::move(name)) {}

    static std::shared_ptr<MockArm> get_mock_arm();

    sdk::pose get_end_position(const sdk::AttributeMap&) override;
    void move_to_position(const sdk::pose& pose, const sdk::AttributeMap&) override;
    std::vector<double> get_joint_positions(const sdk::AttributeMap&) override;
    void move_to_joint_positions(const std::vector<double>& positions,
                                 const sdk::AttributeMap&) override;
    void stop(const sdk::AttributeMap&) override;
    bool is_moving() override;
    sdk::AttributeMap do_command(const sdk::AttributeMap& command) override;
    sdk::Arm::KinematicsData get_kinematics(const sdk::AttributeMap&) override;
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::AttributeMap&) override;

    sdk::pose current_location;
    std::vector<double> joint_positions;
    bool peek_stop_called;
    sdk::AttributeMap peek_command;
};

}  // namespace arm
}  // namespace sdktests
}  // namespace viam
