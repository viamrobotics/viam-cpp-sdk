#pragma once

#include <viam/sdk/components/gantry.hpp>

namespace viam {
namespace sdktests {
namespace gantry {

std::vector<double> fake_lengths();
sdk::KinematicModel fake_kinematic_model();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
sdk::KinematicsData fake_kinematics();
#pragma GCC diagnostic pop

class MockGantry : public sdk::Gantry {
   public:
    MockGantry(std::string name) : Gantry(std::move(name)) {}

    static std::shared_ptr<MockGantry> get_mock_gantry();

    std::vector<double> get_position(const sdk::ProtoStruct& extra) override;
    void move_to_position(const std::vector<sdk::Gantry::movement_coordinate>& coordinates,
                          const sdk::ProtoStruct& extra) override;
    bool home(const sdk::ProtoStruct& extra) override;
    std::vector<double> get_lengths(const sdk::ProtoStruct& extra) override;
    bool is_moving() override;
    void stop(const sdk::ProtoStruct& extra) override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;
    sdk::ProtoStruct get_status() override;
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::ProtoStruct& extra) override;
    sdk::KinematicModel get_kinematic_model(const sdk::kinematics_options& options,
                                            const sdk::ProtoStruct& extra) override;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    sdk::KinematicsData get_kinematics(const sdk::ProtoStruct& extra) override;
#pragma GCC diagnostic pop

    std::vector<double> peek_positions;
    sdk::kinematics_options peek_kinematics_options;
    bool peek_stop_called{false};
    bool peek_home_called{false};
    sdk::ProtoStruct peek_command;
};
}  // namespace gantry
}  // namespace sdktests
}  // namespace viam
