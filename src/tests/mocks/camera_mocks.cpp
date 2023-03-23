#include <common/v1/common.pb.h>
#include <component/camera/v1/camera.grpc.pb.h>
#include <component/camera/v1/camera.pb.h>

#include <components/camera/camera.hpp>
#include <components/camera/server.hpp>

class MockCamera : public Camera {
   public:
    AttributeMap do_command(AttributeMap command) override {
        return map;
    }
    raw_image get_image(std::string mime_type) override {
        return image;
    }
    point_cloud get_point_cloud(std::string mime_type) override {
        return pc;
    }
    properties get_properties() override {
        return camera_properties;
    }

    Camera::point_cloud pc;
    Camera::intrinsic_parameters intrinsic_parameters;
    Camera::distortion_parameters distortion_parameters;
    Camera::properties camera_properties;
    Camera::raw_image image;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map;
};

Camera::raw_image raw_image() {
    Camera::raw_image image;
    image.mime_type = "JPEG";
    std::vector<unsigned char> bytes = {'a', 'b', 'c'};
    image.bytes = bytes;
    return image;
}

Camera::point_cloud point_cloud() {
    Camera::point_cloud point_cloud;
    point_cloud.mime_type = "pointcloud/pcd";
    std::vector<unsigned char> bytes = {'a', 'b', 'c'};
    point_cloud.pc = bytes;
    return point_cloud;
}

Camera::intrinsic_parameters intrinsic_parameters() {
    Camera::intrinsic_parameters intrinsic_parameters;
    intrinsic_parameters.width_px = 1;
    intrinsic_parameters.height_px = 2;
    intrinsic_parameters.focal_x_px = 3;
    intrinsic_parameters.focal_y_px = 4;
    intrinsic_parameters.center_x_px = 5;
    intrinsic_parameters.center_y_px = 6;
    return intrinsic_parameters;
}

Camera::distortion_parameters distortion_parameters() {
    Camera::distortion_parameters distortion_parameters;
    distortion_parameters.model = "no distortion";
    return distortion_parameters;
}

Camera::properties properties() {
    Camera::properties properties;
    properties.supports_pcd = true;
    properties.intrinsic_parameters = intrinsic_parameters();
    properties.distortion_parameters = distortion_parameters();
    return properties;
}

std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map() {
    ProtoType prototype = ProtoType(std::string("hello"));
    std::shared_ptr<ProtoType> proto_ptr = std::make_shared<ProtoType>(prototype);
    std::unordered_map<std::string, std::shared_ptr<ProtoType>> map = {
        {std::string("test"), proto_ptr}};
    return std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map);
}

std::shared_ptr<MockCamera> get_mock_camera() {
    MockCamera camera;

    camera.image = raw_image();
    camera.pc = point_cloud();
    camera.camera_properties = properties();
    camera.map = map();

    return std::make_shared<MockCamera>(camera);
}

class MockStub : public viam::component::camera::v1::CameraService::StubInterface {
   public:
    CameraServer server;

    MockStub() : server(CameraServer(std::make_shared<SubtypeService>())) {
        this->server.sub_svc->add(std::string("camera"), get_mock_camera());
    };

    ::grpc::Status GetImage(::grpc::ClientContext* context,
                            const ::viam::component::camera::v1::GetImageRequest& request,
                            ::viam::component::camera::v1::GetImageResponse* response) override {
        grpc::ServerContext* ctx;
        return server.GetImage(ctx, &request, response);
    }
    ::grpc::Status RenderFrame(::grpc::ClientContext* context,
                               const ::viam::component::camera::v1::RenderFrameRequest& request,
                               ::google::api::HttpBody* response) override {
        grpc::ServerContext* ctx;
        return server.RenderFrame(ctx, &request, response);
    }
    ::grpc::Status GetPointCloud(
        ::grpc::ClientContext* context,
        const ::viam::component::camera::v1::GetPointCloudRequest& request,
        ::viam::component::camera::v1::GetPointCloudResponse* response) override {
        grpc::ServerContext* ctx;
        return server.GetPointCloud(ctx, &request, response);
    }
    ::grpc::Status GetProperties(
        ::grpc::ClientContext* context,
        const ::viam::component::camera::v1::GetPropertiesRequest& request,
        ::viam::component::camera::v1::GetPropertiesResponse* response) override {
        grpc::ServerContext* ctx;
        return server.GetProperties(ctx, &request, response);
    }
    ::grpc::Status DoCommand(::grpc::ClientContext* context,
                             const ::viam::common::v1::DoCommandRequest& request,
                             ::viam::common::v1::DoCommandResponse* response) override {
        grpc::ServerContext* ctx;
        return server.DoCommand(ctx, &request, response);
    }

