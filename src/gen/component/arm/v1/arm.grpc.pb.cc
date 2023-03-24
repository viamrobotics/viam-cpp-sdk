// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: component/arm/v1/arm.proto

#include "component/arm/v1/arm.pb.h"
#include "component/arm/v1/arm.grpc.pb.h"

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
namespace arm {
namespace v1 {

static const char* ArmService_method_names[] = {
  "/viam.component.arm.v1.ArmService/GetEndPosition",
  "/viam.component.arm.v1.ArmService/MoveToPosition",
  "/viam.component.arm.v1.ArmService/GetJointPositions",
  "/viam.component.arm.v1.ArmService/MoveToJointPositions",
  "/viam.component.arm.v1.ArmService/Stop",
  "/viam.component.arm.v1.ArmService/IsMoving",
  "/viam.component.arm.v1.ArmService/DoCommand",
};

std::unique_ptr< ArmService::Stub> ArmService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< ArmService::Stub> stub(new ArmService::Stub(channel, options));
  return stub;
}

ArmService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetEndPosition_(ArmService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_MoveToPosition_(ArmService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetJointPositions_(ArmService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_MoveToJointPositions_(ArmService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Stop_(ArmService_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_IsMoving_(ArmService_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DoCommand_(ArmService_method_names[6], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status ArmService::Stub::GetEndPosition(::grpc::ClientContext* context, const ::viam::component::arm::v1::GetEndPositionRequest& request, ::viam::component::arm::v1::GetEndPositionResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::arm::v1::GetEndPositionRequest, ::viam::component::arm::v1::GetEndPositionResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetEndPosition_, context, request, response);
}

void ArmService::Stub::async::GetEndPosition(::grpc::ClientContext* context, const ::viam::component::arm::v1::GetEndPositionRequest* request, ::viam::component::arm::v1::GetEndPositionResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::arm::v1::GetEndPositionRequest, ::viam::component::arm::v1::GetEndPositionResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetEndPosition_, context, request, response, std::move(f));
}

void ArmService::Stub::async::GetEndPosition(::grpc::ClientContext* context, const ::viam::component::arm::v1::GetEndPositionRequest* request, ::viam::component::arm::v1::GetEndPositionResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetEndPosition_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::GetEndPositionResponse>* ArmService::Stub::PrepareAsyncGetEndPositionRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::GetEndPositionRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::arm::v1::GetEndPositionResponse, ::viam::component::arm::v1::GetEndPositionRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetEndPosition_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::GetEndPositionResponse>* ArmService::Stub::AsyncGetEndPositionRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::GetEndPositionRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetEndPositionRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status ArmService::Stub::MoveToPosition(::grpc::ClientContext* context, const ::viam::component::arm::v1::MoveToPositionRequest& request, ::viam::component::arm::v1::MoveToPositionResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::arm::v1::MoveToPositionRequest, ::viam::component::arm::v1::MoveToPositionResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_MoveToPosition_, context, request, response);
}

void ArmService::Stub::async::MoveToPosition(::grpc::ClientContext* context, const ::viam::component::arm::v1::MoveToPositionRequest* request, ::viam::component::arm::v1::MoveToPositionResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::arm::v1::MoveToPositionRequest, ::viam::component::arm::v1::MoveToPositionResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_MoveToPosition_, context, request, response, std::move(f));
}

void ArmService::Stub::async::MoveToPosition(::grpc::ClientContext* context, const ::viam::component::arm::v1::MoveToPositionRequest* request, ::viam::component::arm::v1::MoveToPositionResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_MoveToPosition_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::MoveToPositionResponse>* ArmService::Stub::PrepareAsyncMoveToPositionRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::MoveToPositionRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::arm::v1::MoveToPositionResponse, ::viam::component::arm::v1::MoveToPositionRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_MoveToPosition_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::MoveToPositionResponse>* ArmService::Stub::AsyncMoveToPositionRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::MoveToPositionRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncMoveToPositionRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status ArmService::Stub::GetJointPositions(::grpc::ClientContext* context, const ::viam::component::arm::v1::GetJointPositionsRequest& request, ::viam::component::arm::v1::GetJointPositionsResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::arm::v1::GetJointPositionsRequest, ::viam::component::arm::v1::GetJointPositionsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetJointPositions_, context, request, response);
}

void ArmService::Stub::async::GetJointPositions(::grpc::ClientContext* context, const ::viam::component::arm::v1::GetJointPositionsRequest* request, ::viam::component::arm::v1::GetJointPositionsResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::arm::v1::GetJointPositionsRequest, ::viam::component::arm::v1::GetJointPositionsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetJointPositions_, context, request, response, std::move(f));
}

void ArmService::Stub::async::GetJointPositions(::grpc::ClientContext* context, const ::viam::component::arm::v1::GetJointPositionsRequest* request, ::viam::component::arm::v1::GetJointPositionsResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetJointPositions_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::GetJointPositionsResponse>* ArmService::Stub::PrepareAsyncGetJointPositionsRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::GetJointPositionsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::arm::v1::GetJointPositionsResponse, ::viam::component::arm::v1::GetJointPositionsRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetJointPositions_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::GetJointPositionsResponse>* ArmService::Stub::AsyncGetJointPositionsRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::GetJointPositionsRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetJointPositionsRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status ArmService::Stub::MoveToJointPositions(::grpc::ClientContext* context, const ::viam::component::arm::v1::MoveToJointPositionsRequest& request, ::viam::component::arm::v1::MoveToJointPositionsResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::arm::v1::MoveToJointPositionsRequest, ::viam::component::arm::v1::MoveToJointPositionsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_MoveToJointPositions_, context, request, response);
}

void ArmService::Stub::async::MoveToJointPositions(::grpc::ClientContext* context, const ::viam::component::arm::v1::MoveToJointPositionsRequest* request, ::viam::component::arm::v1::MoveToJointPositionsResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::arm::v1::MoveToJointPositionsRequest, ::viam::component::arm::v1::MoveToJointPositionsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_MoveToJointPositions_, context, request, response, std::move(f));
}

void ArmService::Stub::async::MoveToJointPositions(::grpc::ClientContext* context, const ::viam::component::arm::v1::MoveToJointPositionsRequest* request, ::viam::component::arm::v1::MoveToJointPositionsResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_MoveToJointPositions_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::MoveToJointPositionsResponse>* ArmService::Stub::PrepareAsyncMoveToJointPositionsRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::MoveToJointPositionsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::arm::v1::MoveToJointPositionsResponse, ::viam::component::arm::v1::MoveToJointPositionsRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_MoveToJointPositions_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::MoveToJointPositionsResponse>* ArmService::Stub::AsyncMoveToJointPositionsRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::MoveToJointPositionsRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncMoveToJointPositionsRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status ArmService::Stub::Stop(::grpc::ClientContext* context, const ::viam::component::arm::v1::StopRequest& request, ::viam::component::arm::v1::StopResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::arm::v1::StopRequest, ::viam::component::arm::v1::StopResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Stop_, context, request, response);
}

void ArmService::Stub::async::Stop(::grpc::ClientContext* context, const ::viam::component::arm::v1::StopRequest* request, ::viam::component::arm::v1::StopResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::arm::v1::StopRequest, ::viam::component::arm::v1::StopResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Stop_, context, request, response, std::move(f));
}

void ArmService::Stub::async::Stop(::grpc::ClientContext* context, const ::viam::component::arm::v1::StopRequest* request, ::viam::component::arm::v1::StopResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Stop_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::StopResponse>* ArmService::Stub::PrepareAsyncStopRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::StopRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::arm::v1::StopResponse, ::viam::component::arm::v1::StopRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Stop_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::StopResponse>* ArmService::Stub::AsyncStopRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::StopRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncStopRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status ArmService::Stub::IsMoving(::grpc::ClientContext* context, const ::viam::component::arm::v1::IsMovingRequest& request, ::viam::component::arm::v1::IsMovingResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::arm::v1::IsMovingRequest, ::viam::component::arm::v1::IsMovingResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_IsMoving_, context, request, response);
}

void ArmService::Stub::async::IsMoving(::grpc::ClientContext* context, const ::viam::component::arm::v1::IsMovingRequest* request, ::viam::component::arm::v1::IsMovingResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::arm::v1::IsMovingRequest, ::viam::component::arm::v1::IsMovingResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_IsMoving_, context, request, response, std::move(f));
}

void ArmService::Stub::async::IsMoving(::grpc::ClientContext* context, const ::viam::component::arm::v1::IsMovingRequest* request, ::viam::component::arm::v1::IsMovingResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_IsMoving_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::IsMovingResponse>* ArmService::Stub::PrepareAsyncIsMovingRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::IsMovingRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::arm::v1::IsMovingResponse, ::viam::component::arm::v1::IsMovingRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_IsMoving_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::arm::v1::IsMovingResponse>* ArmService::Stub::AsyncIsMovingRaw(::grpc::ClientContext* context, const ::viam::component::arm::v1::IsMovingRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncIsMovingRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status ArmService::Stub::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::viam::common::v1::DoCommandResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DoCommand_, context, request, response);
}

void ArmService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, std::move(f));
}

