#include <viam/sdk/tests/mocks/mock_arm.hpp>

#include <stdexcept>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/mesh.hpp>
#include <viam/sdk/rpc/grpc_context_observer.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace arm {

std::map<std::string, sdk::mesh> fake_3d_models() {
    return {{"model1", sdk::mesh{"model1", {1, 2, 3, 4}}}};
}

// One link or joint of each kind, with every optional field populated somewhere, so the
// round trip covers the whole model. The empty name is filled in by the server.
sdk::KinematicModel fake_kinematic_model() {
    using namespace sdk;
    KinematicModel model;

    KinematicLink base;
    base.id = "base";
    base.collision = {GeometryConfig(pose{}, mesh{"stl", {1, 2, 3}}, "base_mesh")};

    KinematicJoint j1;
    j1.id = "j1";
    j1.parent = "base";
    j1.axis = Vector3{0, 0, 1};
    j1.spec = RevoluteJoint{-180.0, 180.0, 90.0, boost::none};

    KinematicLink link1;
    link1.id = "link1";
    link1.parent = "j1";
    link1.pose.translation = Vector3{0, 0, 100};
    link1.pose.orientation = quaternion{0, 0, 0, 1};
    link1.visual = {GeometryConfig(pose{}, box{1, 2, 3}, "link1_box")};

    KinematicJoint j2;
    j2.id = "j2";
    j2.parent = "link1";
    j2.axis = Vector3{1, 0, 0};
    j2.spec = PrismaticJoint{
        0.0, 500.0, boost::none, 10.0, GeometryConfig(pose{}, capsule{1, 2}, "rail")};
    j2.mimic = MimicJoint{"j1", 2.0, 1.0};

    KinematicLink link2;
    link2.id = "link2";
    link2.parent = "j2";
    link2.pose.translation = Vector3{10, 20, 30};
    link2.pose.orientation = euler_angles{90, 0, 0};

    KinematicJoint j3;
    j3.id = "j3";
    j3.parent = "link2";
    j3.axis = Vector3{0, 1, 0};
    j3.spec = ContinuousJoint{};

    KinematicLink tool;
    tool.id = "tool";
    tool.parent = "j3";
    tool.pose.orientation = axis_angles{0, 0, 1, 45};

    model.links = {base, link1, link2, tool};
    model.joints = {j1, j2, j3};
    model.output_frames = {"tool"};
    model.properties = KinematicProperties{30.0, boost::none};
    return model;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

sdk::KinematicsData fake_kinematics() {
    return sdk::KinematicsDataSVA{{std::vector<unsigned char>{1, 2, 3, 4}}};
}

sdk::KinematicsData MockArm::get_kinematics(const sdk::ProtoStruct&) {
    return fake_kinematics();
}

#pragma GCC diagnostic pop

sdk::KinematicModel MockArm::get_kinematic_model(const sdk::kinematics_options& options,
                                                 const sdk::ProtoStruct&) {
    peek_kinematics_options = options;
    return fake_kinematic_model();
}

sdk::pose MockArm::get_end_position(const sdk::ProtoStruct& extra) {
    return current_location;
}

std::shared_ptr<MockArm> MockArm::get_mock_arm() {
    return std::make_shared<MockArm>("mock_arm");
}

void MockArm::move_to_position(const sdk::pose& pose, const sdk::ProtoStruct&) {
    current_location = pose;
}

std::vector<double> MockArm::get_joint_positions(const sdk::ProtoStruct&) {
    return joint_positions;
}

void MockArm::move_to_joint_positions(const std::vector<double>& positions,
                                      const sdk::ProtoStruct&) {
    joint_positions = positions;
}

void MockArm::move_through_joint_positions(const std::vector<std::vector<double>>& positions,
                                           const Arm::MoveOptions& opts,
                                           const sdk::ProtoStruct&) {
    const auto& observer = GrpcContextObserver::current();
    if (observer) {
        viam_client_metadata = observer->get_client_metadata_field_values("viam_client");
    }
    move_thru_positions = positions;
    move_opts = opts;
}

sdk::Arm::stream_outcome MockArm::move_through_joint_positions_streamed(
    const std::function<boost::optional<std::vector<Arm::trajectory_point>>()>& batch_source,
    const std::function<bool(Arm::trajectory_update)>& update_handler,
    const sdk::ProtoStruct&) {
    while (auto batch = batch_source()) {
        peek_streamed_batches.push_back(*batch);
        if (!update_handler(Arm::trajectory_update{})) {
            return sdk::Arm::stream_outcome::k_halted_by_update_handler;
        }
        ++peek_streamed_ack_count;
    }

    switch (streamed_fault) {
        case stream_fault::k_none:
            break;
        case stream_fault::k_runtime_error:
            throw std::runtime_error("mock arm streamed fault");
        case stream_fault::k_grpc_status:
            throw grpc::Status(grpc::StatusCode::FAILED_PRECONDITION,
                               "mock arm streamed grpc fault");
    }

    return sdk::Arm::stream_outcome::k_completed;
}

void MockArm::stop(const sdk::ProtoStruct&) {
    peek_stop_called = true;
}

bool MockArm::is_moving() {
    return false;
}

sdk::ProtoStruct MockArm::get_status() {
    return fake_status();
}

sdk::ProtoStruct MockArm::do_command(const sdk::ProtoStruct& command) {
    return (peek_command = command);
}

std::vector<sdk::GeometryConfig> MockArm::get_geometries(const sdk::ProtoStruct&) {
    return fake_geometries();
}

std::map<std::string, sdk::mesh> MockArm::get_3d_models(const sdk::ProtoStruct&) {
    return fake_3d_models();
}

sdk::Arm::properties MockArm::get_properties(const sdk::ProtoStruct&) {
    // The mock implements move_to_position but does not support manual mode.
    return {/*support_manual_mode=*/false, /*support_cartesian_commands=*/true};
}

void MockArm::set_manual_mode(bool, std::chrono::seconds, const sdk::ProtoStruct&) {
    throw sdk::Exception(sdk::ErrorCondition::k_not_supported,
                         "mock arm does not support manual mode");
}

bool MockArm::get_manual_mode(const sdk::ProtoStruct&) {
    throw sdk::Exception(sdk::ErrorCondition::k_not_supported,
                         "mock arm does not support manual mode");
}

}  // namespace arm
}  // namespace sdktests
}  // namespace viam
