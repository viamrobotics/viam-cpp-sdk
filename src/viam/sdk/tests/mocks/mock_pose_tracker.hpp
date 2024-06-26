#pragma once

#include <viam/sdk/components/pose_tracker.hpp>

namespace viam {
namespace sdktests {
namespace pose_tracker {

// body names for the fake poses
constexpr const char* body1 = "b1";
constexpr const char* body2 = "b2";

std::unordered_map<std::string, sdk::pose_in_frame> fake_poses();

class MockPoseTracker : public sdk::PoseTracker {
   public:
    static std::shared_ptr<MockPoseTracker> get_mock_pose_tracker();

    MockPoseTracker(std::string name) : sdk::PoseTracker(std::move(name)) {}

    std::unordered_map<std::string, sdk::pose_in_frame> get_poses(
        const std::vector<std::string>& body_names, const sdk::AttributeMap& extra) override;

    sdk::AttributeMap do_command(const sdk::AttributeMap& command) override;

    std::vector<sdk::GeometryConfig> get_geometries(const sdk::AttributeMap& extra) override;

    sdk::AttributeMap peek_do_command_command;
};

}  // namespace pose_tracker
}  // namespace sdktests
}  // namespace viam
