#pragma once

#include "viam/sdk/common/pose_in_frame.hpp"
#include "viam/sdk/resource/resource_api.hpp"
#include "viam/sdk/spatialmath/geometry.hpp"
#include <viam/sdk/services/motion/motion.hpp>

namespace viam {
namespace sdktests {
namespace motion {

sdk::PoseInFrame init_fake_pose();
sdk::PoseInFrame fake_pose();

class MockMotion : public sdk::Motion {
   public:
    bool move(const sdk::PoseInFrame& destination,
              const sdk::Name& component_name,
              std::shared_ptr<sdk::WorldState> ws,
              std::shared_ptr<constraints> constraints,
              const sdk::AttributeMap& extra) override;

    bool move_on_map(const sdk::pose& destination,
                     const sdk::Name& component_name,
                     const sdk::Name& slam_name,
                     const sdk::AttributeMap& extra) override;

    sdk::PoseInFrame get_pose(
        const sdk::Name& component_name,
        const std::string& destination_frame,
        const std::vector<sdk::WorldState::transform>& supplemental_transforms,
        sdk::AttributeMap extra) override;

    sdk::AttributeMap do_command(const sdk::AttributeMap& command) override;
    static std::shared_ptr<MockMotion> get_mock_motion();

    // These variables allow the testing infra to `peek` into the mock
    // and ensure that the correct values were passed
    // CR erodkin: should we make this private and not do the peek thing?
    sdk::PoseInFrame current_location;
    sdk::pose peek_current_pose;
    sdk::Name peek_component_name;
    sdk::Name peek_movement_sensor_name;
    sdk::Name peek_slam_name;
    std::string peek_destination_frame;
    std::vector<sdk::WorldState::transform> peek_supplemental_transforms;
    std::shared_ptr<constraints> peek_constraints;
    std::shared_ptr<sdk::WorldState> peek_world_state;
    sdk::AttributeMap peek_map;

    MockMotion(std::string name)
        : sdk::Motion(std::move(name)), current_location(init_fake_pose()){};
};

}  // namespace motion
}  // namespace sdktests
}  // namespace viam
