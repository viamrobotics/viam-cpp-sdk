#define BOOST_TEST_MODULE test module test_camera
#include <common/v1/common.pb.h>
#include <component/camera/v1/camera.grpc.pb.h>
#include <component/camera/v1/camera.pb.h>

#include <boost/test/included/unit_test.hpp>
#include <common/proto_type.hpp>
#include <components/camera/camera.hpp>
#include <components/camera/client.hpp>
#include <components/camera/server.hpp>
#include <tests/mocks/camera_mocks.hpp>
#include <tests/test_utils.hpp>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

BOOST_AUTO_TEST_SUITE(camera_suite)

std::shared_ptr<MockCamera> camera = MockCamera::get_mock_camera();

BOOST_AUTO_TEST_CASE(test_get_image) {
    Camera::raw_image expected_image = fake_raw_image();
    Camera::raw_image image = camera->get_image("JPEG");

    BOOST_CHECK(expected_image == image);
}

BOOST_AUTO_TEST_CASE(test_get_point_cloud) {
    Camera::point_cloud expected_pc = fake_point_cloud();
    Camera::point_cloud pc = camera->get_point_cloud("pointcloud/pcd");

    BOOST_CHECK(expected_pc == pc);
}

BOOST_AUTO_TEST_CASE(test_get_properties) {
    Camera::properties expected_props = fake_properties();
    Camera::properties properties = camera->get_properties();

    BOOST_CHECK(expected_props == properties);
}

BOOST_AUTO_TEST_CASE(test_do) {
    AttributeMap expected = fake_map();

    AttributeMap command;
    AttributeMap result_map = camera->do_command(command);

    ProtoType expected_pt = *(expected->at(std::string("test")));
    ProtoType result_pt = *(result_map->at(std::string("test")));

    BOOST_CHECK(result_pt == expected_pt);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_camera_service)

MockCameraStub mock = MockCameraStub();

BOOST_AUTO_TEST_CASE(test_get_image_service) {
    grpc::ClientContext ctx;
    viam::component::camera::v1::GetImageRequest req;
    viam::component::camera::v1::GetImageResponse resp;

    *req.mutable_name() = "camera";
    *req.mutable_mime_type() = "JPEG";

    grpc::Status status = mock.GetImage(&ctx, req, &resp);
    BOOST_CHECK(status.error_code() == 0);

    Camera::raw_image image = fake_raw_image();

    std::vector<unsigned char> bytes = string_to_bytes(resp.image());

    BOOST_CHECK(image.bytes == bytes);
}

BOOST_AUTO_TEST_CASE(test_get_point_cloud_service) {
    grpc::ClientContext ctx;
    viam::component::camera::v1::GetPointCloudRequest req;
    viam::component::camera::v1::GetPointCloudResponse resp;

    *req.mutable_name() = "camera";
    *req.mutable_mime_type() = "pointcloud/pcd";

    grpc::Status status = mock.GetPointCloud(&ctx, req, &resp);
    BOOST_CHECK(status.error_code() == 0);

    Camera::point_cloud expected_pc = fake_point_cloud();

    std::vector<unsigned char> bytes = string_to_bytes(resp.point_cloud());

    BOOST_CHECK(expected_pc.pc == bytes);
}

BOOST_AUTO_TEST_CASE(test_render_frame_service) {
    grpc::ClientContext ctx;
    viam::component::camera::v1::RenderFrameRequest req;
    ::google::api::HttpBody resp;

    Camera::raw_image image = fake_raw_image();

    *req.mutable_name() = "camera";
    *req.mutable_mime_type() = "JPEG";
    grpc::Status status = mock.RenderFrame(&ctx, req, &resp);

    BOOST_CHECK(status.error_code() == 0);

    std::vector<unsigned char> bytes = string_to_bytes(resp.data());

    BOOST_CHECK(resp.content_type() == image.mime_type);
    BOOST_CHECK(image.bytes == bytes);
}

BOOST_AUTO_TEST_CASE(test_get_properties_service) {
    grpc::ClientContext ctx;
    viam::component::camera::v1::GetPropertiesRequest req;
    viam::component::camera::v1::GetPropertiesResponse resp;
    *req.mutable_name() = "camera";
    grpc::Status status = mock.GetProperties(&ctx, req, &resp);

    BOOST_CHECK(status.error_code() == 0);

    Camera::properties expected = fake_properties();

    BOOST_CHECK(expected.supports_pcd == resp.supports_pcd());
    BOOST_CHECK(expected.intrinsic_parameters == Camera::from_proto(resp.intrinsic_parameters()));
    BOOST_CHECK(expected.distortion_parameters == Camera::from_proto(resp.distortion_parameters()));
}

BOOST_AUTO_TEST_CASE(test_do_service) {
    grpc::ClientContext ctx;
    viam::common::v1::DoCommandRequest req;
    viam::common::v1::DoCommandResponse resp;

    AttributeMap command = fake_map();
    *req.mutable_command() = map_to_struct(command);

    *req.mutable_name() = "camera";

    AttributeMap expected_map = fake_map();

    grpc::Status status = mock.DoCommand(&ctx, req, &resp);
    AttributeMap result_map = struct_to_map(resp.result());

    std::shared_ptr<ProtoType> expected_pt = expected_map->at(std::string("test"));

    std::shared_ptr<ProtoType> result_pt = result_map->at(std::string("test"));

    BOOST_CHECK(*expected_pt == *result_pt);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_camera_client)

MockCameraClient client = MockCameraClient("camera");

BOOST_AUTO_TEST_CASE(test_image_client) {
    Camera::raw_image image = client.get_image("JPEG");
    Camera::raw_image expected_image = fake_raw_image();

    BOOST_CHECK(expected_image == image);
}

BOOST_AUTO_TEST_CASE(test_get_point_cloud_client) {
    Camera::point_cloud pc = client.get_point_cloud("pointcloud/pcd");
    Camera::point_cloud expected = fake_point_cloud();

    BOOST_CHECK(expected == pc);
}

BOOST_AUTO_TEST_CASE(test_get_properties_client) {
    Camera::properties props = client.get_properties();
    Camera::properties expected = fake_properties();

    BOOST_CHECK(expected == props);
}

BOOST_AUTO_TEST_CASE(test_do_client) {
    AttributeMap command = fake_map();

    AttributeMap expected_map = fake_map();

    AttributeMap result_map = client.do_command(command);

    ProtoType expected_pt = *(expected_map->at(std::string("test")));
    ProtoType result_pt = *(result_map->at(std::string("test")));

    BOOST_CHECK(expected_pt == result_pt);
}
BOOST_AUTO_TEST_SUITE_END()
