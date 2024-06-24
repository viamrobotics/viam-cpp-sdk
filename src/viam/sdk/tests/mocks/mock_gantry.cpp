#include <viam/sdk/tests/mocks/mock_gantry.hpp>

#include "mock_gantry.hpp"
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace gantry {

std::vector<double> fake_lengths() {
    return {1.0, 2.0, 3.0, 4.0};
}

std::shared_ptr<MockGantry> MockGantry::get_mock_gantry() {
    return std::make_shared<MockGantry>("mock_gantry");
}

std::vector<double> MockGantry::get_position(const sdk::AttributeMap&) {
    return peek_positions;
}

void MockGantry::move_to_position(const std::vector<sdk::Gantry::movement_coordinate>& coordinates,
                                  const sdk::AttributeMap&) {
    peek_positions = {};
    for (const auto& coord : coordinates)
        peek_positions.push_back(coord.position_mm);
}

bool MockGantry::home(const sdk::AttributeMap&) {
    return true;
}

std::vector<double> MockGantry::get_lengths(const sdk::AttributeMap&) {
    return fake_lengths();
}

bool MockGantry::is_moving() {
    return false;
}

void MockGantry::stop(const sdk::AttributeMap&) {
    peek_stop_called = true;
}

sdk::AttributeMap MockGantry::do_command(const sdk::AttributeMap& command) {
    return (peek_command = command);
}

std::vector<sdk::GeometryConfig> MockGantry::get_geometries(const sdk::AttributeMap&) {
    return fake_geometries();
}

}  // namespace gantry
}  // namespace sdktests
}  // namespace viam
