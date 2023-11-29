#include <memory>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/motion/motion.hpp>
#include <viam/sdk/services/motion/server.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

MotionServer::MotionServer() : ResourceServer(std::make_shared<ResourceManager>()){};
MotionServer::MotionServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

::grpc::Status MotionServer::Move(::grpc::ServerContext* context,
                                  const ::viam::service::motion::v1::MoveRequest* request,
                                  ::viam::service::motion::v1::MoveResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Move] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motion> motion = std::dynamic_pointer_cast<Motion>(rb);

    const pose_in_frame destination = pose_in_frame::from_proto(request->destination());
    const Name name = Name::from_proto(request->component_name());
    std::shared_ptr<WorldState> ws;
    if (request->has_world_state()) {
        ws = std::make_shared<WorldState>(WorldState::from_proto(request->world_state()));
    }

    std::shared_ptr<Motion::constraints> constraints;
    if (request->has_constraints()) {
        constraints = std::make_shared<Motion::constraints>(
            Motion::constraints::from_proto(request->constraints()));
    }

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }
    const bool success = motion->move(std::move(destination),
                                      std::move(name),
                                      std::move(ws),
                                      std::move(constraints),
                                      std::move(extra));
    response->set_success(success);

    return ::grpc::Status();
};

::grpc::Status MotionServer::MoveOnMap(::grpc::ServerContext* context,
                                       const ::viam::service::motion::v1::MoveOnMapRequest* request,
                                       ::viam::service::motion::v1::MoveOnMapResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MoveOnMap] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motion> motion = std::dynamic_pointer_cast<Motion>(rb);

    const auto& destination = pose::from_proto(request->destination());
    const auto& component_name = Name::from_proto(request->component_name());
    const auto& slam_name = Name::from_proto(request->slam_service_name());

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }
    const bool success = motion->move_on_map(
        std::move(destination), std::move(component_name), std::move(slam_name), std::move(extra));

    response->set_success(success);

    return ::grpc::Status();
};

::grpc::Status MotionServer::MoveOnGlobe(
    ::grpc::ServerContext* context,
    const ::viam::service::motion::v1::MoveOnGlobeRequest* request,
    ::viam::service::motion::v1::MoveOnGlobeResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MoveOnGlobe] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motion> motion = std::dynamic_pointer_cast<Motion>(rb);

    const auto& destination = geo_point::from_proto(request->destination());
    const auto& component_name = Name::from_proto(request->component_name());
    const auto& movement_sensor_name = Name::from_proto(request->movement_sensor_name());
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

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const bool success = motion->move_on_globe(std::move(destination),
                                               std::move(heading),
                                               std::move(component_name),
                                               std::move(movement_sensor_name),
                                               std::move(obstacles),
                                               std::move(mc),
                                               std::move(extra));

    response->set_success(success);

    return ::grpc::Status();
};

::grpc::Status MotionServer::GetPose(::grpc::ServerContext* context,
                                     const ::viam::service::motion::v1::GetPoseRequest* request,
                                     ::viam::service::motion::v1::GetPoseResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetPose] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motion> motion = std::dynamic_pointer_cast<Motion>(rb);

    const auto& component_name = Name::from_proto(request->component_name());
    const std::string& destination_frame = request->destination_frame();
    std::vector<WorldState::transform> supplemental_transforms;
    for (const auto& proto_transform : request->supplemental_transforms()) {
        supplemental_transforms.push_back(WorldState::transform::from_proto(proto_transform));
    }
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const pose_in_frame pose = motion->get_pose(std::move(component_name),
                                                std::move(destination_frame),
                                                std::move(supplemental_transforms),
                                                std::move(extra));

    *response->mutable_pose() = pose.to_proto();

    return ::grpc::Status();
};

::grpc::Status MotionServer::GetPlan(::grpc::ServerContext* context,
                                     const ::viam::service::motion::v1::GetPlanRequest* request,
                                     ::viam::service::motion::v1::GetPlanResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetPose] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motion> motion = std::dynamic_pointer_cast<Motion>(rb);

    const auto& component_name = Name::from_proto(request->component_name());
    boost::optional<std::string> execution_id;
    if (request->has_execution_id()) {
        execution_id = request->execution_id();
    }
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    Motion::plan_with_status plan;
    std::vector<Motion::plan_with_status> replan_history;
    bool last_plan_only(request->last_plan_only());
    bool has_execution_id(request->has_execution_id());

    if (last_plan_only && has_execution_id) {
        plan = motion->get_plan(component_name, request->execution_id());
    } else if (last_plan_only) {
        plan = motion->get_latest_plan(component_name, extra);
    } else if (has_execution_id) {
        const auto& res =
            motion->get_plan_with_replan_history(component_name, request->execution_id(), extra);
        plan = res.first;
        replan_history = res.second;
    } else {
        const auto& res = motion->get_latest_plan_with_replan_history(component_name, extra);
        plan = res.first;
        replan_history = res.second;
    }

    *response->mutable_current_plan_with_status() = plan.to_proto();
    for (const auto& p : replan_history) {
        *response->mutable_replan_history()->Add() = p.to_proto();
    }

    return ::grpc::Status();
}

::grpc::Status MotionServer::ListPlanStatuses(
    ::grpc::ServerContext* context,
    const service::motion::v1::ListPlanStatusesRequest* request,
    service::motion::v1::ListPlanStatusesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetPose] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motion> motion = std::dynamic_pointer_cast<Motion>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    std::vector<Motion::plan_status_with_id> statuses;
    if (request->only_active_plans()) {
        statuses = motion->list_active_plan_statuses(extra);
    } else {
        statuses = motion->list_plan_statuses(extra);
    }

    for (const auto& status : statuses) {
        *response->mutable_plan_statuses_with_ids()->Add() = status.to_proto();
    }

    return ::grpc::Status();
}

::grpc::Status MotionServer::StopPlan(::grpc::ServerContext* context,
                                      const ::viam::service::motion::v1::StopPlanRequest* request,
                                      ::viam::service::motion::v1::StopPlanResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetPose] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motion> motion = std::dynamic_pointer_cast<Motion>(rb);

    const auto& component_name = Name::from_proto(request->component_name());
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    motion->stop_plan(component_name, extra);

    return ::grpc::Status();
}

::grpc::Status MotionServer::DoCommand(::grpc::ServerContext* context,
                                       const ::viam::common::v1::DoCommandRequest* request,
                                       ::viam::common::v1::DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [DoCommand] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motion> motion = std::dynamic_pointer_cast<Motion>(rb);
    const AttributeMap result = motion->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
};

void MotionServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
