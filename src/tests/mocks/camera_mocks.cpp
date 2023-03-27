#include <common/v1/common.pb.h>
#include <component/camera/v1/camera.grpc.pb.h>
#include <component/camera/v1/camera.pb.h>

#include <components/camera/camera.hpp>
#include <components/camera/server.hpp>
#include <tests/mocks/camera_mocks.hpp>
#include <tests/test_utils.hpp>

AttributeMap MockCamera::do_command(AttributeMap command) { return map; }
Camera::raw_image MockCamera::get_image(std::string mime_type) { return image; }
Camera::point_cloud MockCamera::get_point_cloud(std::string mime_type) {
  return pc;
}
Camera::properties MockCamera::get_properties() { return camera_properties; }

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
  std::shared_ptr<MockCamera> camera = std::make_shared<MockCamera>();

  camera->image = fake_raw_image();
  camera->pc = fake_point_cloud();
  camera->camera_properties = fake_properties();
  camera->map = fake_map();

  return camera;
}

MockCameraStub::MockCameraStub()
    : server(CameraServer(std::make_shared<SubtypeService>())) {
  this->server.get_sub_svc()->add(std::string("camera"),
                                  MockCamera::get_mock_camera());
};

::grpc::Status MockCameraStub::GetImage(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetImageRequest &request,
    ::viam::component::camera::v1::GetImageResponse *response) {
  grpc::ServerContext *ctx;
  return server.GetImage(ctx, &request, response);
}
::grpc::Status MockCameraStub::RenderFrame(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::RenderFrameRequest &request,
    ::google::api::HttpBody *response) {
  grpc::ServerContext *ctx;
  return server.RenderFrame(ctx, &request, response);
}
::grpc::Status MockCameraStub::GetPointCloud(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPointCloudRequest &request,
    ::viam::component::camera::v1::GetPointCloudResponse *response) {
  grpc::ServerContext *ctx;
  return server.GetPointCloud(ctx, &request, response);
}
::grpc::Status MockCameraStub::GetProperties(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPropertiesRequest &request,
    ::viam::component::camera::v1::GetPropertiesResponse *response) {
  grpc::ServerContext *ctx;
  return server.GetProperties(ctx, &request, response);
}
::grpc::Status
MockCameraStub::DoCommand(::grpc::ClientContext *context,
                          const ::viam::common::v1::DoCommandRequest &request,
                          ::viam::common::v1::DoCommandResponse *response) {
  grpc::ServerContext *ctx;
  return server.DoCommand(ctx, &request, response);
}

std::unique_ptr<::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetImageResponse>>
MockCameraStub::AsyncGetImage(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetImageRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetImageResponse>>(
      AsyncGetImageRaw(context, request, cq));
}
std::unique_ptr<::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetImageResponse>>
MockCameraStub::PrepareAsyncGetImage(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetImageRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetImageResponse>>(
      PrepareAsyncGetImageRaw(context, request, cq));
}
std::unique_ptr<::grpc::ClientAsyncResponseReader<::google::api::HttpBody>>
MockCameraStub::AsyncRenderFrame(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::RenderFrameRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<
      ::grpc::ClientAsyncResponseReader<::google::api::HttpBody>>(
      AsyncRenderFrameRaw(context, request, cq));
}
std::unique_ptr<::grpc::ClientAsyncResponseReader<::google::api::HttpBody>>
MockCameraStub::PrepareAsyncRenderFrame(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::RenderFrameRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<
      ::grpc::ClientAsyncResponseReader<::google::api::HttpBody>>(
      PrepareAsyncRenderFrameRaw(context, request, cq));
}

std::unique_ptr<::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetPointCloudResponse>>
MockCameraStub::AsyncGetPointCloud(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPointCloudRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPointCloudResponse>>(
      AsyncGetPointCloudRaw(context, request, cq));
}
std::unique_ptr<::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetPointCloudResponse>>
MockCameraStub::PrepareAsyncGetPointCloud(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPointCloudRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPointCloudResponse>>(
      PrepareAsyncGetPointCloudRaw(context, request, cq));
}

std::unique_ptr<::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetPropertiesResponse>>
MockCameraStub::AsyncGetProperties(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPropertiesRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPropertiesResponse>>(
      AsyncGetPropertiesRaw(context, request, cq));
}
std::unique_ptr<::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetPropertiesResponse>>
MockCameraStub::PrepareAsyncGetProperties(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPropertiesRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPropertiesResponse>>(
      PrepareAsyncGetPropertiesRaw(context, request, cq));
}

std::unique_ptr<
    ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>
MockCameraStub::AsyncDoCommand(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<
      ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>(
      AsyncDoCommandRaw(context, request, cq));
}
std::unique_ptr<
    ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>
MockCameraStub::PrepareAsyncDoCommand(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<
      ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>(
      PrepareAsyncDoCommandRaw(context, request, cq));
}

