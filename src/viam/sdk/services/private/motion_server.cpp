#include <memory>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/motion.hpp>
#include <viam/sdk/services/private/motion_server.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {
namespace impl {

service::motion::v1::PlanState to_proto(const Motion::plan_state& state) {
    switch (state) {
        case Motion::plan_state::k_failed: {
            return service::motion::v1::PLAN_STATE_FAILED;
        }
        case Motion::plan_state::k_succeeded: {
            return service::motion::v1::PLAN_STATE_SUCCEEDED;
        }
        case Motion::plan_state::k_in_progress: {
            return service::motion::v1::PLAN_STATE_IN_PROGRESS;
        }
        case Motion::plan_state::k_stopped: {
            return service::motion::v1::PLAN_STATE_STOPPED;
        }
        default: {
            throw Exception(ErrorCondition::k_not_supported,
                            "Invalid plan_state to encode to proto");
        }
    }
}

service::motion::v1::PlanStatus to_proto(const Motion::plan_status& ps) {
    service::motion::v1::PlanStatus proto;
    *proto.mutable_timestamp() = time_pt_to_timestamp(ps.timestamp);
    if (ps.reason) {
        *proto.mutable_reason() = *ps.reason;
    }
    proto.set_state(to_proto(ps.state));

    return proto;
}

service::motion::v1::PlanStep to_proto(const Motion::steps::step& step) {
    service::motion::v1::PlanStep proto;
    for (const auto& kv : step) {
        service::motion::v1::ComponentState cs;
        *cs.mutable_pose() = v2::to_proto(kv.second);
        proto.mutable_step()->insert({kv.first, cs});
    }

    return proto;
}

service::motion::v1::Plan to_proto(const Motion::plan& plan) {
    service::motion::v1::Plan proto;
    *proto.mutable_id() = plan.id;
    *proto.mutable_component_name() = plan.component_name.to_proto();
    *proto.mutable_execution_id() = plan.execution_id;
    for (const auto& step : plan.steps.steps) {
        *proto.mutable_steps()->Add() = to_proto(step);
    }

    return proto;
}

service::motion::v1::PlanWithStatus to_proto(const Motion::plan_with_status& pws) {
    service::motion::v1::PlanWithStatus proto;
    *proto.mutable_plan() = to_proto(pws.plan);
    *proto.mutable_status() = to_proto(pws.status);
    for (const auto& sh : pws.status_history) {
        *proto.mutable_status_history()->Add() = to_proto(sh);
    }

    return proto;
}

service::motion::v1::PlanStatusWithID to_proto(const Motion::plan_status_with_id& pswi) {
    service::motion::v1::PlanStatusWithID proto;

    *proto.mutable_execution_id() = pswi.execution_id;
    *proto.mutable_component_name() = pswi.component_name.to_proto();
    *proto.mutable_plan_id() = pswi.plan_id;
    *proto.mutable_status() = to_proto(pswi.status);

    return proto;
}

obstacle_detector from_proto(const service::motion::v1::ObstacleDetector& proto) {
    obstacle_detector oc;
    oc.vision_service = Name::from_proto(proto.vision_service());
    oc.camera = Name::from_proto(proto.camera());
    return oc;
}

motion_configuration from_proto(const service::motion::v1::MotionConfiguration& proto) {
    motion_configuration mc;

    for (const service::motion::v1::ObstacleDetector& od : proto.obstacle_detectors()) {
        mc.obstacle_detectors.push_back(from_proto(od));
    }

    if (proto.has_position_polling_frequency_hz()) {
        mc.position_polling_frequency_hz = proto.position_polling_frequency_hz();
    }

    if (proto.has_obstacle_polling_frequency_hz()) {
        mc.obstacle_polling_frequency_hz = proto.obstacle_polling_frequency_hz();
    }

    if (proto.has_plan_deviation_m()) {
        mc.plan_deviation_m = proto.plan_deviation_m();
    }

    if (proto.has_linear_m_per_sec()) {
        mc.linear_m_per_sec = proto.linear_m_per_sec();
    }

    if (proto.has_angular_degs_per_sec()) {
        mc.angular_degs_per_sec = proto.angular_degs_per_sec();
    }

    return mc;
}

MotionServer::MotionServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

Motion::constraints from_proto(const service::motion::v1::Constraints& proto) {
    std::vector<Motion::linear_constraint> lcs;
    for (const auto& proto_lc : proto.linear_constraint()) {
        Motion::linear_constraint lc;
        lc.orientation_tolerance_degs = proto_lc.orientation_tolerance_degs();
        lc.line_tolerance_mm = proto_lc.line_tolerance_mm();
        lcs.push_back(lc);
    }

    std::vector<Motion::orientation_constraint> ocs;
    for (const auto& proto_oc : proto.orientation_constraint()) {
        Motion::orientation_constraint oc;
        oc.orientation_tolerance_degs = proto_oc.orientation_tolerance_degs();
        ocs.push_back(oc);
    }

    std::vector<Motion::collision_specification> css;
    for (const auto& proto_cs : proto.collision_specification()) {
        std::vector<Motion::collision_specification::allowed_frame_collisions> allows;
        for (const auto& proto_allow : proto_cs.allows()) {
            Motion::collision_specification::allowed_frame_collisions allow;
            allow.frame1 = proto_allow.frame1();
            allow.frame2 = proto_allow.frame2();
            allows.push_back(allow);
        }
        Motion::collision_specification cs;
        cs.allows = allows;
        css.push_back(cs);
    }

    Motion::constraints constraints;
    constraints.linear_constraints = lcs;
    constraints.orientation_constraints = ocs;
    constraints.collision_specifications = css;

    return constraints;
}

::grpc::Status MotionServer::Move(::grpc::ServerContext*,
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
            constraints = std::make_shared<Motion::constraints>(from_proto(request->constraints()));
        }