void ArmService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* ArmService::Stub::PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse, ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* ArmService::Stub::AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoCommandRaw(context, request, cq);
  result->StartCall();
  return result;
}

ArmService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ArmService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ArmService::Service, ::viam::component::arm::v1::GetEndPositionRequest, ::viam::component::arm::v1::GetEndPositionResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ArmService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::arm::v1::GetEndPositionRequest* req,
             ::viam::component::arm::v1::GetEndPositionResponse* resp) {
               return service->GetEndPosition(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ArmService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ArmService::Service, ::viam::component::arm::v1::MoveToPositionRequest, ::viam::component::arm::v1::MoveToPositionResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ArmService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::arm::v1::MoveToPositionRequest* req,
             ::viam::component::arm::v1::MoveToPositionResponse* resp) {
               return service->MoveToPosition(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ArmService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ArmService::Service, ::viam::component::arm::v1::GetJointPositionsRequest, ::viam::component::arm::v1::GetJointPositionsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ArmService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::arm::v1::GetJointPositionsRequest* req,
             ::viam::component::arm::v1::GetJointPositionsResponse* resp) {
               return service->GetJointPositions(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ArmService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ArmService::Service, ::viam::component::arm::v1::MoveToJointPositionsRequest, ::viam::component::arm::v1::MoveToJointPositionsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ArmService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::arm::v1::MoveToJointPositionsRequest* req,
             ::viam::component::arm::v1::MoveToJointPositionsResponse* resp) {
               return service->MoveToJointPositions(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ArmService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ArmService::Service, ::viam::component::arm::v1::StopRequest, ::viam::component::arm::v1::StopResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ArmService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::arm::v1::StopRequest* req,
             ::viam::component::arm::v1::StopResponse* resp) {
               return service->Stop(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ArmService_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ArmService::Service, ::viam::component::arm::v1::IsMovingRequest, ::viam::component::arm::v1::IsMovingResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ArmService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::arm::v1::IsMovingRequest* req,
             ::viam::component::arm::v1::IsMovingResponse* resp) {
               return service->IsMoving(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ArmService_method_names[6],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ArmService::Service, ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ArmService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::common::v1::DoCommandRequest* req,
             ::viam::common::v1::DoCommandResponse* resp) {
               return service->DoCommand(ctx, req, resp);
             }, this)));
}

ArmService::Service::~Service() {
}

::grpc::Status ArmService::Service::GetEndPosition(::grpc::ServerContext* context, const ::viam::component::arm::v1::GetEndPositionRequest* request, ::viam::component::arm::v1::GetEndPositionResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ArmService::Service::MoveToPosition(::grpc::ServerContext* context, const ::viam::component::arm::v1::MoveToPositionRequest* request, ::viam::component::arm::v1::MoveToPositionResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ArmService::Service::GetJointPositions(::grpc::ServerContext* context, const ::viam::component::arm::v1::GetJointPositionsRequest* request, ::viam::component::arm::v1::GetJointPositionsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ArmService::Service::MoveToJointPositions(::grpc::ServerContext* context, const ::viam::component::arm::v1::MoveToJointPositionsRequest* request, ::viam::component::arm::v1::MoveToJointPositionsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ArmService::Service::Stop(::grpc::ServerContext* context, const ::viam::component::arm::v1::StopRequest* request, ::viam::component::arm::v1::StopResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ArmService::Service::IsMoving(::grpc::ServerContext* context, const ::viam::component::arm::v1::IsMovingRequest* request, ::viam::component::arm::v1::IsMovingResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ArmService::Service::DoCommand(::grpc::ServerContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace component
}  // namespace arm
}  // namespace v1

