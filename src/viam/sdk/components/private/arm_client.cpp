#include <viam/sdk/components/private/arm_client.hpp>

#include <grpcpp/channel.h>

#include <viam/api/component/arm/v1/arm.grpc.pb.h>
#include <viam/api/component/arm/v1/arm.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/kinematics.hpp>

namespace viam {
namespace sdk {
namespace impl {

using sdk::from_proto;
using sdk::to_proto;

ArmClient::ArmClient(std::string name, const ViamChannel& channel)
    : Arm(std::move(name)),
      stub_(viam::component::arm::v1::ArmService::NewStub(channel.channel())),
      channel_(&channel) {}

pose ArmClient::get_end_position(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetEndPosition)
        .with(extra)
        .invoke([&](auto& response) { return from_proto(response.pose()); });
}

void ArmClient::move_to_position(const pose& pose, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveToPosition)
        .with(extra, [&](auto& request) { *request.mutable_to() = to_proto(pose); })
        .invoke();
}

std::vector<double> ArmClient::get_joint_positions(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetJointPositions)
        .with(extra)
        .invoke([](auto& response) {
            return std::vector<double>(response.positions().values().begin(),
                                       response.positions().values().end());
        });
}

void ArmClient::move_to_joint_positions(const std::vector<double>& positions,
                                        const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveToJointPositions)
        .with(extra,
              [&](auto& request) {
                  *(request.mutable_positions()->mutable_values()) = {positions.begin(),
                                                                      positions.end()};
              })
        .invoke();
}

void ArmClient::move_through_joint_positions(const std::vector<std::vector<double>>& positions,
                                             const Arm::MoveOptions& options,
                                             const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveThroughJointPositions)
        .with(extra,
              [&](viam::component::arm::v1::MoveThroughJointPositionsRequest& request) {
                  if (options.max_vel_degs_per_sec) {
                      request.mutable_options()->set_max_vel_degs_per_sec(
                          *options.max_vel_degs_per_sec);
                  }

                  if (options.max_acc_degs_per_sec2) {
                      request.mutable_options()->set_max_acc_degs_per_sec2(
                          *options.max_acc_degs_per_sec2);
                  }

                  for (const auto& pos : positions) {
                      viam::component::arm::v1::JointPositions jpos;
                      jpos.mutable_values()->Add(pos.begin(), pos.end());
                      request.mutable_positions()->Add(std::move(jpos));
                  }
              })
        .invoke();
}

bool ArmClient::is_moving() {
    return make_client_helper(this, *stub_, &StubType::IsMoving).invoke([](auto& response) {
        return response.is_moving();
    });
}

void ArmClient::stop(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Stop).with(extra).invoke();
}

ProtoStruct ArmClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

::viam::sdk::KinematicsData ArmClient::get_kinematics(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetKinematics)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

::viam::sdk::KinematicsResponse ArmClient::get_kinematics_response(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetKinematics)
        .with(extra)
        .invoke([](auto& response) { return kinematics_response_from_proto(response); });
}

std::map<std::string, mesh> ArmClient::get_3d_models(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Get3DModels)
        .with(extra)
        .invoke([](auto& response) {
            std::map<std::string, mesh> models;
            for (const auto& entry : response.models()) {
                models.emplace(entry.first, from_proto(entry.second));
            }
            return models;
        });
}

std::vector<GeometryConfig> ArmClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
