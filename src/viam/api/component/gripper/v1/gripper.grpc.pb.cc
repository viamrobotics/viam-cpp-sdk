// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: component/gripper/v1/gripper.proto

#include "component/gripper/v1/gripper.pb.h"
#include "component/gripper/v1/gripper.grpc.pb.h"

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
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace viam {
namespace component {
namespace gripper {
namespace v1 {

static const char* GripperService_method_names[] = {
  "/viam.component.gripper.v1.GripperService/Open",
  "/viam.component.gripper.v1.GripperService/Grab",
  "/viam.component.gripper.v1.GripperService/Stop",
  "/viam.component.gripper.v1.GripperService/IsMoving",
  "/viam.component.gripper.v1.GripperService/DoCommand",
  "/viam.component.gripper.v1.GripperService/GetGeometries",
};

std::unique_ptr< GripperService::Stub> GripperService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< GripperService::Stub> stub(new GripperService::Stub(channel, options));
  return stub;
}

GripperService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Open_(GripperService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Grab_(GripperService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Stop_(GripperService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_IsMoving_(GripperService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DoCommand_(GripperService_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetGeometries_(GripperService_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status GripperService::Stub::Open(::grpc::ClientContext* context, const ::viam::component::gripper::v1::OpenRequest& request, ::viam::component::gripper::v1::OpenResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::gripper::v1::OpenRequest, ::viam::component::gripper::v1::OpenResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Open_, context, request, response);
}

void GripperService::Stub::async::Open(::grpc::ClientContext* context, const ::viam::component::gripper::v1::OpenRequest* request, ::viam::component::gripper::v1::OpenResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::gripper::v1::OpenRequest, ::viam::component::gripper::v1::OpenResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Open_, context, request, response, std::move(f));
}

void GripperService::Stub::async::Open(::grpc::ClientContext* context, const ::viam::component::gripper::v1::OpenRequest* request, ::viam::component::gripper::v1::OpenResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Open_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::gripper::v1::OpenResponse>* GripperService::Stub::PrepareAsyncOpenRaw(::grpc::ClientContext* context, const ::viam::component::gripper::v1::OpenRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::gripper::v1::OpenResponse, ::viam::component::gripper::v1::OpenRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Open_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::gripper::v1::OpenResponse>* GripperService::Stub::AsyncOpenRaw(::grpc::ClientContext* context, const ::viam::component::gripper::v1::OpenRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncOpenRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status GripperService::Stub::Grab(::grpc::ClientContext* context, const ::viam::component::gripper::v1::GrabRequest& request, ::viam::component::gripper::v1::GrabResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::gripper::v1::GrabRequest, ::viam::component::gripper::v1::GrabResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Grab_, context, request, response);
}

void GripperService::Stub::async::Grab(::grpc::ClientContext* context, const ::viam::component::gripper::v1::GrabRequest* request, ::viam::component::gripper::v1::GrabResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::gripper::v1::GrabRequest, ::viam::component::gripper::v1::GrabResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Grab_, context, request, response, std::move(f));
}

void GripperService::Stub::async::Grab(::grpc::ClientContext* context, const ::viam::component::gripper::v1::GrabRequest* request, ::viam::component::gripper::v1::GrabResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Grab_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::gripper::v1::GrabResponse>* GripperService::Stub::PrepareAsyncGrabRaw(::grpc::ClientContext* context, const ::viam::component::gripper::v1::GrabRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::gripper::v1::GrabResponse, ::viam::component::gripper::v1::GrabRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Grab_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::gripper::v1::GrabResponse>* GripperService::Stub::AsyncGrabRaw(::grpc::ClientContext* context, const ::viam::component::gripper::v1::GrabRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGrabRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status GripperService::Stub::Stop(::grpc::ClientContext* context, const ::viam::component::gripper::v1::StopRequest& request, ::viam::component::gripper::v1::StopResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::gripper::v1::StopRequest, ::viam::component::gripper::v1::StopResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Stop_, context, request, response);
}

void GripperService::Stub::async::Stop(::grpc::ClientContext* context, const ::viam::component::gripper::v1::StopRequest* request, ::viam::component::gripper::v1::StopResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::gripper::v1::StopRequest, ::viam::component::gripper::v1::StopResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Stop_, context, request, response, std::move(f));
}

void GripperService::Stub::async::Stop(::grpc::ClientContext* context, const ::viam::component::gripper::v1::StopRequest* request, ::viam::component::gripper::v1::StopResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Stop_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::gripper::v1::StopResponse>* GripperService::Stub::PrepareAsyncStopRaw(::grpc::ClientContext* context, const ::viam::component::gripper::v1::StopRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::gripper::v1::StopResponse, ::viam::component::gripper::v1::StopRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Stop_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::gripper::v1::StopResponse>* GripperService::Stub::AsyncStopRaw(::grpc::ClientContext* context, const ::viam::component::gripper::v1::StopRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncStopRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status GripperService::Stub::IsMoving(::grpc::ClientContext* context, const ::viam::component::gripper::v1::IsMovingRequest& request, ::viam::component::gripper::v1::IsMovingResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::gripper::v1::IsMovingRequest, ::viam::component::gripper::v1::IsMovingResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_IsMoving_, context, request, response);
}

void GripperService::Stub::async::IsMoving(::grpc::ClientContext* context, const ::viam::component::gripper::v1::IsMovingRequest* request, ::viam::component::gripper::v1::IsMovingResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::gripper::v1::IsMovingRequest, ::viam::component::gripper::v1::IsMovingResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_IsMoving_, context, request, response, std::move(f));
}

void GripperService::Stub::async::IsMoving(::grpc::ClientContext* context, const ::viam::component::gripper::v1::IsMovingRequest* request, ::viam::component::gripper::v1::IsMovingResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_IsMoving_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::gripper::v1::IsMovingResponse>* GripperService::Stub::PrepareAsyncIsMovingRaw(::grpc::ClientContext* context, const ::viam::component::gripper::v1::IsMovingRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::gripper::v1::IsMovingResponse, ::viam::component::gripper::v1::IsMovingRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_IsMoving_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::gripper::v1::IsMovingResponse>* GripperService::Stub::AsyncIsMovingRaw(::grpc::ClientContext* context, const ::viam::component::gripper::v1::IsMovingRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncIsMovingRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status GripperService::Stub::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::viam::common::v1::DoCommandResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DoCommand_, context, request, response);
}

void GripperService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, std::move(f));
}

void GripperService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* GripperService::Stub::PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse, ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* GripperService::Stub::AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoCommandRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status GripperService::Stub::GetGeometries(::grpc::ClientContext* context, const ::viam::common::v1::GetGeometriesRequest& request, ::viam::common::v1::GetGeometriesResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::common::v1::GetGeometriesRequest, ::viam::common::v1::GetGeometriesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetGeometries_, context, request, response);
}

void GripperService::Stub::async::GetGeometries(::grpc::ClientContext* context, const ::viam::common::v1::GetGeometriesRequest* request, ::viam::common::v1::GetGeometriesResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::common::v1::GetGeometriesRequest, ::viam::common::v1::GetGeometriesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetGeometries_, context, request, response, std::move(f));
}

void GripperService::Stub::async::GetGeometries(::grpc::ClientContext* context, const ::viam::common::v1::GetGeometriesRequest* request, ::viam::common::v1::GetGeometriesResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetGeometries_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::GetGeometriesResponse>* GripperService::Stub::PrepareAsyncGetGeometriesRaw(::grpc::ClientContext* context, const ::viam::common::v1::GetGeometriesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::GetGeometriesResponse, ::viam::common::v1::GetGeometriesRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetGeometries_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::GetGeometriesResponse>* GripperService::Stub::AsyncGetGeometriesRaw(::grpc::ClientContext* context, const ::viam::common::v1::GetGeometriesRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetGeometriesRaw(context, request, cq);
  result->StartCall();
  return result;
}

GripperService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      GripperService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< GripperService::Service, ::viam::component::gripper::v1::OpenRequest, ::viam::component::gripper::v1::OpenResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](GripperService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::gripper::v1::OpenRequest* req,
             ::viam::component::gripper::v1::OpenResponse* resp) {
               return service->Open(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      GripperService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< GripperService::Service, ::viam::component::gripper::v1::GrabRequest, ::viam::component::gripper::v1::GrabResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](GripperService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::gripper::v1::GrabRequest* req,
             ::viam::component::gripper::v1::GrabResponse* resp) {
               return service->Grab(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      GripperService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< GripperService::Service, ::viam::component::gripper::v1::StopRequest, ::viam::component::gripper::v1::StopResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](GripperService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::gripper::v1::StopRequest* req,
             ::viam::component::gripper::v1::StopResponse* resp) {
               return service->Stop(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      GripperService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< GripperService::Service, ::viam::component::gripper::v1::IsMovingRequest, ::viam::component::gripper::v1::IsMovingResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](GripperService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::gripper::v1::IsMovingRequest* req,
             ::viam::component::gripper::v1::IsMovingResponse* resp) {
               return service->IsMoving(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      GripperService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< GripperService::Service, ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](GripperService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::common::v1::DoCommandRequest* req,
             ::viam::common::v1::DoCommandResponse* resp) {
               return service->DoCommand(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      GripperService_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< GripperService::Service, ::viam::common::v1::GetGeometriesRequest, ::viam::common::v1::GetGeometriesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](GripperService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::common::v1::GetGeometriesRequest* req,
             ::viam::common::v1::GetGeometriesResponse* resp) {
               return service->GetGeometries(ctx, req, resp);
             }, this)));
}

GripperService::Service::~Service() {
}

::grpc::Status GripperService::Service::Open(::grpc::ServerContext* context, const ::viam::component::gripper::v1::OpenRequest* request, ::viam::component::gripper::v1::OpenResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status GripperService::Service::Grab(::grpc::ServerContext* context, const ::viam::component::gripper::v1::GrabRequest* request, ::viam::component::gripper::v1::GrabResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status GripperService::Service::Stop(::grpc::ServerContext* context, const ::viam::component::gripper::v1::StopRequest* request, ::viam::component::gripper::v1::StopResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status GripperService::Service::IsMoving(::grpc::ServerContext* context, const ::viam::component::gripper::v1::IsMovingRequest* request, ::viam::component::gripper::v1::IsMovingResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status GripperService::Service::DoCommand(::grpc::ServerContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status GripperService::Service::GetGeometries(::grpc::ServerContext* context, const ::viam::common::v1::GetGeometriesRequest* request, ::viam::common::v1::GetGeometriesResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace component
}  // namespace gripper
}  // namespace v1

