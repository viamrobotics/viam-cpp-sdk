#include <viam/sdk/services/private/motion_client.hpp>

#include <math.h>

#include <grpcpp/support/status.h>

#include <viam/api/service/motion/v1/motion.grpc.pb.h>
#include <viam/api/service/motion/v1/motion.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/motion.hpp>

namespace viam {
namespace sdk {
namespace impl {

service::motion::v1::ObstacleDetector to_proto(const obstacle_detector& od) {
    service::motion::v1::ObstacleDetector proto;
    *proto.mutable_vision_service() = od.vision_service.to_proto();
    *proto.mutable_camera() = od.camera.to_proto();
    return proto;
}

service::motion::v1::MotionConfiguration to_proto(const motion_configuration& mc) {
    service::motion::v1::MotionConfiguration proto;

    for (const obstacle_detector& od : mc.obstacle_detectors) {
        *proto.mutable_obstacle_detectors()->Add() = to_proto(od);
    }

    if (mc.position_polling_frequency_hz && !isnan(*mc.position_polling_frequency_hz)) {
        proto.set_position_polling_frequency_hz(*mc.position_polling_frequency_hz);
    }

    if (mc.obstacle_polling_frequency_hz && !isnan(*mc.obstacle_polling_frequency_hz)) {
        proto.set_obstacle_polling_frequency_hz(*mc.obstacle_polling_frequency_hz);
    }

    if (mc.plan_deviation_m && !isnan(*mc.plan_deviation_m)) {
        proto.set_plan_deviation_m(*mc.plan_deviation_m);
    }

    if (mc.linear_m_per_sec && !isnan(*mc.linear_m_per_sec)) {
        proto.set_linear_m_per_sec(*mc.linear_m_per_sec);
    }

    if (mc.angular_degs_per_sec && !isnan(*mc.angular_degs_per_sec)) {
        proto.set_angular_degs_per_sec(*mc.angular_degs_per_sec);
    }

    return proto;
}

service::motion::v1::Constraints to_proto(const Motion::constraints& cs) {
    service::motion::v1::Constraints proto;
    for (const auto& lc : cs.linear_constraints) {
        service::motion::v1::LinearConstraint proto_lc;
        proto_lc.set_line_tolerance_mm(lc.line_tolerance_mm);
        proto_lc.set_orientation_tolerance_degs(lc.orientation_tolerance_degs);
        *proto.mutable_linear_constraint()->Add() = std::move(proto_lc);
    }

    for (const auto& oc : cs.orientation_constraints) {
        service::motion::v1::OrientationConstraint proto_oc;
        proto_oc.set_orientation_tolerance_degs(oc.orientation_tolerance_degs);
        *proto.mutable_orientation_constraint()->Add() = std::move(proto_oc);
    }

    for (const auto& spec : cs.collision_specifications) {
        service::motion::v1::CollisionSpecification proto_cs;
        for (const auto& allow : spec.allows) {
            service::motion::v1::CollisionSpecification::AllowedFrameCollisions proto_allow;
            *proto_allow.mutable_frame1() = allow.frame1;
            *proto_allow.mutable_frame2() = allow.frame2;
            *proto_cs.mutable_allows()->Add() = std::move(proto_allow);
        }
        *proto.mutable_collision_specification()->Add() = std::move(proto_cs);
    }

    return proto;
}

Motion::plan_state from_proto(const service::motion::v1::PlanState& proto) {
    switch (proto) {
        case service::motion::v1::PLAN_STATE_FAILED: {
            return Motion::plan_state::k_failed;
        }
        case service::motion::v1::PLAN_STATE_SUCCEEDED: {
            return Motion::plan_state::k_succeeded;
        }
        case service::motion::v1::PLAN_STATE_IN_PROGRESS: {
            return Motion::plan_state::k_in_progress;
        }
        case service::motion::v1::PLAN_STATE_STOPPED: {
            return Motion::plan_state::k_stopped;
        }
        default: {
            throw Exception(ErrorCondition::k_not_supported, "Invalid proto PlanState to encode");
        }
    }
}

Motion::plan_status from_proto(const service::motion::v1::PlanStatus& proto) {
    Motion::plan_status mps;
    mps.state = from_proto(proto.state());
    if (proto.has_reason()) {
        mps.reason = proto.reason();
    }
    mps.timestamp = timestamp_to_time_pt(proto.timestamp());

    return mps;
}

std::vector<Motion::plan_status> from_proto(
    const google::protobuf::RepeatedPtrField<service::motion::v1::PlanStatus>& proto) {
    std::vector<Motion::plan_status> pss;
    for (const auto& ps : proto) {
        pss.push_back(from_proto(ps));
    }

    return pss;
}

Motion::plan_status_with_id from_proto(const service::motion::v1::PlanStatusWithID& proto) {
    Motion::plan_status_with_id pswi;
    pswi.execution_id = proto.execution_id();
    pswi.component_name = Name::from_proto(proto.component_name());
    pswi.plan_id = proto.plan_id();
    pswi.status = from_proto(proto.status());

    return pswi;
}

Motion::steps steps_from_proto(
    const google::protobuf::RepeatedPtrField<service::motion::v1::PlanStep>& proto) {
    using step = Motion::steps::step;
    std::vector<step> steps;
    for (const auto& ps : proto) {
        step step;
        for (const auto& component : ps.step()) {
            step.emplace(component.first, pose::from_proto(component.second.pose()));
        }
        steps.push_back(std::move(step));
    }

    return {steps};
}

Motion::plan plan_from_proto(const service::motion::v1::Plan& proto) {
    Motion::plan plan;
    plan.id = proto.id();
    plan.execution_id = proto.execution_id();
    plan.component_name = Name::from_proto(proto.component_name());
    plan.steps = steps_from_proto(proto.steps());
    return plan;
}

Motion::plan_with_status from_proto(const service::motion::v1::PlanWithStatus& proto) {
    Motion::plan_with_status pws;
    pws.plan = plan_from_proto(proto.plan());
    pws.status = from_proto(proto.status());
    pws.status_history = from_proto(proto.status_history());

    return pws;
}

std::vector<Motion::plan_with_status> from_proto(
    const google::protobuf::RepeatedPtrField<service::motion::v1::PlanWithStatus>& proto) {
    std::vector<Motion::plan_with_status> plans;
    for (const auto& plan : proto) {
        plans.push_back(from_proto(plan));
    }
    return plans;
}

MotionClient::MotionClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Motion(std::move(name)),
      stub_(service::motion::v1::MotionService::NewStub(channel)),
      channel_(std::move(channel)) {}

bool MotionClient::move(const pose_in_frame& destination,
                        const Name& component_name,
                        const std::shared_ptr<WorldState>& world_state,
                        const std::shared_ptr<Motion::constraints>& constraints,
                        const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Move)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_component_name() = component_name.to_proto();
                  *request.mutable_destination() = destination.to_proto();
                  if (constraints) {
                      *request.mutable_constraints() = to_proto(*constraints);
                  }
                  if (world_state) {
                      *request.mutable_world_state() = world_state->to_proto();
                  }
              })
        .invoke([](auto& response) { return response.success(); });
}

