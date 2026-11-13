#include <viam/sdk/tests/mocks/mock_gripper.hpp>

#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace gripper {

using namespace viam::sdk;

sdk::KinematicModel fake_kinematic_model() {
    KinematicModel model;
    model.name = "fake_gripper";

    KinematicLink palm;
    palm.id = "palm";
    palm.collision = {GeometryConfig(pose{}, box{80, 40, 60}, "palm_box")};

    KinematicJoint left;
    left.id = "left";
    left.parent = "palm";
    left.axis = Vector3{1, 0, 0};
    left.spec = PrismaticJoint{0.0, 40.0, 100.0, boost::none, boost::none};

    KinematicJoint right;
    right.id = "right";
    right.parent = "palm";
    right.axis = Vector3{1, 0, 0};
    right.spec = PrismaticJoint{};
    right.mimic = MimicJoint{"left", -1.0, 0.0};

    KinematicLink left_finger;
    left_finger.id = "left_finger";
    left_finger.parent = "left";
    left_finger.pose.translation = Vector3{0, 0, 60};

    KinematicLink right_finger;
    right_finger.id = "right_finger";
    right_finger.parent = "right";
    right_finger.pose.translation = Vector3{0, 0, 60};

    model.links = {palm, left_finger, right_finger};
    model.joints = {left, right};
    model.output_frames = {"left_finger"};
    return model;
}

std::shared_ptr<MockGripper> MockGripper::get_mock_gripper() {
    return std::make_shared<MockGripper>("mock_gripper");
}

void MockGripper::open(const ProtoStruct&) {
    peek_open_called = true;
}

bool MockGripper::grab(const ProtoStruct&) {
    return true;
}

void MockGripper::stop(const ProtoStruct&) {
    peek_stop_called = true;
}

bool MockGripper::is_moving() {
    return false;
}

Gripper::holding_status MockGripper::is_holding_something(const ProtoStruct& extra) {
    Gripper::holding_status res;
    res.is_holding_something = false;
    res.meta = extra;
    return res;
}

ProtoStruct MockGripper::do_command(const ProtoStruct& command) {
    return (peek_command = command);
}

std::vector<GeometryConfig> MockGripper::get_geometries(const ProtoStruct&) {
    return fake_geometries();
}

KinematicModel MockGripper::get_kinematic_model(const kinematics_options& options,
                                                const ProtoStruct&) {
    peek_kinematics_options = options;
    return fake_kinematic_model();
}

sdk::ProtoStruct MockGripper::get_status() {
    return fake_status();
}

}  // namespace gripper
}  // namespace sdktests
}  // namespace viam
