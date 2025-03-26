#define BOOST_TEST_MODULE test module test_pose_tracker

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/components/pose_tracker.hpp>
#include <viam/sdk/tests/mocks/mock_pose_tracker.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::PoseTracker::pose_map)

namespace viam {
namespace sdktests {

using namespace pose_tracker;
using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_pose_tracker)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockPoseTracker pose_tracker("mock_pose_tracker");
    auto api = pose_tracker.api();
    auto static_api = API::get<PoseTracker>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "pose_tracker");
}

BOOST_AUTO_TEST_CASE(mock_get_poses) {
    std::shared_ptr<MockPoseTracker> mock = MockPoseTracker::get_mock_pose_tracker();
    client_to_mock_pipeline<PoseTracker>(mock, [](PoseTracker& client) {
        const PoseTracker::pose_map& fakes = fake_poses();
        BOOST_CHECK_EQUAL(fakes, client.get_poses({}));
        BOOST_CHECK_EQUAL(fakes, client.get_poses({body1, body2}));

        const PoseTracker::pose_map& single = client.get_poses({body1});
        BOOST_CHECK_EQUAL(single.size(), 1);
        BOOST_CHECK_EQUAL(single.at(body1), fakes.at(body1));
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockPoseTracker> mock = MockPoseTracker::get_mock_pose_tracker();
    client_to_mock_pipeline<PoseTracker>(mock, [](PoseTracker& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockPoseTracker> mock = MockPoseTracker::get_mock_pose_tracker();
    client_to_mock_pipeline<PoseTracker>(mock, [](PoseTracker& client) {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_SUITE_END()
}  // namespace sdktests
}  // namespace viam
