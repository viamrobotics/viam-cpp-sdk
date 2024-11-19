#include <viam/sdk/services/motion.hpp>

#include <math.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

Motion::Motion(std::string name) : Service(std::move(name)){};

API Motion::api() const {
    return API::get<Motion>();
}

API API::traits<Motion>::api() {
    return {kRDK, kService, "motion"};
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

}  // namespace sdk
}  // namespace viam
