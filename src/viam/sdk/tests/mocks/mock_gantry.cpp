#include <viam/sdk/tests/mocks/mock_gantry.hpp>

#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace gantry {

std::vector<double> fake_lengths() {
    return {1.0, 2.0, 3.0, 4.0};
}

sdk::KinematicsResponse fake_kinematics() {
    return sdk::KinematicsResponse{
        sdk::KinematicsDataSVA{{std::vector<unsigned char>{1, 2, 3, 4}}},
        {}
    };
}

std::shared_ptr<MockGantry> MockGantry::get_mock_gantry() {
    return std::make_shared<MockGantry>("mock_gantry");
}

std::vector<double> MockGantry::get_position(const sdk::ProtoStruct&) {
    return peek_positions;
}

void MockGantry::move_to_position(const std::vector<sdk::Gantry::movement_coordinate>& coordinates,
                                  const sdk::ProtoStruct&) {
    peek_positions = {};
    for (const auto& coord : coordinates)
        peek_positions.push_back(coord.position_mm);
}

bool MockGantry::home(const sdk::ProtoStruct&) {
    return (peek_home_called = true);
}

std::vector<double> MockGantry::get_lengths(const sdk::ProtoStruct&) {
    return fake_lengths();
}

bool MockGantry::is_moving() {
    return false;
}

void MockGantry::stop(const sdk::ProtoStruct&) {
    peek_stop_called = true;
}

sdk::ProtoStruct MockGantry::do_command(const sdk::ProtoStruct& command) {
    return (peek_command = command);
}

std::vector<sdk::GeometryConfig> MockGantry::get_geometries(const sdk::ProtoStruct&) {
    return fake_geometries();
}

sdk::KinematicsResponse MockGantry::get_kinematics(const sdk::ProtoStruct&) {
    return fake_kinematics();
}

}  // namespace gantry
}  // namespace sdktests
}  // namespace viam
