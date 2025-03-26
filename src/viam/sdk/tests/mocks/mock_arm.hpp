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

    sdk::pose get_end_position(const sdk::ProtoStruct&) override;
    void move_to_position(const sdk::pose& pose, const sdk::ProtoStruct&) override;
    std::vector<double> get_joint_positions(const sdk::ProtoStruct&) override;
    void move_to_joint_positions(const std::vector<double>& positions,
                                 const sdk::ProtoStruct&) override;

    void move_through_joint_positions(const std::vector<std::vector<double>>& positions,
                                      const Arm::MoveOptions& opts,
                                      const sdk::ProtoStruct&) override;

    void stop(const sdk::ProtoStruct&) override;
    bool is_moving() override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;
    sdk::Arm::KinematicsData get_kinematics(const sdk::ProtoStruct&) override;
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::ProtoStruct&) override;

    sdk::pose current_location;
    std::vector<double> joint_positions;
    std::vector<std::vector<double>> move_thru_positions;
    sdk::Arm::MoveOptions move_opts;
    bool peek_stop_called;
    sdk::ProtoStruct peek_command;
};

}  // namespace arm
}  // namespace sdktests
}  // namespace viam
