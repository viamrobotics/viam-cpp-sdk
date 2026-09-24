#include <viam/sdk/services/motion.hpp>

#include <math.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

Motion::Motion(std::string name) : Service(std::move(name)) {}

API Motion::api() const {
    return API::get<Motion>();
}

API API::traits<Motion>::api() {
    return {kRDK, kService, "motion"};
}

bool operator==(const obstacle_detector& lhs, const obstacle_detector& rhs) {
    return lhs.vision_service == rhs.vision_service && lhs.camera == rhs.camera;
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

bool operator==(const Motion::orientation_constraint& lhs, const Motion::orientation_constraint& rhs) {
    return lhs.orientation_tolerance_degs == rhs.orientation_tolerance_degs &&
           lhs.ignore_theta == rhs.ignore_theta;
}

std::ostream& operator<<(std::ostream& os, const obstacle_detector& v) {
    os << "{ ";
    os << "\tvision_service: " << v.vision_service << '\n';
    os << "\tcamera: " << v.camera << '\n';
    os << "}";
    return os;
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

namespace proto_convert_details {

void to_proto_impl<service::motion::v1::PlanWithStatus>::operator()(
    const Motion::plan_with_status& self, service::motion::v1::PlanWithStatus* proto) const {
    *proto->mutable_plan() = to_proto(self.plan);
    *proto->mutable_status() = to_proto(self.status);
    for (const auto& status_history_item : self.status_history) {
        *proto->add_status_history() = to_proto(status_history_item);
    }
}

Motion::plan_with_status from_proto_impl<service::motion::v1::PlanWithStatus>::operator()(
    const service::motion::v1::PlanWithStatus* proto) const {
    Motion::plan_with_status result;
    result.plan = from_proto(proto->plan());
    result.status = from_proto(proto->status());
    for (const auto& status_history_item : proto->status_history()) {
        result.status_history.push_back(from_proto(status_history_item));
    }
    return result;
}

void to_proto_impl<Motion::TempStreamOptions>::operator()(
    const Motion::TempStreamOptions& self, viam::service::motion::v1::TempStreamOptions* proto) const {
    if (self.arm_side_target_runway_ms) {
        proto->set_arm_side_target_runway_ms(*self.arm_side_target_runway_ms);
    }
    if (self.send_to_arm_interval_ms) {
        proto->set_send_to_arm_interval_ms(*self.send_to_arm_interval_ms);
    }
    if (self.diagnostics_window_secs) {
        proto->set_diagnostics_window_secs(*self.diagnostics_window_secs);
    }
    if (self.move_options) {
        *proto->mutable_move_options() = to_proto(*self.move_options);
    }
}

Motion::TempStreamOptions from_proto_impl<viam::service::motion::v1::TempStreamOptions>::operator()(
    const viam::service::motion::v1::TempStreamOptions* proto) const {
    Motion::TempStreamOptions result;
    if (proto->has_arm_side_target_runway_ms()) {
        result.arm_side_target_runway_ms = proto->arm_side_target_runway_ms();
    }
    if (proto->has_send_to_arm_interval_ms()) {
        result.send_to_arm_interval_ms = proto->send_to_arm_interval_ms();
    }
    if (proto->has_diagnostics_window_secs()) {
        result.diagnostics_window_secs = proto->diagnostics_window_secs();
    }
    if (proto->has_move_options()) {
        result.move_options = from_proto(proto->move_options());
    }
    return result;
}

void to_proto_impl<Motion::TempStreamArmJointPositionsRequest_Init>::operator()(
    const Motion::TempStreamArmJointPositionsRequest_Init& self,
    viam::service::motion::v1::TempStreamArmJointPositionsRequest_Init* proto) const {
    proto->set_component_name(self.component_name);
    if (self.options) {
        *proto->mutable_options() = to_proto(*self.options);
    }
    *proto->mutable_extra() = to_proto(self.extra);
}

Motion::TempStreamArmJointPositionsRequest_Init
from_proto_impl<viam::service::motion::v1::TempStreamArmJointPositionsRequest_Init>::operator()(
    const viam::service::motion::v1::TempStreamArmJointPositionsRequest_Init* proto) const {
    Motion::TempStreamArmJointPositionsRequest_Init result;
    result.component_name = proto->component_name();
    if (proto->has_options()) {
        result.options = from_proto(proto->options());
    }
    result.extra = from_proto(proto->extra());
    return result;
}

void to_proto_impl<Motion::TempStreamArmJointPositionsRequest_Targets>::operator()(
    const Motion::TempStreamArmJointPositionsRequest_Targets& self,
    viam::service::motion::v1::TempStreamArmJointPositionsRequest_Targets* proto) const {
    for (const auto& pos : self.positions) {
        *proto->add_positions() = pos;
    }
}

Motion::TempStreamArmJointPositionsRequest_Targets
from_proto_impl<viam::service::motion::v1::TempStreamArmJointPositionsRequest_Targets>::operator()(
    const viam::service::motion::v1::TempStreamArmJointPositionsRequest_Targets* proto) const {
    Motion::TempStreamArmJointPositionsRequest_Targets result;
    for (const auto& pos : proto->positions()) {
        result.positions.push_back(pos);
    }
    return result;
}

void to_proto_impl<Motion::TempStreamArmJointPositionsRequest>::operator()(
    const Motion::TempStreamArmJointPositionsRequest& self,
    viam::service::motion::v1::TempStreamArmJointPositionsRequest* proto) const {
    proto->set_name(self.name);
    if (self.message.type() == typeid(Motion::TempStreamArmJointPositionsRequest_Init)) {
        *proto->mutable_init() = to_proto(boost::get<Motion::TempStreamArmJointPositionsRequest_Init>(self.message));
    } else if (self.message.type() == typeid(Motion::TempStreamArmJointPositionsRequest_Targets)) {
        *proto->mutable_targets() = to_proto(boost::get<Motion::TempStreamArmJointPositionsRequest_Targets>(self.message));
    }
}

Motion::TempStreamArmJointPositionsRequest
from_proto_impl<viam::service::motion::v1::TempStreamArmJointPositionsRequest>::operator()(
    const viam::service::motion::v1::TempStreamArmJointPositionsRequest* proto) const {
    Motion::TempStreamArmJointPositionsRequest result;
    result.name = proto->name();
    if (proto->has_init()) {
        result.message = from_proto(proto->init());
    } else if (proto->has_targets()) {
        result.message = from_proto(proto->targets());
    }
    return result;
}

void to_proto_impl<Motion::TempStreamArmJointPositionsResponse>::operator()(
    const Motion::TempStreamArmJointPositionsResponse&,
    viam::service::motion::v1::TempStreamArmJointPositionsResponse*) const {
    // No fields to set for an empty response.
}

Motion::TempStreamArmJointPositionsResponse
from_proto_impl<viam::service::motion::v1::TempStreamArmJointPositionsResponse>::operator()(
    const viam::service::motion::v1::TempStreamArmJointPositionsResponse*) const {
    return {};
}

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam