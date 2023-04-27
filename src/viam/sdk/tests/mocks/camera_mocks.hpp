#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>
#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/components/camera/client.hpp>
#include <viam/sdk/components/camera/server.hpp>

namespace viam {
namespace sdktests {
namespace camera {

using namespace viam::sdk;

class MockCamera : public Camera {
   public:
    virtual ~MockCamera();
    AttributeMap do_command(AttributeMap command) override;
    raw_image get_image(std::string mime_type) override;
    point_cloud get_point_cloud(std::string mime_type) override;
    properties get_properties() override;
    static std::shared_ptr<MockCamera> get_mock_camera();
    MockCamera(std::string name) : Camera(std::move(name)){};

   private:
    Camera::intrinsic_parameters intrinsic_parameters_;
    Camera::distortion_parameters distortion_parameters_;
    Camera::properties camera_properties_;
    Camera::raw_image image_;
    Camera::point_cloud pc_;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map_;
};

Camera::raw_image fake_raw_image();
Camera::point_cloud fake_point_cloud();
Camera::intrinsic_parameters fake_intrinsic_parameters();
Camera::distortion_parameters fake_distortion_parameters();
Camera::properties fake_properties();

class MockCameraStub : public viam::component::camera::v1::CameraService::StubInterface {
   public:
    MockCameraStub();
    ::grpc::Status GetImage(::grpc::ClientContext* context,
                            const ::viam::component::camera::v1::GetImageRequest& request,
                            ::viam::component::camera::v1::GetImageResponse* response) override;

    ::grpc::Status RenderFrame(::grpc::ClientContext* context,
                               const ::viam::component::camera::v1::RenderFrameRequest& request,
                               ::google::api::HttpBody* response) override;

    ::grpc::Status GetPointCloud(
        ::grpc::ClientContext* context,
        const ::viam::component::camera::v1::GetPointCloudRequest& request,
        ::viam::component::camera::v1::GetPointCloudResponse* response) override;

    ::grpc::Status GetProperties(
        ::grpc::ClientContext* context,
        const ::viam::component::camera::v1::GetPropertiesRequest& request,
        ::viam::component::camera::v1::GetPropertiesResponse* response) override;

    ::grpc::Status DoCommand(::grpc::ClientContext* context,
                             const ::viam::common::v1::DoCommandRequest& request,
                             ::viam::common::v1::DoCommandResponse* response) override;

   private:
    std::shared_ptr<CameraServer> server_;

    ::grpc::ClientAsyncResponseReaderInterface<::viam::component::camera::v1::GetImageResponse>*
    AsyncGetImageRaw(::grpc::ClientContext* context,
                     const ::viam::component::camera::v1::GetImageRequest& request,
                     ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("unimplemented");
    };
    ::grpc::ClientAsyncResponseReaderInterface<::viam::component::camera::v1::GetImageResponse>*
    PrepareAsyncGetImageRaw(::grpc::ClientContext* context,
                            const ::viam::component::camera::v1::GetImageRequest& request,
                            ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("unimplemented");
    };
    ::grpc::ClientAsyncResponseReaderInterface<::google::api::HttpBody>* AsyncRenderFrameRaw(
        ::grpc::ClientContext* context,
        const ::viam::component::camera::v1::RenderFrameRequest& request,
        ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("unimplemented");
    };
    ::grpc::ClientAsyncResponseReaderInterface<::google::api::HttpBody>* PrepareAsyncRenderFrameRaw(
        ::grpc::ClientContext* context,
        const ::viam::component::camera::v1::RenderFrameRequest& request,
        ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("unimplemented");
    };
    ::grpc::ClientAsyncResponseReaderInterface<
        ::viam::component::camera::v1::GetPointCloudResponse>*
    AsyncGetPointCloudRaw(::grpc::ClientContext* context,
                          const ::viam::component::camera::v1::GetPointCloudRequest& request,
                          ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("unimplemented");
    };
    ::grpc::ClientAsyncResponseReaderInterface<
        ::viam::component::camera::v1::GetPointCloudResponse>*
    PrepareAsyncGetPointCloudRaw(::grpc::ClientContext* context,
                                 const ::viam::component::camera::v1::GetPointCloudRequest& request,
                                 ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("unimplemented");
    };
    ::grpc::ClientAsyncResponseReaderInterface<
        ::viam::component::camera::v1::GetPropertiesResponse>*
    AsyncGetPropertiesRaw(::grpc::ClientContext* context,
                          const ::viam::component::camera::v1::GetPropertiesRequest& request,
                          ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("unimplemented");
    };
    ::grpc::ClientAsyncResponseReaderInterface<
        ::viam::component::camera::v1::GetPropertiesResponse>*
    PrepareAsyncGetPropertiesRaw(::grpc::ClientContext* context,
                                 const ::viam::component::camera::v1::GetPropertiesRequest& request,
                                 ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("unimplemented");
    };
    ::grpc::ClientAsyncResponseReaderInterface<::viam::common::v1::DoCommandResponse>*
    AsyncDoCommandRaw(::grpc::ClientContext* context,
                      const ::viam::common::v1::DoCommandRequest& request,
                      ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("unimplemented");
    };
    ::grpc::ClientAsyncResponseReaderInterface<::viam::common::v1::DoCommandResponse>*
    PrepareAsyncDoCommandRaw(::grpc::ClientContext* context,
                             const ::viam::common::v1::DoCommandRequest& request,
                             ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("unimplemented");
    };
};

class MockCameraClient : public CameraClient {
   public:
    MockCameraClient(std::string name)
        : CameraClient(std::move(name), std::make_unique<MockCameraStub>()){};
};

}  // namespace camera
}  // namespace sdktests
}  // namespace viam
