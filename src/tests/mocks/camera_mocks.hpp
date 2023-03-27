#pragma once

#include <common/v1/common.pb.h>
#include <component/camera/v1/camera.grpc.pb.h>
#include <component/camera/v1/camera.pb.h>

#include <components/camera/camera.hpp>
#include <components/camera/client.hpp>
#include <components/camera/server.hpp>

class MockCamera : public Camera {
public:
  AttributeMap do_command(AttributeMap command) override;
  raw_image get_image(std::string mime_type) override;
  point_cloud get_point_cloud(std::string mime_type) override;
  properties get_properties() override;

  Camera::intrinsic_parameters intrinsic_parameters;
  Camera::distortion_parameters distortion_parameters;
  Camera::properties camera_properties;
  Camera::raw_image image;
  Camera::point_cloud pc;
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
      map;

  static std::shared_ptr<MockCamera> get_mock_camera();
};

Camera::raw_image fake_raw_image();
Camera::point_cloud fake_point_cloud();
Camera::intrinsic_parameters fake_intrinsic_parameters();
Camera::distortion_parameters fake_distortion_parameters();
Camera::properties fake_properties();

class MockCameraStub
    : public viam::component::camera::v1::CameraService::StubInterface {
public:
  CameraServer server;

  MockCameraStub();

  ::grpc::Status
  GetImage(::grpc::ClientContext *context,
           const ::viam::component::camera::v1::GetImageRequest &request,
           ::viam::component::camera::v1::GetImageResponse *response) override;

  ::grpc::Status
  RenderFrame(::grpc::ClientContext *context,
              const ::viam::component::camera::v1::RenderFrameRequest &request,
              ::google::api::HttpBody *response) override;

  ::grpc::Status GetPointCloud(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetPointCloudRequest &request,
      ::viam::component::camera::v1::GetPointCloudResponse *response) override;

  ::grpc::Status GetProperties(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetPropertiesRequest &request,
      ::viam::component::camera::v1::GetPropertiesResponse *response) override;

  ::grpc::Status
  DoCommand(::grpc::ClientContext *context,
            const ::viam::common::v1::DoCommandRequest &request,
            ::viam::common::v1::DoCommandResponse *response) override;

  std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetImageResponse>>
  AsyncGetImage(::grpc::ClientContext *context,
                const ::viam::component::camera::v1::GetImageRequest &request,
                ::grpc::CompletionQueue *cq);
  std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetImageResponse>>
  PrepareAsyncGetImage(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetImageRequest &request,
      ::grpc::CompletionQueue *cq);

  std::unique_ptr<::grpc::ClientAsyncResponseReader<::google::api::HttpBody>>
  AsyncRenderFrame(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::RenderFrameRequest &request,
      ::grpc::CompletionQueue *cq);

  std::unique_ptr<::grpc::ClientAsyncResponseReader<::google::api::HttpBody>>
  PrepareAsyncRenderFrame(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::RenderFrameRequest &request,
      ::grpc::CompletionQueue *cq);

  std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPointCloudResponse>>
  AsyncGetPointCloud(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetPointCloudRequest &request,
      ::grpc::CompletionQueue *cq);

  std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPointCloudResponse>>
  PrepareAsyncGetPointCloud(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetPointCloudRequest &request,
      ::grpc::CompletionQueue *cq);

  std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPropertiesResponse>>
  AsyncGetProperties(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetPropertiesRequest &request,
      ::grpc::CompletionQueue *cq);

  std::unique_ptr<::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPropertiesResponse>>
  PrepareAsyncGetProperties(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetPropertiesRequest &request,
      ::grpc::CompletionQueue *cq);

  std::unique_ptr<
      ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>
  AsyncDoCommand(::grpc::ClientContext *context,
                 const ::viam::common::v1::DoCommandRequest &request,
                 ::grpc::CompletionQueue *cq);
  std::unique_ptr<
      ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>
  PrepareAsyncDoCommand(::grpc::ClientContext *context,
                        const ::viam::common::v1::DoCommandRequest &request,
                        ::grpc::CompletionQueue *cq);

  class async final : public StubInterface::async_interface {
  public:
    void GetImage(::grpc::ClientContext *context,
                  const ::viam::component::camera::v1::GetImageRequest *request,
                  ::viam::component::camera::v1::GetImageResponse *response,
                  std::function<void(::grpc::Status)>) override;

    void GetImage(::grpc::ClientContext *context,
                  const ::viam::component::camera::v1::GetImageRequest *request,
                  ::viam::component::camera::v1::GetImageResponse *response,
                  ::grpc::ClientUnaryReactor *reactor) override;

    void RenderFrame(
        ::grpc::ClientContext *context,
        const ::viam::component::camera::v1::RenderFrameRequest *request,
        ::google::api::HttpBody *response,
        std::function<void(::grpc::Status)>) override;

    void RenderFrame(
        ::grpc::ClientContext *context,
        const ::viam::component::camera::v1::RenderFrameRequest *request,
        ::google::api::HttpBody *response,
        ::grpc::ClientUnaryReactor *reactor) override;

    void GetPointCloud(
        ::grpc::ClientContext *context,
        const ::viam::component::camera::v1::GetPointCloudRequest *request,
        ::viam::component::camera::v1::GetPointCloudResponse *response,
        std::function<void(::grpc::Status)>) override;

    void GetPointCloud(
        ::grpc::ClientContext *context,
        const ::viam::component::camera::v1::GetPointCloudRequest *request,
        ::viam::component::camera::v1::GetPointCloudResponse *response,
        ::grpc::ClientUnaryReactor *reactor) override;

    void GetProperties(
        ::grpc::ClientContext *context,
        const ::viam::component::camera::v1::GetPropertiesRequest *request,
        ::viam::component::camera::v1::GetPropertiesResponse *response,
        std::function<void(::grpc::Status)>) override;

    void GetProperties(
        ::grpc::ClientContext *context,
        const ::viam::component::camera::v1::GetPropertiesRequest *request,
        ::viam::component::camera::v1::GetPropertiesResponse *response,
        ::grpc::ClientUnaryReactor *reactor) override;

    void DoCommand(::grpc::ClientContext *context,
                   const ::viam::common::v1::DoCommandRequest *request,
                   ::viam::common::v1::DoCommandResponse *response,
                   std::function<void(::grpc::Status)>) override;

    void DoCommand(::grpc::ClientContext *context,
                   const ::viam::common::v1::DoCommandRequest *request,
                   ::viam::common::v1::DoCommandResponse *response,
                   ::grpc::ClientUnaryReactor *reactor) override;

  public:
    friend class Stub;
    explicit async(MockCameraStub *stub) : stub_(stub) {}
    MockCameraStub *stub() { return stub_; }
    MockCameraStub *stub_;
  }; 

  class async *async() override { return &async_stub_; }

  std::shared_ptr<::grpc::ChannelInterface> channel_;
  class async async_stub_ {
    this
  };
  ::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetImageResponse> *
  AsyncGetImageRaw(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetImageRequest &request,
      ::grpc::CompletionQueue *cq) override;

  ::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetImageResponse> *
  PrepareAsyncGetImageRaw(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetImageRequest &request,
      ::grpc::CompletionQueue *cq) override;

  ::grpc::ClientAsyncResponseReader<::google::api::HttpBody> *
  AsyncRenderFrameRaw(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::RenderFrameRequest &request,
      ::grpc::CompletionQueue *cq) override;

  ::grpc::ClientAsyncResponseReader<::google::api::HttpBody> *
  PrepareAsyncRenderFrameRaw(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::RenderFrameRequest &request,
      ::grpc::CompletionQueue *cq) override;

  ::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPointCloudResponse> *
  AsyncGetPointCloudRaw(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetPointCloudRequest &request,
      ::grpc::CompletionQueue *cq) override;

  ::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPointCloudResponse> *
  PrepareAsyncGetPointCloudRaw(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetPointCloudRequest &request,
      ::grpc::CompletionQueue *cq) override;

  ::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPropertiesResponse> *
  AsyncGetPropertiesRaw(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetPropertiesRequest &request,
      ::grpc::CompletionQueue *cq) override;

  ::grpc::ClientAsyncResponseReader<
      ::viam::component::camera::v1::GetPropertiesResponse> *
  PrepareAsyncGetPropertiesRaw(
      ::grpc::ClientContext *context,
      const ::viam::component::camera::v1::GetPropertiesRequest &request,
      ::grpc::CompletionQueue *cq) override;

  ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse> *
  AsyncDoCommandRaw(::grpc::ClientContext *context,
                    const ::viam::common::v1::DoCommandRequest &request,
                    ::grpc::CompletionQueue *cq) override;

  ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse> *
  PrepareAsyncDoCommandRaw(::grpc::ClientContext *context,
                           const ::viam::common::v1::DoCommandRequest &request,
                           ::grpc::CompletionQueue *cq) override;

  const ::grpc::internal::RpcServiceMethod::RpcType type =
      ::grpc::internal::RpcServiceMethod::RpcType();
  const ::grpc::internal::RpcMethod rpcmethod_GetImage_ =
      ::grpc::internal::RpcMethod("name", type);
  const ::grpc::internal::RpcMethod rpcmethod_RenderFrame_ =
      ::grpc::internal::RpcMethod("name", type);
  const ::grpc::internal::RpcMethod rpcmethod_GetPointCloud_ =
      ::grpc::internal::RpcMethod("name", type);
  const ::grpc::internal::RpcMethod rpcmethod_GetProperties_ =
      ::grpc::internal::RpcMethod("name", type);
  const ::grpc::internal::RpcMethod rpcmethod_DoCommand_ =
      ::grpc::internal::RpcMethod("name", type);
};

class MockCameraClient : public CameraClient {
public:
  MockCameraClient(std::string name);
};
