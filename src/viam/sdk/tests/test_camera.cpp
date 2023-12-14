#define BOOST_TEST_MODULE test module test_camera

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>
#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/components/camera/client.hpp>
#include <viam/sdk/components/camera/server.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/tests/mocks/camera_mocks.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace camera;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_camera)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    std::shared_ptr<MockCamera> camera = MockCamera::get_mock_camera();
    auto api = camera->api();
    auto static_api = API::get<Camera>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "camera");
}

BOOST_AUTO_TEST_CASE(test_get_image) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<CameraClient, CameraServer>(mock, [&](Camera& client) {
        Camera::raw_image image = client.get_image("JPEG");
        Camera::raw_image expected_image = fake_raw_image();

        BOOST_CHECK(expected_image == image);
    });
}

BOOST_AUTO_TEST_CASE(test_get_images) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<CameraClient, CameraServer>(mock, [&](Camera& client) {
        Camera::image_collection expected_images = fake_raw_images();
        Camera::image_collection images = client.get_images();

        BOOST_CHECK(expected_images == images);
    });
}

BOOST_AUTO_TEST_CASE(test_get_point_cloud) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<CameraClient, CameraServer>(mock, [&](Camera& client) {
        Camera::point_cloud pc = client.get_point_cloud("pointcloud/pcd");
        Camera::point_cloud expected = fake_point_cloud();

        BOOST_CHECK(expected == pc);
    });
}

BOOST_AUTO_TEST_CASE(test_get_properties) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<CameraClient, CameraServer>(mock, [&](Camera& client) {
        Camera::properties props = client.get_properties();
        Camera::properties expected = fake_properties();

        BOOST_CHECK(expected == props);
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<CameraClient, CameraServer>(mock, [&](Camera& client) {
        std::vector<GeometryConfig> result_geometries = client.get_geometries();
        std::vector<GeometryConfig> expected_geometries = fake_geometries();

        BOOST_CHECK(result_geometries == expected_geometries);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<CameraClient, CameraServer>(mock, [&](Camera& client) {
        AttributeMap command = fake_map();
        AttributeMap expected_map = fake_map();
        AttributeMap result_map = client.do_command(command);

        ProtoType expected_pt = *(expected_map->at(std::string("test")));
        ProtoType result_pt = *(result_map->at(std::string("test")));

        BOOST_CHECK(expected_pt == result_pt);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