    std::unique_ptr<
        ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetImageResponse>>
    AsyncGetImage(::grpc::ClientContext* context,
                  const ::viam::component::camera::v1::GetImageRequest& request,
                  ::grpc::CompletionQueue* cq) {
        return std::unique_ptr<
            ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetImageResponse>>(
            AsyncGetImageRaw(context, request, cq));
    }
    std::unique_ptr<
        ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetImageResponse>>
    PrepareAsyncGetImage(::grpc::ClientContext* context,
                         const ::viam::component::camera::v1::GetImageRequest& request,
                         ::grpc::CompletionQueue* cq) {
        return std::unique_ptr<
            ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetImageResponse>>(
            PrepareAsyncGetImageRaw(context, request, cq));
    }
    std::unique_ptr<::grpc::ClientAsyncResponseReader<::google::api::HttpBody>> AsyncRenderFrame(
        ::grpc::ClientContext* context,
        const ::viam::component::camera::v1::RenderFrameRequest& request,
        ::grpc::CompletionQueue* cq) {
        return std::unique_ptr<::grpc::ClientAsyncResponseReader<::google::api::HttpBody>>(
            AsyncRenderFrameRaw(context, request, cq));
    }
    std::unique_ptr<::grpc::ClientAsyncResponseReader<::google::api::HttpBody>>
    PrepareAsyncRenderFrame(::grpc::ClientContext* context,
                            const ::viam::component::camera::v1::RenderFrameRequest& request,
                            ::grpc::CompletionQueue* cq) {
        return std::unique_ptr<::grpc::ClientAsyncResponseReader<::google::api::HttpBody>>(
            PrepareAsyncRenderFrameRaw(context, request, cq));
    }

    std::unique_ptr<
        ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetPointCloudResponse>>
    AsyncGetPointCloud(::grpc::ClientContext* context,
                       const ::viam::component::camera::v1::GetPointCloudRequest& request,
                       ::grpc::CompletionQueue* cq) {
        return std::unique_ptr<::grpc::ClientAsyncResponseReader<
            ::viam::component::camera::v1::GetPointCloudResponse>>(
            AsyncGetPointCloudRaw(context, request, cq));
    }
    std::unique_ptr<
        ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetPointCloudResponse>>
    PrepareAsyncGetPointCloud(::grpc::ClientContext* context,
                              const ::viam::component::camera::v1::GetPointCloudRequest& request,
                              ::grpc::CompletionQueue* cq) {
        return std::unique_ptr<::grpc::ClientAsyncResponseReader<
            ::viam::component::camera::v1::GetPointCloudResponse>>(
            PrepareAsyncGetPointCloudRaw(context, request, cq));
    }

    std::unique_ptr<
        ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetPropertiesResponse>>
    AsyncGetProperties(::grpc::ClientContext* context,
                       const ::viam::component::camera::v1::GetPropertiesRequest& request,
                       ::grpc::CompletionQueue* cq) {
        return std::unique_ptr<::grpc::ClientAsyncResponseReader<
            ::viam::component::camera::v1::GetPropertiesResponse>>(
            AsyncGetPropertiesRaw(context, request, cq));
    }
    std::unique_ptr<
        ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetPropertiesResponse>>
    PrepareAsyncGetProperties(::grpc::ClientContext* context,
                              const ::viam::component::camera::v1::GetPropertiesRequest& request,
                              ::grpc::CompletionQueue* cq) {
        return std::unique_ptr<::grpc::ClientAsyncResponseReader<
            ::viam::component::camera::v1::GetPropertiesResponse>>(
            PrepareAsyncGetPropertiesRaw(context, request, cq));
    }

