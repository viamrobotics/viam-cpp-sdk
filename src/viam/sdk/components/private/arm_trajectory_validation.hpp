/// @file components/private/arm_trajectory_validation.hpp
///
/// @brief Shared invariant checking for streamed arm trajectories.
#pragma once

#include <chrono>
#include <string>

#include <boost/optional.hpp>

#include <viam/sdk/components/arm.hpp>

namespace viam {
namespace sdk {
namespace impl {

// Validates the invariants of a streamed joint-space trajectory as its points
// arrive. One instance validates one logical stream: feed it each point in
// wire order via check(). The detection is identical on both sides of the RPC;
// the client and server differ only in how they react to a violation, so this
// reports the offending invariant as a message and leaves the throwing to the
// caller. On a valid point the running state advances; on a violation the state
// is left untouched.
//
// Invariants:
//   - the first point of the stream has time zero;
//   - times strictly increase across the entire stream;
//   - every point carries at least one position;
//   - when constraints are present, velocities carries one entry per position;
//   - when accelerations are present, they carry one entry per velocity.
//
// Dimensions are only checked for internal consistency; the trajectory is not
// validated against the arm's actual degrees of freedom.
class TrajectoryStreamValidator {
   public:
    // Returns a description of the first invariant the point violates, or
    // boost::none if the point is valid in sequence.
    boost::optional<std::string> check(const Arm::TrajectoryPoint& point) {
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
            if (constraints.velocities.size() != point.positions.size()) {
                return std::string(
                    "trajectory point must carry one velocity per position when constraints "
                    "are present");
            }
            if (constraints.accelerations &&
                constraints.accelerations->size() != constraints.velocities.size()) {
                return std::string(
                    "trajectory point must carry one acceleration per velocity when "
                    "accelerations are present");
            }
        }

        seen_first_ = true;
        last_time_ = point.time;
        return boost::none;
    }

   private:
    bool seen_first_ = false;
    std::chrono::microseconds last_time_{};
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