        const bool success = motion->move(v2::from_proto(request->destination()),
                                          Name::from_proto(request->component_name()),
                                          std::move(ws),
                                          std::move(constraints),
                                          helper.getExtra());
        response->set_success(success);
    });
};

::grpc::Status MotionServer::MoveOnMap(
    ::grpc::ServerContext*,
    const ::viam::service::motion::v1::MoveOnMapRequest* request,
    ::viam::service::motion::v1::MoveOnMapResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::MoveOnMap", this, request)([&](auto& helper, auto& motion) {
        const auto destination = v2::from_proto(request->destination());
        const auto component_name = Name::from_proto(request->component_name());
        const auto slam_name = Name::from_proto(request->slam_service_name());

        std::shared_ptr<motion_configuration> mc;
        if (request->has_motion_configuration()) {
            mc =
                std::make_shared<motion_configuration>(from_proto(request->motion_configuration()));
        }

        std::vector<GeometryConfig> obstacles;
        for (const auto& obstacle : request->obstacles()) {
            obstacles.push_back(v2::from_proto(obstacle));
        }

        const std::string execution_id = motion->move_on_map(
            destination, component_name, slam_name, mc, obstacles, helper.getExtra());

        *response->mutable_execution_id() = execution_id;
    });
}

::grpc::Status MotionServer::MoveOnGlobe(
    ::grpc::ServerContext*,
    const ::viam::service::motion::v1::MoveOnGlobeRequest* request,
    ::viam::service::motion::v1::MoveOnGlobeResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::MoveOnGlobe", this, request)([&](auto& helper, auto& motion) {
        const auto destination = v2::from_proto(request->destination());
        const auto component_name = Name::from_proto(request->component_name());
        const auto movement_sensor_name = Name::from_proto(request->movement_sensor_name());
        std::vector<geo_geometry> obstacles;
        std::vector<geo_geometry> bounding_regions;

        for (const auto& obstacle : request->obstacles()) {
            obstacles.push_back(geo_geometry::from_proto(obstacle));
        }

        boost::optional<double> heading;
        if (request->has_heading()) {
            heading = request->heading();
        }

        std::shared_ptr<motion_configuration> mc;
        if (request->has_motion_configuration()) {
            mc =
                std::make_shared<motion_configuration>(from_proto(request->motion_configuration()));
        }

        for (const auto& bounding_region : request->bounding_regions()) {
            bounding_regions.push_back(geo_geometry::from_proto(bounding_region));
        }

        const std::string execution_id = motion->move_on_globe(destination,
                                                               heading,
                                                               component_name,
                                                               movement_sensor_name,
                                                               obstacles,
                                                               mc,
                                                               bounding_regions,
                                                               helper.getExtra());

        *response->mutable_execution_id() = execution_id;
    });
}

::grpc::Status MotionServer::GetPose(
    ::grpc::ServerContext*,
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
        *response->mutable_pose() = v2::to_proto(pose);
    });
};

::grpc::Status MotionServer::GetPlan(
    ::grpc::ServerContext*,
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

        *response->mutable_current_plan_with_status() = to_proto(plan);
        for (const auto& p : replan_history) {
            *response->mutable_replan_history()->Add() = to_proto(p);
        }
    });
}

::grpc::Status MotionServer::ListPlanStatuses(
    ::grpc::ServerContext*,
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
            *response->mutable_plan_statuses_with_ids()->Add() = to_proto(status);
        }
    });
}

::grpc::Status MotionServer::StopPlan(::grpc::ServerContext*,
                                      const ::viam::service::motion::v1::StopPlanRequest* request,
                                      ::viam::service::motion::v1::StopPlanResponse*) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::StopPlan", this, request)([&](auto& helper, auto& motion) {
        const auto& component_name = Name::from_proto(request->component_name());

        motion->stop_plan(component_name, helper.getExtra());
    });
}

::grpc::Status MotionServer::DoCommand(::grpc::ServerContext*,
                                       const ::viam::common::v1::DoCommandRequest* request,
                                       ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Motion>(
        "MotionServer::DoCommand", this, request)([&](auto&, auto& motion) {
        const ProtoStruct result = motion->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