    std::unique_ptr<::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>
    AsyncDoCommand(::grpc::ClientContext* context,
                   const ::viam::common::v1::DoCommandRequest& request,
                   ::grpc::CompletionQueue* cq) {
        return std::unique_ptr<
            ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>(
            AsyncDoCommandRaw(context, request, cq));
    }
    std::unique_ptr<::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>
    PrepareAsyncDoCommand(::grpc::ClientContext* context,
                          const ::viam::common::v1::DoCommandRequest& request,
                          ::grpc::CompletionQueue* cq) {
        return std::unique_ptr<
            ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>(
            PrepareAsyncDoCommandRaw(context, request, cq));
    }

    class async final : public StubInterface::async_interface {
       public:
        void GetImage(::grpc::ClientContext* context,
                      const ::viam::component::camera::v1::GetImageRequest* request,
                      ::viam::component::camera::v1::GetImageResponse* response,
                      std::function<void(::grpc::Status)>) override {
            return;
        }
        void GetImage(::grpc::ClientContext* context,
                      const ::viam::component::camera::v1::GetImageRequest* request,
                      ::viam::component::camera::v1::GetImageResponse* response,
                      ::grpc::ClientUnaryReactor* reactor) override {
            return;
        }
        void RenderFrame(::grpc::ClientContext* context,
                         const ::viam::component::camera::v1::RenderFrameRequest* request,
                         ::google::api::HttpBody* response,
                         std::function<void(::grpc::Status)>) override {
            return;
        }
        void RenderFrame(::grpc::ClientContext* context,
                         const ::viam::component::camera::v1::RenderFrameRequest* request,
                         ::google::api::HttpBody* response,
                         ::grpc::ClientUnaryReactor* reactor) override {
            return;
        }
        void GetPointCloud(::grpc::ClientContext* context,
                           const ::viam::component::camera::v1::GetPointCloudRequest* request,
                           ::viam::component::camera::v1::GetPointCloudResponse* response,
                           std::function<void(::grpc::Status)>) override {
            return;
        }
        void GetPointCloud(::grpc::ClientContext* context,
                           const ::viam::component::camera::v1::GetPointCloudRequest* request,
                           ::viam::component::camera::v1::GetPointCloudResponse* response,
                           ::grpc::ClientUnaryReactor* reactor) override {
            return;
        }
        void GetProperties(::grpc::ClientContext* context,
                           const ::viam::component::camera::v1::GetPropertiesRequest* request,
                           ::viam::component::camera::v1::GetPropertiesResponse* response,
                           std::function<void(::grpc::Status)>) override {
            return;
        }
        void GetProperties(::grpc::ClientContext* context,
                           const ::viam::component::camera::v1::GetPropertiesRequest* request,
                           ::viam::component::camera::v1::GetPropertiesResponse* response,
                           ::grpc::ClientUnaryReactor* reactor) override {
            return;
        }
        void DoCommand(::grpc::ClientContext* context,
                       const ::viam::common::v1::DoCommandRequest* request,
                       ::viam::common::v1::DoCommandResponse* response,
                       std::function<void(::grpc::Status)>) override {
            return;
        }
        void DoCommand(::grpc::ClientContext* context,
                       const ::viam::common::v1::DoCommandRequest* request,
                       ::viam::common::v1::DoCommandResponse* response,
                       ::grpc::ClientUnaryReactor* reactor) override {
            return;
        }

       public:
        friend class Stub;
        explicit async(MockStub* stub) : stub_(stub) {}
        MockStub* stub() {
            return stub_;
        }
        MockStub* stub_;
    };

    class async* async() override {
        return &async_stub_;
    }

