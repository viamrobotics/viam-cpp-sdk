#include <viam/sdk/services/motion.hpp>

#include <math.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

Motion::Motion(std::string name) : Service(std::move(name)) {}

Motion::Motion(const ResourceConfig& cfg) : Service(cfg) {}

API Motion::api() const {
    return API::get<Motion>();
}

API API::traits<Motion>::api() {
    return {kRDK, kService, "motion"};
}

std::ostream& operator<<(std::ostream& os, const obstacle_detector& v) {
    os << "{ ";
    os << "\tvision_service: " << v.vision_service << '\n';
    os << "\tcamera: " << v.camera << '\n';
    os << "}";
    return os;
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
        os << "\tangular_degs_per_sec: " << v.angular_degs_per_sec.value() << ",\n";
    }
    if (v.linear_m_per_sec.has_value()) {
        os << "\tlinear_m_per_sec: " << v.linear_m_per_sec.value() << ",\n";
    }
    if (v.obstacle_polling_frequency_hz.has_value()) {
        os << "\tobstacle_polling_frequency_hz: " << v.obstacle_polling_frequency_hz.value()
           << ",\n";
    }
    if (v.plan_deviation_m.has_value()) {
        os << "\tplan_deviation_m: " << v.plan_deviation_m.value() << ",\n";
    }
    if (v.position_polling_frequency_hz.has_value()) {
        os << "\tposition_polling_frequency_hz: " << v.position_polling_frequency_hz.value()
           << ",\n";
    }
    os << "}";

    return os;
}

}  // namespace sdk
}  // namespace viam