std::string MotionClient::move_on_map(
    const pose& destination,
    const Name& component_name,
    const Name& slam_name,
    const std::shared_ptr<motion_configuration>& motion_configuration,
    const std::vector<GeometryConfig>& obstacles,
    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveOnMap)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_destination() = destination.to_proto();
                  *request.mutable_component_name() = component_name.to_proto();
                  *request.mutable_slam_service_name() = slam_name.to_proto();

                  for (const auto& obstacle : obstacles) {
                      *request.mutable_obstacles()->Add() = obstacle.to_proto();
                  }

                  if (motion_configuration) {
                      *request.mutable_motion_configuration() = to_proto(*motion_configuration);
                  }
              })
        .invoke([](auto& response) { return response.execution_id(); });
}

std::string MotionClient::move_on_globe(
    const geo_point& destination,
    const boost::optional<double>& heading,
    const Name& component_name,
    const Name& movement_sensor_name,
    const std::vector<geo_geometry>& obstacles,
    const std::shared_ptr<motion_configuration>& motion_configuration,
    const std::vector<geo_geometry>& bounding_regions,
    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveOnGlobe)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_destination() = destination.to_proto();
                  *request.mutable_component_name() = component_name.to_proto();
                  *request.mutable_movement_sensor_name() = movement_sensor_name.to_proto();

                  if (heading && !isnan(*heading)) {
                      request.set_heading(*heading);
                  }

                  for (const auto& obstacle : obstacles) {
                      *request.mutable_obstacles()->Add() = obstacle.to_proto();
                  }

                  if (motion_configuration) {
                      *request.mutable_motion_configuration() = to_proto(*motion_configuration);
                  }

                  for (const auto& bounding_region : bounding_regions) {
                      *request.mutable_bounding_regions()->Add() = bounding_region.to_proto();
                  }
              })
        .invoke([](auto& response) { return response.execution_id(); });
}

