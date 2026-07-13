// components/private/arm_trajectory_validation.hpp
//
// Invariant checking for streamed arm trajectories.
#pragma once

#include <chrono>
#include <string>

#include <boost/optional.hpp>

#include <viam/sdk/components/arm.hpp>

namespace viam {
namespace sdk {
namespace impl {

// Validates the invariants of a streamed joint-space trajectory as its points
// arrive. One instance validates one logical stream: feed it each point in wire
// order via check(). It is deliberately agnostic of the RPC layer -- it reports
// the offending invariant as a message rather than throwing, leaving its caller
// (the server dispatcher) to map that message onto the appropriate
// grpc::Status. Keeping it free of gRPC types leaves it a self-contained
// invariant checker rather than entangling it with transport concerns. On a
// valid point the running state advances; on a violation the state is left
// untouched.
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
    boost::optional<std::string> check(const Arm::trajectory_point& point);

   private:
    bool seen_first_ = false;
    std::chrono::microseconds last_time_{};
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
