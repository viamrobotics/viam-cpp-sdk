#include <viam/sdk/components/private/arm_client.hpp>

#include <viam/api/component/arm/v1/arm.grpc.pb.h>
#include <viam/api/component/arm/v1/arm.pb.h>

#include <viam/sdk/common/client_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

ArmClient::ArmClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Arm(std::move(name)),
      stub_(viam::component::arm::v1::ArmService::NewStub(channel)),
      channel_(std::move(channel)) {}

pose ArmClient::get_end_position(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetEndPosition)
        .with(extra)
        .invoke([&](auto& response) { return pose::from_proto(response.pose()); });
}

void ArmClient::move_to_position(const pose& pose, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveToPosition)
        .with(extra, [&](auto& request) { *request.mutable_to() = pose.to_proto(); })
        .invoke();
}

std::vector<double> ArmClient::get_joint_positions(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetJointPositions)
        .with(extra)
        .invoke([](auto& response) {
            return std::vector<double>(response.positions().values().begin(),
                                       response.positions().values().end());
        });
}

void ArmClient::move_to_joint_positions(const std::vector<double>& positions,
                                        const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveToJointPositions)
        .with(extra,
              [&](auto& request) {
                  (request.mutable_positions())
                      ->mutable_values()
                      ->Assign(positions.begin(), positions.end());
              })
        .invoke();
}

bool ArmClient::is_moving() {
    return make_client_helper(this, *stub_, &StubType::IsMoving).invoke([](auto& response) {
        return response.is_moving();
    });
}

void ArmClient::stop(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::Stop).with(extra).invoke();
}

AttributeMap ArmClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

Arm::KinematicsData ArmClient::get_kinematics(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetKinematics)
        .with(extra)
        .invoke([](auto& response) { return Arm::from_proto(response); });
}

std::vector<GeometryConfig> ArmClient::get_geometries(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return GeometryConfig::from_proto(response); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
