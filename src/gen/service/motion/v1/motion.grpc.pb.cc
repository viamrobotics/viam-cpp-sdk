// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: service/motion/v1/motion.proto

#include "service/motion/v1/motion.pb.h"
#include "service/motion/v1/motion.grpc.pb.h"

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
namespace service {
namespace motion {
namespace v1 {

static const char* MotionService_method_names[] = {
  "/viam.service.motion.v1.MotionService/Move",
  "/viam.service.motion.v1.MotionService/MoveSingleComponent",
  "/viam.service.motion.v1.MotionService/GetPose",
  "/viam.service.motion.v1.MotionService/DoCommand",
};

std::unique_ptr< MotionService::Stub> MotionService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< MotionService::Stub> stub(new MotionService::Stub(channel, options));
  return stub;
}

MotionService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Move_(MotionService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_MoveSingleComponent_(MotionService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetPose_(MotionService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DoCommand_(MotionService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status MotionService::Stub::Move(::grpc::ClientContext* context, const ::viam::service::motion::v1::MoveRequest& request, ::viam::service::motion::v1::MoveResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::motion::v1::MoveRequest, ::viam::service::motion::v1::MoveResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Move_, context, request, response);
}

void MotionService::Stub::async::Move(::grpc::ClientContext* context, const ::viam::service::motion::v1::MoveRequest* request, ::viam::service::motion::v1::MoveResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::motion::v1::MoveRequest, ::viam::service::motion::v1::MoveResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Move_, context, request, response, std::move(f));
}

void MotionService::Stub::async::Move(::grpc::ClientContext* context, const ::viam::service::motion::v1::MoveRequest* request, ::viam::service::motion::v1::MoveResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Move_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::motion::v1::MoveResponse>* MotionService::Stub::PrepareAsyncMoveRaw(::grpc::ClientContext* context, const ::viam::service::motion::v1::MoveRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::motion::v1::MoveResponse, ::viam::service::motion::v1::MoveRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Move_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::motion::v1::MoveResponse>* MotionService::Stub::AsyncMoveRaw(::grpc::ClientContext* context, const ::viam::service::motion::v1::MoveRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncMoveRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MotionService::Stub::MoveSingleComponent(::grpc::ClientContext* context, const ::viam::service::motion::v1::MoveSingleComponentRequest& request, ::viam::service::motion::v1::MoveSingleComponentResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::motion::v1::MoveSingleComponentRequest, ::viam::service::motion::v1::MoveSingleComponentResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_MoveSingleComponent_, context, request, response);
}

void MotionService::Stub::async::MoveSingleComponent(::grpc::ClientContext* context, const ::viam::service::motion::v1::MoveSingleComponentRequest* request, ::viam::service::motion::v1::MoveSingleComponentResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::motion::v1::MoveSingleComponentRequest, ::viam::service::motion::v1::MoveSingleComponentResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_MoveSingleComponent_, context, request, response, std::move(f));
}

void MotionService::Stub::async::MoveSingleComponent(::grpc::ClientContext* context, const ::viam::service::motion::v1::MoveSingleComponentRequest* request, ::viam::service::motion::v1::MoveSingleComponentResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_MoveSingleComponent_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::motion::v1::MoveSingleComponentResponse>* MotionService::Stub::PrepareAsyncMoveSingleComponentRaw(::grpc::ClientContext* context, const ::viam::service::motion::v1::MoveSingleComponentRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::motion::v1::MoveSingleComponentResponse, ::viam::service::motion::v1::MoveSingleComponentRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_MoveSingleComponent_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::motion::v1::MoveSingleComponentResponse>* MotionService::Stub::AsyncMoveSingleComponentRaw(::grpc::ClientContext* context, const ::viam::service::motion::v1::MoveSingleComponentRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncMoveSingleComponentRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MotionService::Stub::GetPose(::grpc::ClientContext* context, const ::viam::service::motion::v1::GetPoseRequest& request, ::viam::service::motion::v1::GetPoseResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::motion::v1::GetPoseRequest, ::viam::service::motion::v1::GetPoseResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetPose_, context, request, response);
}

void MotionService::Stub::async::GetPose(::grpc::ClientContext* context, const ::viam::service::motion::v1::GetPoseRequest* request, ::viam::service::motion::v1::GetPoseResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::motion::v1::GetPoseRequest, ::viam::service::motion::v1::GetPoseResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetPose_, context, request, response, std::move(f));
}

void MotionService::Stub::async::GetPose(::grpc::ClientContext* context, const ::viam::service::motion::v1::GetPoseRequest* request, ::viam::service::motion::v1::GetPoseResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetPose_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::motion::v1::GetPoseResponse>* MotionService::Stub::PrepareAsyncGetPoseRaw(::grpc::ClientContext* context, const ::viam::service::motion::v1::GetPoseRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::motion::v1::GetPoseResponse, ::viam::service::motion::v1::GetPoseRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetPose_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::motion::v1::GetPoseResponse>* MotionService::Stub::AsyncGetPoseRaw(::grpc::ClientContext* context, const ::viam::service::motion::v1::GetPoseRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetPoseRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MotionService::Stub::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::viam::common::v1::DoCommandResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DoCommand_, context, request, response);
}

void MotionService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, std::move(f));
}

void MotionService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* MotionService::Stub::PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse, ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* MotionService::Stub::AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoCommandRaw(context, request, cq);
  result->StartCall();
  return result;
}

MotionService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MotionService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MotionService::Service, ::viam::service::motion::v1::MoveRequest, ::viam::service::motion::v1::MoveResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MotionService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::motion::v1::MoveRequest* req,
             ::viam::service::motion::v1::MoveResponse* resp) {
               return service->Move(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MotionService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MotionService::Service, ::viam::service::motion::v1::MoveSingleComponentRequest, ::viam::service::motion::v1::MoveSingleComponentResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MotionService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::motion::v1::MoveSingleComponentRequest* req,
             ::viam::service::motion::v1::MoveSingleComponentResponse* resp) {
               return service->MoveSingleComponent(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MotionService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MotionService::Service, ::viam::service::motion::v1::GetPoseRequest, ::viam::service::motion::v1::GetPoseResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MotionService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::motion::v1::GetPoseRequest* req,
             ::viam::service::motion::v1::GetPoseResponse* resp) {
               return service->GetPose(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MotionService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MotionService::Service, ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MotionService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::common::v1::DoCommandRequest* req,
             ::viam::common::v1::DoCommandResponse* resp) {
               return service->DoCommand(ctx, req, resp);
             }, this)));
}

MotionService::Service::~Service() {
}

::grpc::Status MotionService::Service::Move(::grpc::ServerContext* context, const ::viam::service::motion::v1::MoveRequest* request, ::viam::service::motion::v1::MoveResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MotionService::Service::MoveSingleComponent(::grpc::ServerContext* context, const ::viam::service::motion::v1::MoveSingleComponentRequest* request, ::viam::service::motion::v1::MoveSingleComponentResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MotionService::Service::GetPose(::grpc::ServerContext* context, const ::viam::service::motion::v1::GetPoseRequest* request, ::viam::service::motion::v1::GetPoseResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MotionService::Service::DoCommand(::grpc::ServerContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace service
}  // namespace motion
}  // namespace v1

