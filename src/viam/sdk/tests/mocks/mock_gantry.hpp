#pragma once

#include <viam/sdk/components/gantry.hpp>

namespace viam {
namespace sdktests {
namespace gantry {

std::vector<double> fake_lengths();

class MockGantry : public sdk::Gantry {
   public:
    MockGantry(std::string name) : Gantry(std::move(name)) {}

    static std::shared_ptr<MockGantry> get_mock_gantry();

    std::vector<double> get_position(const sdk::AttributeMap& extra) override;
    void move_to_position(const std::vector<sdk::Gantry::movement_coordinate>& coordinates,
                          const sdk::AttributeMap& extra) override;
    bool home(const sdk::AttributeMap& extra) override;
    std::vector<double> get_lengths(const sdk::AttributeMap& extra) override;
    bool is_moving() override;
    void stop(const sdk::AttributeMap& extra) override;
    sdk::AttributeMap do_command(const sdk::AttributeMap& command) override;
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::AttributeMap& extra) override;

    std::vector<double> peek_positions;
    bool peek_stop_called{false};
    bool peek_home_called{false};
    sdk::AttributeMap peek_command;
};
}  // namespace gantry
}  // namespace sdktests
}  // namespace viam
