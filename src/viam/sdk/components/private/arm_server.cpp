#include <viam/sdk/components/private/arm_server.hpp>

#include <viam/sdk/common/service_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

ArmServer::ArmServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status ArmServer::GetEndPosition(
    ::grpc::ServerContext*,
    const ::viam::component::arm::v1::GetEndPositionRequest* request,
    ::viam::component::arm::v1::GetEndPositionResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::GetEndPosition", this, request)([&](auto& helper, auto& arm) {
        const pose p = arm->get_end_position(helper.getExtra());
        *response->mutable_pose() = p.to_proto();
    });
}

::grpc::Status ArmServer::MoveToPosition(
    ::grpc::ServerContext*,
    const ::viam::component::arm::v1::MoveToPositionRequest* request,
    ::viam::component::arm::v1::MoveToPositionResponse*) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::MoveToPosition", this, request)([&](auto& helper, auto& arm) {
        arm->move_to_position(pose::from_proto(request->to()), helper.getExtra());
    });
}

::grpc::Status ArmServer::GetJointPositions(
    ::grpc::ServerContext*,
    const ::viam::component::arm::v1::GetJointPositionsRequest* request,
    ::viam::component::arm::v1::GetJointPositionsResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::GetJointPositions", this, request)([&](auto& helper, auto& arm) {
        const std::vector<double> positions = arm->get_joint_positions(helper.getExtra());
        *(response->mutable_positions()->mutable_values()) = {positions.begin(), positions.end()};
    });
}

::grpc::Status ArmServer::MoveToJointPositions(
    ::grpc::ServerContext*,
    const ::viam::component::arm::v1::MoveToJointPositionsRequest* request,
    ::viam::component::arm::v1::MoveToJointPositionsResponse*) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::MoveToJointPositions", this, request)([&](auto& helper, auto& arm) {
        arm->move_to_joint_positions(
            {request->positions().values().begin(), request->positions().values().end()},
            helper.getExtra());
    });
}

::grpc::Status ArmServer::MoveThroughJointPositions(
    ::grpc::ServerContext*,
    const ::viam::component::arm::v1::MoveThroughJointPositionsRequest* request,
    ::viam::component::arm::v1::MoveThroughJointPositionsResponse*) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::MoveThroughJointPositions", this, request)([&](auto& helper, auto& arm) {
        std::vector<std::vector<double>> positions;

        positions.reserve(request->positions_size());
        for (const auto& values : request->positions()) {
            positions.push_back({values.values().begin(), values.values().end()});
        }

        Arm::MoveOptions opts;
        if (request->options().has_max_vel_degs_per_sec()) {
            opts.max_vel_degs_per_sec = request->options().max_vel_degs_per_sec();
        }

        if (request->options().has_max_acc_degs_per_sec2()) {
            opts.max_acc_degs_per_sec2 = request->options().max_acc_degs_per_sec2();
        }

        arm->move_through_joint_positions(positions, opts, helper.getExtra());
    });
}

::grpc::Status ArmServer::Stop(::grpc::ServerContext*,
                               const ::viam::component::arm::v1::StopRequest* request,
                               ::viam::component::arm::v1::StopResponse*) noexcept {
    return make_service_helper<Arm>("ArmServer::Stop", this, request)(
        [&](auto& helper, auto& arm) { arm->stop(helper.getExtra()); });
}

::grpc::Status ArmServer::IsMoving(
    ::grpc::ServerContext*,
    const ::viam::component::arm::v1::IsMovingRequest* request,
    ::viam::component::arm::v1::IsMovingResponse* response) noexcept {
    return make_service_helper<Arm>("ArmServer::IsMoving", this, request)(
        [&](auto&, auto& arm) { response->set_is_moving(arm->is_moving()); });
}

::grpc::Status ArmServer::DoCommand(::grpc::ServerContext*,
                                    const ::viam::common::v1::DoCommandRequest* request,
                                    ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Arm>("ArmServer::DoCommand", this, request)([&](auto&, auto& arm) {
        const ProtoStruct result = arm->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

::grpc::Status ArmServer::GetKinematics(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetKinematicsRequest* request,
    ::viam::common::v1::GetKinematicsResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::GetKinematics", this, request)([&](auto& helper, auto& arm) {
        const Arm::KinematicsData result = arm->get_kinematics(helper.getExtra());

        struct Visitor {
            using FileFormat = common::v1::KinematicsFileFormat;
            auto operator()(const Arm::KinematicsDataUnspecified&) const noexcept {
                return FileFormat::KINEMATICS_FILE_FORMAT_UNSPECIFIED;
            }

            auto operator()(const Arm::KinematicsDataSVA&) const noexcept {
                return FileFormat::KINEMATICS_FILE_FORMAT_SVA;
            }

            auto operator()(const Arm::KinematicsDataURDF&) const noexcept {
                return FileFormat::KINEMATICS_FILE_FORMAT_URDF;
            }
        } visitor;

        boost::apply_visitor(
            [&](const auto& v) {
                response->set_format(visitor(v));
                response->mutable_kinematics_data()->assign(v.bytes.begin(), v.bytes.end());
            },
            result);
    });
}

::grpc::Status ArmServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::GetGeometries", this, request)([&](auto& helper, auto& arm) {
        const std::vector<GeometryConfig> geometries = arm->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