pose_in_frame MotionClient::get_pose(
    const Name& component_name,
    const std::string& destination_frame,
    const std::vector<WorldState::transform>& supplemental_transforms,
    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPose)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_component_name() = component_name.to_proto();
                  *request.mutable_destination_frame() = destination_frame;
                  for (const auto& transform : supplemental_transforms) {
                      *request.mutable_supplemental_transforms()->Add() = transform.to_proto();
                  }
              })
        .invoke([](auto& response) { return pose_in_frame::from_proto(response.pose()); });
}

void MotionClient::stop_plan(const Name& name, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::StopPlan)
        .with(extra, [&](auto& request) { *request.mutable_component_name() = name.to_proto(); })
        .invoke();
}

std::pair<Motion::plan_with_status, std::vector<Motion::plan_with_status>> MotionClient::get_plan_(
    const Name& component_name,
    boost::optional<std::string> execution_id,
    bool last_plan_only,
    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPlan)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_component_name() = component_name.to_proto();
                  request.set_last_plan_only(last_plan_only);
                  if (execution_id) {
                      *request.mutable_execution_id() = *execution_id;
                  }
              })
        .invoke([](auto& response) {
            return std::pair<Motion::plan_with_status, std::vector<Motion::plan_with_status>>(
                from_proto(response.current_plan_with_status()),
                from_proto(response.replan_history()));
        });
}

Motion::plan_with_status MotionClient::get_plan(const Name& name,
                                                const std::string& execution_id,
                                                const ProtoStruct& extra) {
    return get_plan_(name, execution_id, true, extra).first;
}

Motion::plan_with_status MotionClient::get_latest_plan(const Name& name, const ProtoStruct& extra) {
    return get_plan_(name, {}, true, extra).first;
}

std::pair<Motion::plan_with_status, std::vector<Motion::plan_with_status>>
MotionClient::get_plan_with_replan_history(const Name& name,
                                           const std::string& execution_id,
                                           const ProtoStruct& extra) {
    return get_plan_(name, execution_id, false, extra);
}

std::pair<Motion::plan_with_status, std::vector<Motion::plan_with_status>>
MotionClient::get_latest_plan_with_replan_history(const Name& name, const ProtoStruct& extra) {
    return get_plan_(name, {}, false, extra);
}

std::vector<Motion::plan_status_with_id> MotionClient::list_plan_statuses_(
    bool only_active_plans, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::ListPlanStatuses)
        .with(extra, [&](auto& request) { request.set_only_active_plans(only_active_plans); })
        .invoke([](auto& response) {
            std::vector<Motion::plan_status_with_id> statuses;
            for (const auto& proto : response.plan_statuses_with_ids()) {
                statuses.push_back(from_proto(proto));
            }

            return statuses;
        });
}

std::vector<Motion::plan_status_with_id> MotionClient::list_plan_statuses(
    const ProtoStruct& extra) {
    return list_plan_statuses_(false, extra);
}

std::vector<Motion::plan_status_with_id> MotionClient::list_active_plan_statuses(
    const ProtoStruct& extra) {
    return list_plan_statuses_(true, extra);
}

ProtoStruct MotionClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