    std::shared_ptr<::grpc::ChannelInterface> channel_;
    class async async_stub_ {
        this
    };
    ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetImageResponse>*
    AsyncGetImageRaw(::grpc::ClientContext* context,
                     const ::viam::component::camera::v1::GetImageRequest& request,
                     ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
            ::viam::component::camera::v1::GetImageResponse,
            ::viam::component::camera::v1::GetImageRequest,
            ::grpc::protobuf::MessageLite,
            ::grpc::protobuf::MessageLite>(
            channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetImageResponse>*
    PrepareAsyncGetImageRaw(::grpc::ClientContext* context,
                            const ::viam::component::camera::v1::GetImageRequest& request,
                            ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
            ::viam::component::camera::v1::GetImageResponse,
            ::viam::component::camera::v1::GetImageRequest,
            ::grpc::protobuf::MessageLite,
            ::grpc::protobuf::MessageLite>(
            channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader<::google::api::HttpBody>* AsyncRenderFrameRaw(
        ::grpc::ClientContext* context,
        const ::viam::component::camera::v1::RenderFrameRequest& request,
        ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
            ::google::api::HttpBody,
            ::viam::component::camera::v1::RenderFrameRequest,
            ::grpc::protobuf::MessageLite,
            ::grpc::protobuf::MessageLite>(
            channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader<::google::api::HttpBody>* PrepareAsyncRenderFrameRaw(
        ::grpc::ClientContext* context,
        const ::viam::component::camera::v1::RenderFrameRequest& request,
        ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
            ::google::api::HttpBody,
            ::viam::component::camera::v1::RenderFrameRequest,
            ::grpc::protobuf::MessageLite,
            ::grpc::protobuf::MessageLite>(
            channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetPointCloudResponse>*
    AsyncGetPointCloudRaw(::grpc::ClientContext* context,
                          const ::viam::component::camera::v1::GetPointCloudRequest& request,
                          ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
            ::viam::component::camera::v1::GetPointCloudResponse,
            ::viam::component::camera::v1::GetPointCloudRequest,
            ::grpc::protobuf::MessageLite,
            ::grpc::protobuf::MessageLite>(
            channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetPointCloudResponse>*
    PrepareAsyncGetPointCloudRaw(::grpc::ClientContext* context,
                                 const ::viam::component::camera::v1::GetPointCloudRequest& request,
                                 ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
            ::viam::component::camera::v1::GetPointCloudResponse,
            ::viam::component::camera::v1::GetPointCloudRequest,
            ::grpc::protobuf::MessageLite,
            ::grpc::protobuf::MessageLite>(
            channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetPropertiesResponse>*
    AsyncGetPropertiesRaw(::grpc::ClientContext* context,
                          const ::viam::component::camera::v1::GetPropertiesRequest& request,
                          ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
            ::viam::component::camera::v1::GetPropertiesResponse,
            ::viam::component::camera::v1::GetPropertiesRequest,
            ::grpc::protobuf::MessageLite,
            ::grpc::protobuf::MessageLite>(
            channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader<::viam::component::camera::v1::GetPropertiesResponse>*
    PrepareAsyncGetPropertiesRaw(::grpc::ClientContext* context,
                                 const ::viam::component::camera::v1::GetPropertiesRequest& request,
                                 ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
            ::viam::component::camera::v1::GetPropertiesResponse,
            ::viam::component::camera::v1::GetPropertiesRequest,
            ::grpc::protobuf::MessageLite,
            ::grpc::protobuf::MessageLite>(
            channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>* AsyncDoCommandRaw(
        ::grpc::ClientContext* context,
        const ::viam::common::v1::DoCommandRequest& request,
        ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
            ::viam::common::v1::DoCommandResponse,
            ::viam::common::v1::DoCommandRequest,
            ::grpc::protobuf::MessageLite,
            ::grpc::protobuf::MessageLite>(
            channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>*
    PrepareAsyncDoCommandRaw(::grpc::ClientContext* context,
                             const ::viam::common::v1::DoCommandRequest& request,
                             ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
            ::viam::common::v1::DoCommandResponse,
            ::viam::common::v1::DoCommandRequest,
            ::grpc::protobuf::MessageLite,
            ::grpc::protobuf::MessageLite>(
            channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
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