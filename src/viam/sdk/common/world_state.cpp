#include <viam/sdk/common/world_state.hpp>

#include <viam/sdk/common/pose.hpp>

namespace viam {
namespace sdk {

bool operator==(const WorldState::geometries_in_frame& lhs,
                const WorldState::geometries_in_frame& rhs) {
    return lhs.reference_frame == rhs.reference_frame && lhs.geometries == rhs.geometries;
}
bool operator==(const WorldState::transform& lhs, const WorldState::transform& rhs) {
    return lhs.reference_frame == rhs.reference_frame &&
           (lhs.physical_object == rhs.physical_object ||
            *lhs.physical_object == *rhs.physical_object) &&
           lhs.pose_in_observer_frame == rhs.pose_in_observer_frame;
}

bool operator==(const WorldState& lhs, const WorldState& rhs) {
    return lhs.obstacles_ == rhs.obstacles_ && lhs.transforms_ == rhs.transforms_;
}

const std::vector<WorldState::geometries_in_frame>& WorldState::obstacles() const {
    return obstacles_;
}
const std::vector<WorldState::transform>& WorldState::transforms() const {
    return transforms_;
}

}  // namespace sdk
}  // namespace viam
