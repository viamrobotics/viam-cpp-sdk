#define BOOST_TEST_MODULE test module test_camera

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/camera.hpp>
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
    client_to_mock_pipeline<Camera>(mock, [](Camera& client) {
        Camera::raw_image image = client.get_image("JPEG");
        Camera::raw_image expected_image = fake_raw_image();

        BOOST_CHECK(expected_image == image);
    });
}

BOOST_AUTO_TEST_CASE(test_get_images) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<Camera>(mock, [](Camera& client) {
        Camera::image_collection expected_images = fake_raw_images();
        Camera::image_collection images = client.get_images();

        BOOST_CHECK(expected_images == images);
    });
}

BOOST_AUTO_TEST_CASE(test_get_images_filtering) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<Camera>(mock, [&](Camera& client) {
        // request only color
        Camera::image_collection images = client.get_images({"color"});
        BOOST_CHECK_EQUAL(images.images.size(), 1);
        BOOST_CHECK_EQUAL(images.images[0].source_name, "color");

        // empty filter returns all
        Camera::image_collection all_images = client.get_images({});
        BOOST_CHECK_EQUAL(all_images.images.size(), 2);

        // verify filter propagated to mock
        auto last_filter = mock->last_filter_source_names();
        BOOST_CHECK_EQUAL(last_filter.size(), 0);
    });
}

BOOST_AUTO_TEST_CASE(test_get_images_with_extra) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<Camera>(mock, [&](Camera& client) {
        ProtoStruct extra;
        extra["foo"] = ProtoValue("bar");
        auto images = client.get_images({}, extra);
        (void)images; // unused variable in test body
        const auto& last_extra = mock->last_extra();
        BOOST_CHECK(last_extra.at("foo").is_a<std::string>());
        BOOST_CHECK_EQUAL(last_extra.at("foo").get_unchecked<std::string>(), "bar");
    });
}

BOOST_AUTO_TEST_CASE(test_get_point_cloud) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<Camera>(mock, [](Camera& client) {
        Camera::point_cloud pc = client.get_point_cloud("pointcloud/pcd");
        Camera::point_cloud expected = fake_point_cloud();

        BOOST_CHECK(expected == pc);
    });
}

BOOST_AUTO_TEST_CASE(test_get_properties) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<Camera>(mock, [](Camera& client) {
        Camera::properties props = client.get_properties();
        Camera::properties expected = fake_properties();

        BOOST_CHECK(expected == props);
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<Camera>(mock, [](Camera& client) {
        std::vector<GeometryConfig> result_geometries = client.get_geometries();
        std::vector<GeometryConfig> expected_geometries = fake_geometries();

        BOOST_CHECK(result_geometries == expected_geometries);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockCamera> mock = MockCamera::get_mock_camera();
    client_to_mock_pipeline<Camera>(mock, [](Camera& client) {
        ProtoStruct command = fake_map();
        ProtoStruct expected_map = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(expected_map.at("test") == result_map.at("test"));
    });
}

BOOST_AUTO_TEST_CASE(test_depth_map_encode_decode) {
    xt::xarray<uint16_t> depth_map =
        xt::xarray<uint16_t>::from_shape({3, 2});  // height = 3, width = 2
    depth_map(0, 0) = 100;
    depth_map(0, 1) = 200;
    depth_map(1, 0) = 300;
    depth_map(1, 1) = 400;
    depth_map(2, 0) = 500;
    depth_map(2, 1) = 600;

    std::vector<unsigned char> data = Camera::encode_depth_map(depth_map);
    auto result_map = Camera::decode_depth_map(data);

    // Check if the dimensions and values match
    BOOST_CHECK_EQUAL(result_map.shape()[0], 3);  // height
    BOOST_CHECK_EQUAL(result_map.shape()[1], 2);  // width

    std::vector<uint16_t> expected_values = {100, 200, 300, 400, 500, 600};
    std::vector<uint16_t> result_values(result_map.begin(), result_map.end());

    BOOST_CHECK_EQUAL_COLLECTIONS(
        result_values.begin(), result_values.end(), expected_values.begin(), expected_values.end());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
