#include "viam/sdk/common/mesh.hpp"
#include "viam/sdk/rpc/grpc_context_observer.hpp"
#include <viam/sdk/tests/mocks/mock_arm.hpp>

#include "mock_arm.hpp"
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace arm {

std::map<std::string, sdk::mesh> fake_3d_models() {
    return {{"model1", sdk::mesh{"model1", {1, 2, 3, 4}}}};
}

sdk::KinematicsData fake_kinematics() {
    return sdk::KinematicsDataSVA{{std::vector<unsigned char>{1, 2, 3, 4}}};
}

sdk::KinematicsResponse fake_kinematics_response() {
    return sdk::KinematicsResponse{fake_kinematics(), {}};
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

void MockArm::stop(const sdk::ProtoStruct&) {
    peek_stop_called = true;
}

bool MockArm::is_moving() {
    return false;
}

sdk::ProtoStruct MockArm::do_command(const sdk::ProtoStruct& command) {
    return (peek_command = command);
}

sdk::KinematicsData MockArm::get_kinematics(const sdk::ProtoStruct&) {
    return fake_kinematics();
}

sdk::KinematicsResponse MockArm::get_kinematics_response(const sdk::ProtoStruct&) {
    return fake_kinematics_response();
}

std::vector<sdk::GeometryConfig> MockArm::get_geometries(const sdk::ProtoStruct&) {
    return fake_geometries();
}

std::map<std::string, sdk::mesh> MockArm::get_3d_models(const sdk::ProtoStruct&) {
    return fake_3d_models();
}

}  // namespace arm
}  // namespace sdktests
}  // namespace viam
