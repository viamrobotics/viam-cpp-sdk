// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: component/camera/v1/camera.proto

#include "component/camera/v1/camera.pb.h"
#include "component/camera/v1/camera.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace viam {
namespace component {
namespace camera {
namespace v1 {

static const char* CameraService_method_names[] = {
  "/viam.component.camera.v1.CameraService/GetImage",
  "/viam.component.camera.v1.CameraService/RenderFrame",
  "/viam.component.camera.v1.CameraService/GetPointCloud",
  "/viam.component.camera.v1.CameraService/GetProperties",
  "/viam.component.camera.v1.CameraService/DoCommand",
};

std::unique_ptr< CameraService::Stub> CameraService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< CameraService::Stub> stub(new CameraService::Stub(channel, options));
  return stub;
}

CameraService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetImage_(CameraService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_RenderFrame_(CameraService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetPointCloud_(CameraService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetProperties_(CameraService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DoCommand_(CameraService_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status CameraService::Stub::GetImage(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest& request, ::viam::component::camera::v1::GetImageResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::camera::v1::GetImageRequest, ::viam::component::camera::v1::GetImageResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetImage_, context, request, response);
}

void CameraService::Stub::async::GetImage(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest* request, ::viam::component::camera::v1::GetImageResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::camera::v1::GetImageRequest, ::viam::component::camera::v1::GetImageResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetImage_, context, request, response, std::move(f));
}

void CameraService::Stub::async::GetImage(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest* request, ::viam::component::camera::v1::GetImageResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetImage_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetImageResponse>* CameraService::Stub::PrepareAsyncGetImageRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::camera::v1::GetImageResponse, ::viam::component::camera::v1::GetImageRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetImageResponse>* CameraService::Stub::AsyncGetImageRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetImageRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status CameraService::Stub::RenderFrame(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest& request, ::google::api::HttpBody* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::camera::v1::RenderFrameRequest, ::google::api::HttpBody, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_RenderFrame_, context, request, response);
}

void CameraService::Stub::async::RenderFrame(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest* request, ::google::api::HttpBody* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::camera::v1::RenderFrameRequest, ::google::api::HttpBody, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RenderFrame_, context, request, response, std::move(f));
}

void CameraService::Stub::async::RenderFrame(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest* request, ::google::api::HttpBody* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RenderFrame_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::google::api::HttpBody>* CameraService::Stub::PrepareAsyncRenderFrameRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::google::api::HttpBody, ::viam::component::camera::v1::RenderFrameRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_RenderFrame_, context, request);
}

::grpc::ClientAsyncResponseReader< ::google::api::HttpBody>* CameraService::Stub::AsyncRenderFrameRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncRenderFrameRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status CameraService::Stub::GetPointCloud(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest& request, ::viam::component::camera::v1::GetPointCloudResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::camera::v1::GetPointCloudRequest, ::viam::component::camera::v1::GetPointCloudResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetPointCloud_, context, request, response);
}

void CameraService::Stub::async::GetPointCloud(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest* request, ::viam::component::camera::v1::GetPointCloudResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::camera::v1::GetPointCloudRequest, ::viam::component::camera::v1::GetPointCloudResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetPointCloud_, context, request, response, std::move(f));
}

void CameraService::Stub::async::GetPointCloud(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest* request, ::viam::component::camera::v1::GetPointCloudResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetPointCloud_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPointCloudResponse>* CameraService::Stub::PrepareAsyncGetPointCloudRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::camera::v1::GetPointCloudResponse, ::viam::component::camera::v1::GetPointCloudRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetPointCloud_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPointCloudResponse>* CameraService::Stub::AsyncGetPointCloudRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetPointCloudRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status CameraService::Stub::GetProperties(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest& request, ::viam::component::camera::v1::GetPropertiesResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::camera::v1::GetPropertiesRequest, ::viam::component::camera::v1::GetPropertiesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetProperties_, context, request, response);
}

void CameraService::Stub::async::GetProperties(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest* request, ::viam::component::camera::v1::GetPropertiesResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::camera::v1::GetPropertiesRequest, ::viam::component::camera::v1::GetPropertiesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetProperties_, context, request, response, std::move(f));
}

void CameraService::Stub::async::GetProperties(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest* request, ::viam::component::camera::v1::GetPropertiesResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetProperties_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPropertiesResponse>* CameraService::Stub::PrepareAsyncGetPropertiesRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::camera::v1::GetPropertiesResponse, ::viam::component::camera::v1::GetPropertiesRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetProperties_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPropertiesResponse>* CameraService::Stub::AsyncGetPropertiesRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetPropertiesRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status CameraService::Stub::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::viam::common::v1::DoCommandResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DoCommand_, context, request, response);
}

void CameraService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, std::move(f));
}

void CameraService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* CameraService::Stub::PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse, ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* CameraService::Stub::AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoCommandRaw(context, request, cq);
  result->StartCall();
  return result;
}

CameraService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      CameraService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< CameraService::Service, ::viam::component::camera::v1::GetImageRequest, ::viam::component::camera::v1::GetImageResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](CameraService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::camera::v1::GetImageRequest* req,
             ::viam::component::camera::v1::GetImageResponse* resp) {
               return service->GetImage(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      CameraService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< CameraService::Service, ::viam::component::camera::v1::RenderFrameRequest, ::google::api::HttpBody, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](CameraService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::camera::v1::RenderFrameRequest* req,
             ::google::api::HttpBody* resp) {
               return service->RenderFrame(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      CameraService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< CameraService::Service, ::viam::component::camera::v1::GetPointCloudRequest, ::viam::component::camera::v1::GetPointCloudResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](CameraService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::camera::v1::GetPointCloudRequest* req,
             ::viam::component::camera::v1::GetPointCloudResponse* resp) {
               return service->GetPointCloud(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      CameraService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< CameraService::Service, ::viam::component::camera::v1::GetPropertiesRequest, ::viam::component::camera::v1::GetPropertiesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](CameraService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::camera::v1::GetPropertiesRequest* req,
             ::viam::component::camera::v1::GetPropertiesResponse* resp) {
               return service->GetProperties(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      CameraService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< CameraService::Service, ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](CameraService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::common::v1::DoCommandRequest* req,
             ::viam::common::v1::DoCommandResponse* resp) {
               return service->DoCommand(ctx, req, resp);
             }, this)));
}

CameraService::Service::~Service() {
}

::grpc::Status CameraService::Service::GetImage(::grpc::ServerContext* context, const ::viam::component::camera::v1::GetImageRequest* request, ::viam::component::camera::v1::GetImageResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status CameraService::Service::RenderFrame(::grpc::ServerContext* context, const ::viam::component::camera::v1::RenderFrameRequest* request, ::google::api::HttpBody* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status CameraService::Service::GetPointCloud(::grpc::ServerContext* context, const ::viam::component::camera::v1::GetPointCloudRequest* request, ::viam::component::camera::v1::GetPointCloudResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status CameraService::Service::GetProperties(::grpc::ServerContext* context, const ::viam::component::camera::v1::GetPropertiesRequest* request, ::viam::component::camera::v1::GetPropertiesResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status CameraService::Service::DoCommand(::grpc::ServerContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace component
}  // namespace camera
}  // namespace v1