void MockCameraStub::async::GetImage(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetImageRequest *request,
    ::viam::component::camera::v1::GetImageResponse *response,
    std::function<void(::grpc::Status)>) {
  return;
}
void MockCameraStub::async::GetImage(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetImageRequest *request,
    ::viam::component::camera::v1::GetImageResponse *response,
    ::grpc::ClientUnaryReactor *reactor) {
  return;
}
void MockCameraStub::async::RenderFrame(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::RenderFrameRequest *request,
    ::google::api::HttpBody *response, std::function<void(::grpc::Status)>) {
  return;
}
void MockCameraStub::async::RenderFrame(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::RenderFrameRequest *request,
    ::google::api::HttpBody *response, ::grpc::ClientUnaryReactor *reactor) {
  return;
}
void MockCameraStub::async::GetPointCloud(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPointCloudRequest *request,
    ::viam::component::camera::v1::GetPointCloudResponse *response,
    std::function<void(::grpc::Status)>) {
  return;
}
void MockCameraStub::async::GetPointCloud(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPointCloudRequest *request,
    ::viam::component::camera::v1::GetPointCloudResponse *response,
    ::grpc::ClientUnaryReactor *reactor) {
  return;
}
void MockCameraStub::async::GetProperties(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPropertiesRequest *request,
    ::viam::component::camera::v1::GetPropertiesResponse *response,
    std::function<void(::grpc::Status)>) {
  return;
}
void MockCameraStub::async::GetProperties(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPropertiesRequest *request,
    ::viam::component::camera::v1::GetPropertiesResponse *response,
    ::grpc::ClientUnaryReactor *reactor) {
  return;
}
void MockCameraStub::async::DoCommand(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest *request,
    ::viam::common::v1::DoCommandResponse *response,
    std::function<void(::grpc::Status)>) {
  return;
}
void MockCameraStub::async::DoCommand(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest *request,
    ::viam::common::v1::DoCommandResponse *response,
    ::grpc::ClientUnaryReactor *reactor) {
  return;
}

::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetImageResponse> *
MockCameraStub::AsyncGetImageRaw(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetImageRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::viam::component::camera::v1::GetImageResponse,
      ::viam::component::camera::v1::GetImageRequest,
      ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
      channel_.get(), cq, rpcmethod_GetImage_, context, request);
}
::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetImageResponse> *
MockCameraStub::PrepareAsyncGetImageRaw(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetImageRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::viam::component::camera::v1::GetImageResponse,
      ::viam::component::camera::v1::GetImageRequest,
      ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
      channel_.get(), cq, rpcmethod_GetImage_, context, request);
}
::grpc::ClientAsyncResponseReader<::google::api::HttpBody> *
MockCameraStub::AsyncRenderFrameRaw(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::RenderFrameRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::google::api::HttpBody,
      ::viam::component::camera::v1::RenderFrameRequest,
      ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
      channel_.get(), cq, rpcmethod_GetImage_, context, request);
}
::grpc::ClientAsyncResponseReader<::google::api::HttpBody> *
MockCameraStub::PrepareAsyncRenderFrameRaw(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::RenderFrameRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::google::api::HttpBody,
      ::viam::component::camera::v1::RenderFrameRequest,
      ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
      channel_.get(), cq, rpcmethod_GetImage_, context, request);
}
::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetPointCloudResponse> *
MockCameraStub::AsyncGetPointCloudRaw(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPointCloudRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::viam::component::camera::v1::GetPointCloudResponse,
      ::viam::component::camera::v1::GetPointCloudRequest,
      ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
      channel_.get(), cq, rpcmethod_GetImage_, context, request);
}
::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetPointCloudResponse> *
MockCameraStub::PrepareAsyncGetPointCloudRaw(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPointCloudRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::viam::component::camera::v1::GetPointCloudResponse,
      ::viam::component::camera::v1::GetPointCloudRequest,
      ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
      channel_.get(), cq, rpcmethod_GetImage_, context, request);
}
::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetPropertiesResponse> *
MockCameraStub::AsyncGetPropertiesRaw(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPropertiesRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::viam::component::camera::v1::GetPropertiesResponse,
      ::viam::component::camera::v1::GetPropertiesRequest,
      ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
      channel_.get(), cq, rpcmethod_GetImage_, context, request);
}
::grpc::ClientAsyncResponseReader<
    ::viam::component::camera::v1::GetPropertiesResponse> *
MockCameraStub::PrepareAsyncGetPropertiesRaw(
    ::grpc::ClientContext *context,
    const ::viam::component::camera::v1::GetPropertiesRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::viam::component::camera::v1::GetPropertiesResponse,
      ::viam::component::camera::v1::GetPropertiesRequest,
      ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
      channel_.get(), cq, rpcmethod_GetImage_, context, request);
}
::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse> *
MockCameraStub::AsyncDoCommandRaw(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::viam::common::v1::DoCommandResponse,
      ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite,
      ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_,
                                     context, request);
}
::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse> *
MockCameraStub::PrepareAsyncDoCommandRaw(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::viam::common::v1::DoCommandResponse,
      ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite,
      ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_,
                                     context, request);
}

MockCameraClient::MockCameraClient(std::string name) : CameraClient(name) {
  stub_ = std::make_unique<MockCameraStub>();
  name_ = name;
}
