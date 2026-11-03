#include <viam/sdk/tests/mocks/mock_gantry.hpp>

#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace gantry {

std::vector<double> fake_lengths() {
    return {1.0, 2.0, 3.0, 4.0};
}

sdk::KinematicModel fake_kinematic_model() {
    using namespace sdk;
    KinematicModel model;
    model.name = "fake_gantry";

    KinematicLink base;
    base.id = "base";

    KinematicJoint axis;
    axis.id = "x";
    axis.parent = "base";
    axis.axis = Vector3{1, 0, 0};
    axis.spec = PrismaticJoint{0.0, 1000.0, 50.0, boost::none, boost::none};

    KinematicLink carriage;
    carriage.id = "carriage";
    carriage.parent = "x";
    carriage.pose.translation = Vector3{0, 0, 50};
    carriage.pose.orientation = orientation_vector_degrees{0, 0, 1, 90};

    model.links = {base, carriage};
    model.joints = {axis};
    model.output_frames = {"carriage"};
    return model;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

sdk::KinematicsData fake_kinematics() {
    return sdk::KinematicsDataSVA{{std::vector<unsigned char>{1, 2, 3, 4}}};
}

sdk::KinematicsData MockGantry::get_kinematics(const sdk::ProtoStruct&) {
    return fake_kinematics();
}

#pragma GCC diagnostic pop

sdk::KinematicModel MockGantry::get_kinematic_model(const sdk::kinematics_options& options,
                                                    const sdk::ProtoStruct&) {
    peek_kinematics_options = options;
    return fake_kinematic_model();
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

sdk::ProtoStruct MockGantry::get_status() {
    return fake_status();
}

}  // namespace gantry
}  // namespace sdktests
}  // namespace viam
