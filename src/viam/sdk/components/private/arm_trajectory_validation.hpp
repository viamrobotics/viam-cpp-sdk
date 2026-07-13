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
// order via check(). On a valid point the running state advances; on a
// violation the state is left untouched.
//
// check() reports the offending invariant as a string instead of throwing, and
// the class pulls in no gRPC types. The only caller today is the server
// dispatcher, which turns that string into a grpc::Status (so it could just as
// well throw), but keeping it free of gRPC means that if we ever want the client
// to validate before sending, this drops straight in and raises whatever error
// the client wants from the same message.
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
