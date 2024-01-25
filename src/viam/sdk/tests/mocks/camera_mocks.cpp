#include <viam/sdk/tests/mocks/camera_mocks.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>
#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/components/camera/server.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace camera {

using namespace viam::sdk;

AttributeMap MockCamera::do_command(AttributeMap command) {
    return map_;
}
Camera::raw_image MockCamera::get_image(std::string mime_type, const AttributeMap& extra) {
    return image_;
}
Camera::image_collection MockCamera::get_images() {
    return images_;
}
Camera::point_cloud MockCamera::get_point_cloud(std::string mime_type, const AttributeMap& extra) {
    return pc_;
}
std::vector<GeometryConfig> MockCamera::get_geometries(const AttributeMap& extra) {
    return geometries_;
}
Camera::properties MockCamera::get_properties() {
    return camera_properties_;
}

Camera::raw_image fake_raw_image() {
    Camera::raw_image image;
    image.mime_type = "JPEG";
    image.source_name = "";
    std::vector<unsigned char> bytes = {'a', 'b', 'c'};
    image.bytes = bytes;
    return image;
}

Camera::image_collection fake_raw_images() {
    Camera::image_collection collection;
    std::vector<Camera::raw_image> images;
    Camera::raw_image image1;
    image1.mime_type = "image/jpeg";
    image1.source_name = "color_sensor";
    std::vector<unsigned char> bytes1 = {'a', 'b', 'c'};
    image1.bytes = bytes1;
    images.push_back(image1);
    Camera::raw_image image2;
    image2.mime_type = "image/vnd.viam.dep";
    image2.source_name = "depth_sensor";
    std::vector<unsigned char> bytes2 = {'d', 'e', 'f'};
    image2.bytes = bytes2;
    images.push_back(image2);
    std::chrono::seconds seconds(12345);
    std::chrono::nanoseconds nanos(0);
    collection.images = images;
    collection.metadata.captured_at = std::chrono::time_point<long long, std::chrono::nanoseconds>(
        std::chrono::duration_cast<std::chrono::system_clock::duration>(seconds) + nanos);
    return collection;
}

Camera::point_cloud fake_point_cloud() {
    Camera::point_cloud point_cloud;
    point_cloud.mime_type = "pointcloud/pcd";
    std::vector<unsigned char> bytes = {'a', 'b', 'c'};
    point_cloud.pc = bytes;
    return point_cloud;
}

Camera::intrinsic_parameters fake_intrinsic_parameters() {
    Camera::intrinsic_parameters intrinsic_parameters;
    intrinsic_parameters.width_px = 1;
    intrinsic_parameters.height_px = 2;
    intrinsic_parameters.focal_x_px = 3;
    intrinsic_parameters.focal_y_px = 4;
    intrinsic_parameters.center_x_px = 5;
    intrinsic_parameters.center_y_px = 6;
    return intrinsic_parameters;
}

Camera::distortion_parameters fake_distortion_parameters() {
    Camera::distortion_parameters distortion_parameters;
    distortion_parameters.model = "no distortion";
    return distortion_parameters;
}

Camera::properties fake_properties() {
    Camera::properties properties;
    properties.supports_pcd = true;
    properties.intrinsic_parameters = fake_intrinsic_parameters();
    properties.distortion_parameters = fake_distortion_parameters();
    return properties;
}

std::shared_ptr<MockCamera> MockCamera::get_mock_camera() {
    auto camera = std::make_shared<MockCamera>("mock_camera");

    camera->image_ = fake_raw_image();
    camera->images_ = fake_raw_images();
    camera->pc_ = fake_point_cloud();
    camera->camera_properties_ = fake_properties();
    camera->intrinsic_parameters_ = fake_intrinsic_parameters();
    camera->distortion_parameters_ = fake_distortion_parameters();
    camera->map_ = fake_map();
    camera->geometries_ = fake_geometries();
    return camera;
}

}  // namespace camera
}  // namespace sdktests
}  // namespace viam
