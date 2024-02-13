#include <viam/sdk/services/motion/client.hpp>

#include <math.h>

#include <grpcpp/support/status.h>

#include <viam/api/service/motion/v1/motion.grpc.pb.h>
#include <viam/api/service/motion/v1/motion.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/motion/motion.hpp>

namespace viam {
namespace sdk {

MotionClient::MotionClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Motion(std::move(name)),
      stub_(service::motion::v1::MotionService::NewStub(channel)),
      channel_(std::move(channel)){};

bool MotionClient::move(const pose_in_frame& destination,
                        const Name& component_name,
                        std::shared_ptr<WorldState> world_state,
                        std::shared_ptr<Motion::constraints> constraints,
                        const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::Move)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_component_name() = component_name.to_proto();
                  *request.mutable_destination() = destination.to_proto();
                  if (constraints) {
                      *request.mutable_constraints() = constraints->to_proto();
                  }
                  if (world_state) {
                      *request.mutable_world_state() = world_state->to_proto();
                  }
              })
        .invoke([](auto& response) { return response.success(); });
}

bool MotionClient::move_on_map(const pose& destination,
                               const Name& component_name,
                               const Name& slam_name,
                               const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveOnMap)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_destination() = destination.to_proto();
                  *request.mutable_component_name() = component_name.to_proto();
                  *request.mutable_slam_service_name() = slam_name.to_proto();
              })
        .invoke([](auto& response) { return response.success(); });
}

std::string MotionClient::move_on_globe(const geo_point& destination,
                                        const boost::optional<double>& heading,
                                        const Name& component_name,
                                        const Name& movement_sensor_name,
                                        const std::vector<geo_obstacle>& obstacles,
                                        std::shared_ptr<motion_configuration> motion_configuration,
                                        const AttributeMap& extra) {
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
                      *request.mutable_motion_configuration() = motion_configuration->to_proto();
                  }
              })
        .invoke([](auto& response) { return response.execution_id(); });
}

pose_in_frame MotionClient::get_pose(
    const Name& component_name,
    const std::string& destination_frame,
    const std::vector<WorldState::transform>& supplemental_transforms,
    const AttributeMap& extra) {
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

void MotionClient::stop_plan(const Name& name, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::StopPlan)
        .with(extra, [&](auto& request) { *request.mutable_component_name() = name.to_proto(); })
        .invoke();
}

std::pair<Motion::plan_with_status, std::vector<Motion::plan_with_status>> MotionClient::get_plan_(
    const Name& component_name,
    boost::optional<std::string> execution_id,
    bool last_plan_only,
    const AttributeMap& extra) {
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
                Motion::plan_with_status::from_proto(response.current_plan_with_status()),
                Motion::plan_with_status::from_proto(response.replan_history()));
        });
}

Motion::plan_with_status MotionClient::get_plan(const Name& name,
                                                const std::string& execution_id,
                                                const AttributeMap& extra) {
    return get_plan_(name, execution_id, true, extra).first;
}

Motion::plan_with_status MotionClient::get_latest_plan(const Name& name,
                                                       const AttributeMap& extra) {
    return get_plan_(name, {}, true, extra).first;
}

std::pair<Motion::plan_with_status, std::vector<Motion::plan_with_status>>
MotionClient::get_plan_with_replan_history(const Name& name,
                                           const std::string& execution_id,
                                           const AttributeMap& extra) {
    return get_plan_(name, execution_id, false, extra);
}

std::pair<Motion::plan_with_status, std::vector<Motion::plan_with_status>>
MotionClient::get_latest_plan_with_replan_history(const Name& name, const AttributeMap& extra) {
    return get_plan_(name, {}, false, extra);
}

std::vector<Motion::plan_status_with_id> MotionClient::list_plan_statuses_(
    bool only_active_plans, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::ListPlanStatuses)
        .with(extra, [&](auto& request) { request.set_only_active_plans(only_active_plans); })
        .invoke([](auto& response) {
            std::vector<Motion::plan_status_with_id> statuses;
            for (const auto& proto : response.plan_statuses_with_ids()) {
                statuses.push_back(Motion::plan_status_with_id::from_proto(proto));
            }

            return statuses;
        });
}

std::vector<Motion::plan_status_with_id> MotionClient::list_plan_statuses(
    const AttributeMap& extra) {
    return list_plan_statuses_(false, extra);
}

std::vector<Motion::plan_status_with_id> MotionClient::list_active_plan_statuses(
    const AttributeMap& extra) {
    return list_plan_statuses_(true, extra);
}

AttributeMap MotionClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace sdk
}  // namespace viam
