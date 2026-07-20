#include <algorithm>

#include <viam/sdk/components/private/arm_trajectory_validation.hpp>

namespace viam {
namespace sdk {
namespace impl {

boost::optional<std::string> TrajectoryStreamValidator::check(const Arm::trajectory_point& point) {
    if (!seen_first_) {
        if (point.time != std::chrono::microseconds::zero()) {
            return std::string("first trajectory point must have time zero");
        }
    } else if (point.time <= last_time_) {
        return std::string("trajectory point times must strictly increase");
    }

    if (point.positions.empty()) {
        return std::string("trajectory point must carry at least one position");
    }

    if (point.constraints) {
        const auto& constraints = *point.constraints;
        if (constraints.velocities_degs_per_sec.size() != point.positions.size()) {
            return std::string(
                "trajectory point must carry one velocity per position when constraints "
                "are present");
        }
        // We require a trajectory to start from rest, so a first point that
        // specifies velocities must specify them all as zero.
        if (!seen_first_ && std::any_of(constraints.velocities_degs_per_sec.begin(),
                                        constraints.velocities_degs_per_sec.end(),
                                        [](double velocity) { return velocity != 0.0; })) {
            return std::string("first trajectory point must start from rest (all velocities zero)");
        }
        if (constraints.accelerations_degs_per_sec2 &&
            constraints.accelerations_degs_per_sec2->size() !=
                constraints.velocities_degs_per_sec.size()) {
            return std::string(
                "trajectory point must carry one acceleration per velocity when "
                "accelerations are present");
        }
    }

    seen_first_ = true;
    last_time_ = point.time;
    return boost::none;
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
