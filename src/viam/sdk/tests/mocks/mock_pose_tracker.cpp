#include <viam/sdk/tests/mocks/mock_pose_tracker.hpp>

#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace pose_tracker {

using namespace viam::sdk;

PoseTracker::pose_map fake_poses() {
    return {
        {body1, pose_in_frame("", {{0, 0, 0}, {0, 0, 0}, 0})},
        {body2, pose_in_frame("", {{1, 2, 3}, {4, 5, 6}, 7})},
    };
}

std::shared_ptr<MockPoseTracker> MockPoseTracker::get_mock_pose_tracker() {
    return std::make_shared<MockPoseTracker>("mock_pose_tracker");
}

PoseTracker::pose_map MockPoseTracker::get_poses(const std::vector<std::string>& body_names,
                                                 const sdk::AttributeMap&) {
    auto full_map = fake_poses();

    if (body_names.empty())
        return full_map;

    PoseTracker::pose_map result;
    for (const auto& pair : full_map)
        if (std::find(body_names.begin(), body_names.end(), pair.first) != body_names.end())
            result.insert(pair);

    return result;
}

AttributeMap MockPoseTracker::do_command(const AttributeMap& command) {
    this->peek_do_command_command = command;
    return command;
}

std::vector<GeometryConfig> MockPoseTracker::get_geometries(const sdk::AttributeMap&) {
    return fake_geometries();
}

}  // namespace pose_tracker
}  // namespace sdktests
}  // namespace viam
