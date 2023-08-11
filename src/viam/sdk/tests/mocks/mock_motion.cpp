#include <viam/sdk/tests/mocks/mock_motion.hpp>

namespace viam {
namespace sdktests {
namespace motion {

using namespace viam::sdk;

bool MockMotion::move(const PoseInFrame& destination,
                      const Name& component_name,
                      std::unique_ptr<WorldState> ws,
                      std::unique_ptr<constraints> constraints,
                      const AttributeMap& extra) {
    this->current_location = destination;
    this->peek_component_name = component_name;
    this->peek_world_state = *ws;
    this->peek_constraints = *constraints;
    return true;
};

bool MockMotion::move_on_map(const pose& destination,
                             const Name& component_name,
                             const Name& slam_name,
                             const AttributeMap& extra) {
    this->peek_current_pose = std::move(destination);
    this->peek_component_name = std::move(component_name);
    this->peek_slam_name = std::move(slam_name);
    this->peek_extra = std::move(extra);

    return true;
};

PoseInFrame MockMotion::get_pose(const Name& component_name,
                                 const std::string& destination_frame,
                                 std::vector<WorldState::transform> supplemental_transforms,
                                 AttributeMap extra){};

AttributeMap do_command(const AttributeMap& command){};
static std::shared_ptr<MockMotion> get_mock_motion();

}  // namespace motion
}  // namespace sdktests
}  // namespace viam
