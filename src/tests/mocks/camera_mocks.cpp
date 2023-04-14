#include <tests/mocks/camera_mocks.hpp>

#include <common/v1/common.pb.h>
#include <component/camera/v1/camera.grpc.pb.h>
#include <component/camera/v1/camera.pb.h>

#include <components/camera/camera.hpp>
#include <components/camera/server.hpp>
#include <tests/test_utils.hpp>

namespace viam {
namespace cppsdktests {

using namespace viam::cppsdk;

AttributeMap MockCamera::do_command(AttributeMap command) {
    return map;
}
Camera::raw_image MockCamera::get_image(std::string mime_type) {
    return image;
}
Camera::point_cloud MockCamera::get_point_cloud(std::string mime_type) {
    return pc;
}
Camera::properties MockCamera::get_properties() {
    return camera_properties;
}

Camera::raw_image fake_raw_image() {
    Camera::raw_image image;
    image.mime_type = "JPEG";
    std::vector<unsigned char> bytes = {'a', 'b', 'c'};
    image.bytes = bytes;
    return image;
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
    auto camera = std::make_shared<MockCamera>("camera");

    camera->image = fake_raw_image();
    camera->pc = fake_point_cloud();
    camera->camera_properties = fake_properties();
    camera->map = fake_map();

    return camera;
}

MockCameraStub::MockCameraStub() : server(std::make_shared<CameraServer>()) {
    this->server->get_sub_svc()->add(std::string("camera"), MockCamera::get_mock_camera());
};

::grpc::Status MockCameraStub::GetImage(
    ::grpc::ClientContext* context,
    const ::viam::component::camera::v1::GetImageRequest& request,
    ::viam::component::camera::v1::GetImageResponse* response) {
    grpc::ServerContext* ctx;
    return server->GetImage(ctx, &request, response);
}
::grpc::Status MockCameraStub::RenderFrame(
    ::grpc::ClientContext* context,
    const ::viam::component::camera::v1::RenderFrameRequest& request,
    ::google::api::HttpBody* response) {
    grpc::ServerContext* ctx;
    return server->RenderFrame(ctx, &request, response);
}
::grpc::Status MockCameraStub::GetPointCloud(
    ::grpc::ClientContext* context,
    const ::viam::component::camera::v1::GetPointCloudRequest& request,
    ::viam::component::camera::v1::GetPointCloudResponse* response) {
    grpc::ServerContext* ctx;
    return server->GetPointCloud(ctx, &request, response);
}
::grpc::Status MockCameraStub::GetProperties(
    ::grpc::ClientContext* context,
    const ::viam::component::camera::v1::GetPropertiesRequest& request,
    ::viam::component::camera::v1::GetPropertiesResponse* response) {
    grpc::ServerContext* ctx;
    return server->GetProperties(ctx, &request, response);
}
::grpc::Status MockCameraStub::DoCommand(::grpc::ClientContext* context,
                                         const ::viam::common::v1::DoCommandRequest& request,
                                         ::viam::common::v1::DoCommandResponse* response) {
    grpc::ServerContext* ctx;
    return server->DoCommand(ctx, &request, response);
}

}  // namespace cppsdktests
}  // namespace viam
