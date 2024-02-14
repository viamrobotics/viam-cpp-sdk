#include <memory>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/motion/motion.hpp>
#include <viam/sdk/services/motion/server.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

MotionServer::MotionServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)){};

::grpc::Status MotionServer::Move(::grpc::ServerContext* context,
                                  const ::viam::service::motion::v1::MoveRequest* request,
                                  ::viam::service::motion::v1::MoveResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::Move", this, request)([&](auto& helper, auto& motion) {
        std::shared_ptr<WorldState> ws;
        if (request->has_world_state()) {
            ws = std::make_shared<WorldState>(WorldState::from_proto(request->world_state()));
        }

        std::shared_ptr<Motion::constraints> constraints;
        if (request->has_constraints()) {
            constraints = std::make_shared<Motion::constraints>(
                Motion::constraints::from_proto(request->constraints()));
        }

        const bool success = motion->move(pose_in_frame::from_proto(request->destination()),
                                          Name::from_proto(request->component_name()),
                                          std::move(ws),
                                          std::move(constraints),
                                          helper.getExtra());
        response->set_success(success);
    });
};

::grpc::Status MotionServer::MoveOnMap(
    ::grpc::ServerContext* context,
    const ::viam::service::motion::v1::MoveOnMapRequest* request,
    ::viam::service::motion::v1::MoveOnMapResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::MoveOnMap", this, request)([&](auto& helper, auto& motion) {
        const auto destination = pose::from_proto(request->destination());
        auto component_name = Name::from_proto(request->component_name());
        auto slam_name = Name::from_proto(request->slam_service_name());
        const bool success = motion->move_on_map(
            destination, std::move(component_name), std::move(slam_name), helper.getExtra());

        response->set_success(success);
    });
}

::grpc::Status MotionServer::MoveOnGlobe(
    ::grpc::ServerContext* context,
    const ::viam::service::motion::v1::MoveOnGlobeRequest* request,
    ::viam::service::motion::v1::MoveOnGlobeResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::MoveOnGlobe", this, request)([&](auto& helper, auto& motion) {
        const auto destination = geo_point::from_proto(request->destination());
        auto component_name = Name::from_proto(request->component_name());
        auto movement_sensor_name = Name::from_proto(request->movement_sensor_name());
        std::vector<geo_obstacle> obstacles;

        for (const auto& obstacle : request->obstacles()) {
            obstacles.push_back(geo_obstacle::from_proto(obstacle));
        }

        boost::optional<double> heading;
        if (request->has_heading()) {
            heading = request->heading();
        }

        std::shared_ptr<motion_configuration> mc;
        if (request->has_motion_configuration()) {
            mc = std::make_shared<motion_configuration>(
                motion_configuration::from_proto(request->motion_configuration()));
        }

        const std::string execution_id = motion->move_on_globe(destination,
                                                               heading,
                                                               std::move(component_name),
                                                               std::move(movement_sensor_name),
                                                               std::move(obstacles),
                                                               std::move(mc),
                                                               std::move(helper.getExtra()));

        *response->mutable_execution_id() = execution_id;
    });
}

::grpc::Status MotionServer::GetPose(
    ::grpc::ServerContext* context,
    const ::viam::service::motion::v1::GetPoseRequest* request,
    ::viam::service::motion::v1::GetPoseResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::GetPose", this, request)([&](auto& helper, auto& motion) {
        const auto& component_name = Name::from_proto(request->component_name());
        const std::string& destination_frame = request->destination_frame();
        std::vector<WorldState::transform> supplemental_transforms;
        for (const auto& proto_transform : request->supplemental_transforms()) {
            supplemental_transforms.push_back(WorldState::transform::from_proto(proto_transform));
        }
        const pose_in_frame pose = motion->get_pose(
            component_name, destination_frame, supplemental_transforms, helper.getExtra());
        *response->mutable_pose() = pose.to_proto();
    });
};

::grpc::Status MotionServer::GetPlan(
    ::grpc::ServerContext* context,
    const ::viam::service::motion::v1::GetPlanRequest* request,
    ::viam::service::motion::v1::GetPlanResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::GetPlan", this, request)([&](auto& helper, auto& motion) {
        const auto& component_name = Name::from_proto(request->component_name());
        Motion::plan_with_status plan;
        std::vector<Motion::plan_with_status> replan_history;
        const bool last_plan_only(request->last_plan_only());
        const bool has_execution_id(request->has_execution_id());

        if (last_plan_only && has_execution_id) {
            plan = motion->get_plan(component_name, request->execution_id());
        } else if (last_plan_only) {
            plan = motion->get_latest_plan(component_name, helper.getExtra());
        } else if (has_execution_id) {
            const auto& res = motion->get_plan_with_replan_history(
                component_name, request->execution_id(), helper.getExtra());
            plan = res.first;
            replan_history = res.second;
        } else {
            const auto& res =
                motion->get_latest_plan_with_replan_history(component_name, helper.getExtra());
            plan = res.first;
            replan_history = res.second;
        }

        *response->mutable_current_plan_with_status() = plan.to_proto();
        for (const auto& p : replan_history) {
            *response->mutable_replan_history()->Add() = p.to_proto();
        }
    });
}

::grpc::Status MotionServer::ListPlanStatuses(
    ::grpc::ServerContext* context,
    const service::motion::v1::ListPlanStatusesRequest* request,
    service::motion::v1::ListPlanStatusesResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::ListPlanStatuses", this, request)([&](auto& helper, auto& motion) {
        std::vector<Motion::plan_status_with_id> statuses;
        if (request->only_active_plans()) {
            statuses = motion->list_active_plan_statuses(helper.getExtra());
        } else {
            statuses = motion->list_plan_statuses(helper.getExtra());
        }

        for (const auto& status : statuses) {
            *response->mutable_plan_statuses_with_ids()->Add() = status.to_proto();
        }
    });
}

::grpc::Status MotionServer::StopPlan(
    ::grpc::ServerContext* context,
    const ::viam::service::motion::v1::StopPlanRequest* request,
    ::viam::service::motion::v1::StopPlanResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::StopPlan", this, request)([&](auto& helper, auto& motion) {
        const auto& component_name = Name::from_proto(request->component_name());

        motion->stop_plan(component_name, helper.getExtra());
    });
}

::grpc::Status MotionServer::DoCommand(::grpc::ServerContext* context,
                                       const ::viam::common::v1::DoCommandRequest* request,
                                       ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::DoCommand", this, request)([&](auto& helper, auto& motion) {
        const AttributeMap result = motion->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
};

}  // namespace sdk
}  // namespace viam
