#pragma once

#include "viam/sdk/resource/resource_api.hpp"
#include "viam/sdk/spatialmath/geometry.hpp"
#include <viam/sdk/services/motion/motion.hpp>

namespace viam {
namespace sdktests {
namespace motion {

class MockMotion : public sdk::Motion {
   public:
    bool move(const sdk::PoseInFrame& destination,
              const sdk::Name& component_name,
              std::unique_ptr<sdk::WorldState> ws,
              std::unique_ptr<constraints> constraints,
              const sdk::AttributeMap& extra) override;

    bool move_on_map(const sdk::pose& destination,
                     const sdk::Name& component_name,
                     const sdk::Name& slam_name,
                     const sdk::AttributeMap& extra) override;

    sdk::PoseInFrame get_pose(const sdk::Name& component_name,
                              const std::string& destination_frame,
                              std::vector<sdk::WorldState::transform> supplemental_transforms,
                              sdk::AttributeMap extra) override;

    sdk::AttributeMap do_command(const sdk::AttributeMap& command) override;
    static std::shared_ptr<MockMotion> get_mock_motion();

    // These variables allow the testing infra to `peek` into the mock
    // and ensure that the correct values were passed
    sdk::PoseInFrame current_location;
    sdk::pose peek_current_pose;
    sdk::Name peek_component_name;
    sdk::Name peek_movement_sensor_name;
    sdk::Name peek_slam_name;
    constraints peek_constraints;
    sdk::WorldState peek_world_state;
    sdk::AttributeMap peek_extra;

    MockMotion(std::string name) : Motion(std::move(name)){};
};

}  // namespace motion
}  // namespace sdktests
}  // namespace viam
