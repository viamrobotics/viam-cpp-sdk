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
    this->peek_map = std::move(extra);
    this->current_location.pose = destination;

    return true;
};

PoseInFrame MockMotion::get_pose(const Name& component_name,
                                 const std::string& destination_frame,
                                 std::vector<WorldState::transform> supplemental_transforms,
                                 AttributeMap extra) {
    return current_location;
};

AttributeMap MockMotion::do_command(const AttributeMap& _command) {
    return peek_map;
};

static std::shared_ptr<MockMotion> get_mock_motion() {
    auto motion = std::make_shared<MockMotion>("mock_motion");
    motion->current_location = fake_pose();

    return motion;
};

PoseInFrame fake_pose() {
    return PoseInFrame("fake-reference-frame", {{0, 1, 2}, {3, 4, 5}, 6});
}

PoseInFrame init_fake_pose() {
    return PoseInFrame("", {{0, 0, 0}, {0, 0, 0}, 0});
}

}  // namespace motion
}  // namespace sdktests
}  // namespace viam
