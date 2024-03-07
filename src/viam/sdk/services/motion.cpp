#include <viam/sdk/services/motion.hpp>

#include <math.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/service/motion/v1/motion.grpc.pb.h>
#include <viam/api/service/motion/v1/motion.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

Motion::Motion(std::string name) : Service(std::move(name)){};

service::motion::v1::Constraints Motion::constraints::to_proto() const {
    service::motion::v1::Constraints proto;
    for (const auto& lc : linear_constraints) {
        service::motion::v1::LinearConstraint proto_lc;
        proto_lc.set_line_tolerance_mm(lc.line_tolerance_mm);
        proto_lc.set_orientation_tolerance_degs(lc.orientation_tolerance_degs);
        *proto.mutable_linear_constraint()->Add() = std::move(proto_lc);
    }

    for (const auto& oc : orientation_constraints) {
        service::motion::v1::OrientationConstraint proto_oc;
        proto_oc.set_orientation_tolerance_degs(oc.orientation_tolerance_degs);
        *proto.mutable_orientation_constraint()->Add() = std::move(proto_oc);
    }

    for (const auto& cs : collision_specifications) {
        service::motion::v1::CollisionSpecification proto_cs;
        for (const auto& allow : cs.allows) {
            service::motion::v1::CollisionSpecification::AllowedFrameCollisions proto_allow;
            *proto_allow.mutable_frame1() = allow.frame1;
            *proto_allow.mutable_frame2() = allow.frame2;
            *proto_cs.mutable_allows()->Add() = std::move(proto_allow);
        }
        *proto.mutable_collision_specification()->Add() = std::move(proto_cs);
    }

    return proto;
}

