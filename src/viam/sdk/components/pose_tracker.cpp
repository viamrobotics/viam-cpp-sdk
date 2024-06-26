#include <viam/sdk/components/pose_tracker.hpp>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {
API PoseTracker::api() const {
    return API::get<PoseTracker>();
}

API API::traits<PoseTracker>::api() {
    return {kRDK, kComponent, "pose_tracker"};
}

}  // namespace sdk
}  // namespace viam