Motion::constraints Motion::constraints::from_proto(const service::motion::v1::Constraints& proto) {
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

API Motion::api() const {
    return API::get<Motion>();
}

API API::traits<Motion>::api() {
    return {kRDK, kService, "motion"};
}

service::motion::v1::ObstacleDetector obstacle_detector::to_proto() const {
    service::motion::v1::ObstacleDetector proto;
    *proto.mutable_vision_service() = vision_service.to_proto();
    *proto.mutable_camera() = camera.to_proto();
    return proto;
}

obstacle_detector obstacle_detector::from_proto(
    const service::motion::v1::ObstacleDetector& proto) {
    obstacle_detector oc;
    oc.vision_service = Name::from_proto(proto.vision_service());
    oc.camera = Name::from_proto(proto.camera());
    return oc;
}

bool operator==(const obstacle_detector& lhs, const obstacle_detector& rhs) {
    return lhs.vision_service == rhs.vision_service && lhs.camera == rhs.camera;
}

bool operator==(const Motion::steps& lhs, const Motion::steps& rhs) {
    return lhs.steps == rhs.steps;
}

bool operator==(const Motion::plan_status& lhs, const Motion::plan_status& rhs) {
    return std::tie(lhs.reason, lhs.state, lhs.timestamp) ==
           std::tie(rhs.reason, rhs.state, rhs.timestamp);
}

bool operator==(const Motion::plan_status_with_id& lhs, const Motion::plan_status_with_id& rhs) {
    return std::tie(lhs.execution_id, lhs.component_name, lhs.status, lhs.plan_id) ==
           std::tie(rhs.execution_id, rhs.component_name, rhs.status, rhs.plan_id);
}

bool operator==(const Motion::plan& lhs, const Motion::plan& rhs) {
    return std::tie(lhs.component_name, lhs.execution_id, lhs.steps, lhs.id) ==
           std::tie(rhs.component_name, rhs.execution_id, rhs.steps, rhs.id);
}

bool operator==(const Motion::plan_with_status& lhs, const Motion::plan_with_status& rhs) {
    return std::tie(lhs.plan, lhs.status, lhs.status_history) ==
           std::tie(rhs.plan, rhs.status, rhs.status_history);
}

std::ostream& operator<<(std::ostream& os, const obstacle_detector& v) {
    os << "{ ";
    os << "\tvision_service: " << v.vision_service << '\n';
    os << "\tcamera: " << v.camera << '\n';
    os << "}";
    return os;
}

service::motion::v1::MotionConfiguration motion_configuration::to_proto() const {
    service::motion::v1::MotionConfiguration proto;

    for (const obstacle_detector& od : obstacle_detectors) {
        *proto.mutable_obstacle_detectors()->Add() = od.to_proto();
    }

    if (position_polling_frequency_hz && !isnan(*position_polling_frequency_hz)) {
        proto.set_position_polling_frequency_hz(*position_polling_frequency_hz);
    }

    if (obstacle_polling_frequency_hz && !isnan(*obstacle_polling_frequency_hz)) {
        proto.set_obstacle_polling_frequency_hz(*obstacle_polling_frequency_hz);
    }

    if (plan_deviation_m && !isnan(*plan_deviation_m)) {
        proto.set_plan_deviation_m(*plan_deviation_m);
    }

    if (linear_m_per_sec && !isnan(*linear_m_per_sec)) {
        proto.set_linear_m_per_sec(*linear_m_per_sec);
    }

    if (angular_degs_per_sec && !isnan(*angular_degs_per_sec)) {
        proto.set_angular_degs_per_sec(*angular_degs_per_sec);
    }

    return proto;
}

motion_configuration motion_configuration::from_proto(
    const service::motion::v1::MotionConfiguration& proto) {
    motion_configuration mc;

    for (const service::motion::v1::ObstacleDetector& od : proto.obstacle_detectors()) {
        mc.obstacle_detectors.push_back(obstacle_detector::from_proto(od));
    }

    if (proto.has_position_polling_frequency_hz()) {
        *mc.position_polling_frequency_hz = proto.position_polling_frequency_hz();
    }

    if (proto.has_obstacle_polling_frequency_hz()) {
        *mc.obstacle_polling_frequency_hz = proto.obstacle_polling_frequency_hz();
    }

    if (proto.has_plan_deviation_m()) {
        *mc.plan_deviation_m = proto.plan_deviation_m();
    }

    if (proto.has_linear_m_per_sec()) {
        *mc.linear_m_per_sec = proto.linear_m_per_sec();
    }

    if (proto.has_angular_degs_per_sec()) {
        *mc.angular_degs_per_sec = proto.angular_degs_per_sec();
    }

    return mc;
}

bool operator==(const motion_configuration& lhs, const motion_configuration& rhs) {
    return lhs.angular_degs_per_sec == rhs.angular_degs_per_sec &&
           lhs.obstacle_detectors == rhs.obstacle_detectors &&
           lhs.linear_m_per_sec == rhs.linear_m_per_sec &&
           lhs.plan_deviation_m == rhs.plan_deviation_m &&
           lhs.obstacle_polling_frequency_hz == rhs.obstacle_polling_frequency_hz &&
           lhs.position_polling_frequency_hz == rhs.position_polling_frequency_hz;
}

std::ostream& operator<<(std::ostream& os, const motion_configuration& v) {
    os << "{ ";
    if (!v.obstacle_detectors.empty()) {
        os << "\tobstacle_detectors: [\n";
        for (const obstacle_detector& od : v.obstacle_detectors) {
            os << "\t\t" << od << ",\n";
        }
        os << "\t],\n";
    }
    if (v.angular_degs_per_sec.has_value()) {
        os << "\tangular_degs_per_sec: " << v.angular_degs_per_sec.get() << ",\n";
    }
    if (v.linear_m_per_sec.has_value()) {
        os << "\tlinear_m_per_sec: " << v.linear_m_per_sec.get() << ",\n";
    }
    if (v.obstacle_polling_frequency_hz.has_value()) {
        os << "\tobstacle_polling_frequency_hz: " << v.obstacle_polling_frequency_hz.get() << ",\n";
    }
    if (v.plan_deviation_m.has_value()) {
        os << "\tplan_deviation_m: " << v.plan_deviation_m.get() << ",\n";
    }
    if (v.position_polling_frequency_hz.has_value()) {
        os << "\tposition_polling_frequency_hz: " << v.position_polling_frequency_hz.get() << ",\n";
    }
    os << "}";

    return os;
}

Motion::plan_state Motion::from_proto(const service::motion::v1::PlanState& proto) {
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

service::motion::v1::PlanState Motion::to_proto(const Motion::plan_state& state) {
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

Motion::plan_status Motion::plan_status::from_proto(const service::motion::v1::PlanStatus& proto) {
    plan_status mps;
    mps.state = Motion::from_proto(proto.state());
    if (proto.has_reason()) {
        mps.reason = proto.reason();
    }
    mps.timestamp = timestamp_to_time_pt(proto.timestamp());

    return mps;
}

std::vector<Motion::plan_status> Motion::plan_status::from_proto(
    const google::protobuf::RepeatedPtrField<service::motion::v1::PlanStatus>& proto) {
    std::vector<Motion::plan_status> pss;
    for (const auto& ps : proto) {
        pss.push_back(Motion::plan_status::from_proto(ps));
    }

    return pss;
}
service::motion::v1::PlanStatus Motion::plan_status::to_proto() const {
    service::motion::v1::PlanStatus proto;
    *proto.mutable_timestamp() = time_pt_to_timestamp(timestamp);
    if (reason) {
        *proto.mutable_reason() = *reason;
    }
    proto.set_state(Motion::to_proto(state));

    return proto;
}

Motion::steps Motion::steps::from_proto(
    const google::protobuf::RepeatedPtrField<service::motion::v1::PlanStep>& proto) {
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

service::motion::v1::PlanStep Motion::steps::to_proto(const Motion::steps::step& step) {
    service::motion::v1::PlanStep proto;
    for (const auto& kv : step) {
        service::motion::v1::ComponentState cs;
        *cs.mutable_pose() = kv.second.to_proto();
        proto.mutable_step()->insert({kv.first, cs});
    }

    return proto;
}

Motion::plan Motion::plan::from_proto(const service::motion::v1::Plan& proto) {
    Motion::plan plan;
    plan.id = proto.id();
    plan.execution_id = proto.execution_id();
    plan.component_name = Name::from_proto(proto.component_name());
    plan.steps = Motion::steps::from_proto(proto.steps());
    return plan;
}

service::motion::v1::Plan Motion::plan::to_proto() const {
    service::motion::v1::Plan proto;
    *proto.mutable_id() = id;
    *proto.mutable_component_name() = component_name.to_proto();
    *proto.mutable_execution_id() = execution_id;
    for (const auto& step : steps.steps) {
        *proto.mutable_steps()->Add() = Motion::steps::to_proto(step);
    }

    return proto;
}

Motion::plan_with_status Motion::plan_with_status::from_proto(
    const service::motion::v1::PlanWithStatus& proto) {
    Motion::plan_with_status pws;
    pws.plan = Motion::plan::from_proto(proto.plan());
    pws.status = Motion::plan_status::from_proto(proto.status());
    pws.status_history = Motion::plan_status::from_proto(proto.status_history());

    return pws;
}

std::vector<Motion::plan_with_status> Motion::plan_with_status::from_proto(
    const google::protobuf::RepeatedPtrField<service::motion::v1::PlanWithStatus>& proto) {
    std::vector<Motion::plan_with_status> plans;
    for (const auto& plan : proto) {
        plans.push_back(Motion::plan_with_status::from_proto(plan));
    }
    return plans;
}

service::motion::v1::PlanWithStatus Motion::plan_with_status::to_proto() const {
    service::motion::v1::PlanWithStatus proto;
    *proto.mutable_plan() = plan.to_proto();
    *proto.mutable_status() = status.to_proto();
    for (const auto& sh : status_history) {
        *proto.mutable_status_history()->Add() = sh.to_proto();
    }

    return proto;
}

Motion::plan_status_with_id Motion::plan_status_with_id::from_proto(
    const service::motion::v1::PlanStatusWithID& proto) {
    Motion::plan_status_with_id pswi;
    pswi.execution_id = proto.execution_id();
    pswi.component_name = Name::from_proto(proto.component_name());
    pswi.plan_id = proto.plan_id();
    pswi.status = Motion::plan_status::from_proto(proto.status());

    return pswi;
}

service::motion::v1::PlanStatusWithID Motion::plan_status_with_id::to_proto() const {
    service::motion::v1::PlanStatusWithID proto;

    *proto.mutable_execution_id() = execution_id;
    *proto.mutable_component_name() = component_name.to_proto();
    *proto.mutable_plan_id() = plan_id;
    *proto.mutable_status() = status.to_proto();

    return proto;
}

}  // namespace sdk
}  // namespace viam
