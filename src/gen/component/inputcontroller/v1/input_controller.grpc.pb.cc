// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: component/inputcontroller/v1/input_controller.proto

#include "component/inputcontroller/v1/input_controller.pb.h"
#include "component/inputcontroller/v1/input_controller.grpc.pb.h"

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
namespace inputcontroller {
namespace v1 {

static const char* InputControllerService_method_names[] = {
  "/viam.component.inputcontroller.v1.InputControllerService/GetControls",
  "/viam.component.inputcontroller.v1.InputControllerService/GetEvents",
  "/viam.component.inputcontroller.v1.InputControllerService/StreamEvents",
  "/viam.component.inputcontroller.v1.InputControllerService/TriggerEvent",
  "/viam.component.inputcontroller.v1.InputControllerService/DoCommand",
};

std::unique_ptr< InputControllerService::Stub> InputControllerService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< InputControllerService::Stub> stub(new InputControllerService::Stub(channel, options));
  return stub;
}

InputControllerService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetControls_(InputControllerService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetEvents_(InputControllerService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_StreamEvents_(InputControllerService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_TriggerEvent_(InputControllerService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DoCommand_(InputControllerService_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status InputControllerService::Stub::GetControls(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::GetControlsRequest& request, ::viam::component::inputcontroller::v1::GetControlsResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::inputcontroller::v1::GetControlsRequest, ::viam::component::inputcontroller::v1::GetControlsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetControls_, context, request, response);
}

void InputControllerService::Stub::async::GetControls(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::GetControlsRequest* request, ::viam::component::inputcontroller::v1::GetControlsResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::inputcontroller::v1::GetControlsRequest, ::viam::component::inputcontroller::v1::GetControlsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetControls_, context, request, response, std::move(f));
}

void InputControllerService::Stub::async::GetControls(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::GetControlsRequest* request, ::viam::component::inputcontroller::v1::GetControlsResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetControls_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::inputcontroller::v1::GetControlsResponse>* InputControllerService::Stub::PrepareAsyncGetControlsRaw(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::GetControlsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::inputcontroller::v1::GetControlsResponse, ::viam::component::inputcontroller::v1::GetControlsRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetControls_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::inputcontroller::v1::GetControlsResponse>* InputControllerService::Stub::AsyncGetControlsRaw(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::GetControlsRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetControlsRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status InputControllerService::Stub::GetEvents(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::GetEventsRequest& request, ::viam::component::inputcontroller::v1::GetEventsResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::inputcontroller::v1::GetEventsRequest, ::viam::component::inputcontroller::v1::GetEventsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetEvents_, context, request, response);
}

void InputControllerService::Stub::async::GetEvents(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::GetEventsRequest* request, ::viam::component::inputcontroller::v1::GetEventsResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::inputcontroller::v1::GetEventsRequest, ::viam::component::inputcontroller::v1::GetEventsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetEvents_, context, request, response, std::move(f));
}

void InputControllerService::Stub::async::GetEvents(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::GetEventsRequest* request, ::viam::component::inputcontroller::v1::GetEventsResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetEvents_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::inputcontroller::v1::GetEventsResponse>* InputControllerService::Stub::PrepareAsyncGetEventsRaw(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::GetEventsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::inputcontroller::v1::GetEventsResponse, ::viam::component::inputcontroller::v1::GetEventsRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetEvents_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::inputcontroller::v1::GetEventsResponse>* InputControllerService::Stub::AsyncGetEventsRaw(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::GetEventsRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetEventsRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::ClientReader< ::viam::component::inputcontroller::v1::StreamEventsResponse>* InputControllerService::Stub::StreamEventsRaw(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::StreamEventsRequest& request) {
  return ::grpc::internal::ClientReaderFactory< ::viam::component::inputcontroller::v1::StreamEventsResponse>::Create(channel_.get(), rpcmethod_StreamEvents_, context, request);
}

void InputControllerService::Stub::async::StreamEvents(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::StreamEventsRequest* request, ::grpc::ClientReadReactor< ::viam::component::inputcontroller::v1::StreamEventsResponse>* reactor) {
  ::grpc::internal::ClientCallbackReaderFactory< ::viam::component::inputcontroller::v1::StreamEventsResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_StreamEvents_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::viam::component::inputcontroller::v1::StreamEventsResponse>* InputControllerService::Stub::AsyncStreamEventsRaw(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::StreamEventsRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::viam::component::inputcontroller::v1::StreamEventsResponse>::Create(channel_.get(), cq, rpcmethod_StreamEvents_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::viam::component::inputcontroller::v1::StreamEventsResponse>* InputControllerService::Stub::PrepareAsyncStreamEventsRaw(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::StreamEventsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::viam::component::inputcontroller::v1::StreamEventsResponse>::Create(channel_.get(), cq, rpcmethod_StreamEvents_, context, request, false, nullptr);
}

::grpc::Status InputControllerService::Stub::TriggerEvent(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::TriggerEventRequest& request, ::viam::component::inputcontroller::v1::TriggerEventResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::inputcontroller::v1::TriggerEventRequest, ::viam::component::inputcontroller::v1::TriggerEventResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_TriggerEvent_, context, request, response);
}

void InputControllerService::Stub::async::TriggerEvent(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::TriggerEventRequest* request, ::viam::component::inputcontroller::v1::TriggerEventResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::inputcontroller::v1::TriggerEventRequest, ::viam::component::inputcontroller::v1::TriggerEventResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_TriggerEvent_, context, request, response, std::move(f));
}

void InputControllerService::Stub::async::TriggerEvent(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::TriggerEventRequest* request, ::viam::component::inputcontroller::v1::TriggerEventResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_TriggerEvent_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::inputcontroller::v1::TriggerEventResponse>* InputControllerService::Stub::PrepareAsyncTriggerEventRaw(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::TriggerEventRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::inputcontroller::v1::TriggerEventResponse, ::viam::component::inputcontroller::v1::TriggerEventRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_TriggerEvent_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::inputcontroller::v1::TriggerEventResponse>* InputControllerService::Stub::AsyncTriggerEventRaw(::grpc::ClientContext* context, const ::viam::component::inputcontroller::v1::TriggerEventRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncTriggerEventRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status InputControllerService::Stub::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::viam::common::v1::DoCommandResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DoCommand_, context, request, response);
}

void InputControllerService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, std::move(f));
}

void InputControllerService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* InputControllerService::Stub::PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse, ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* InputControllerService::Stub::AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoCommandRaw(context, request, cq);
  result->StartCall();
  return result;
}

InputControllerService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      InputControllerService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< InputControllerService::Service, ::viam::component::inputcontroller::v1::GetControlsRequest, ::viam::component::inputcontroller::v1::GetControlsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](InputControllerService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::inputcontroller::v1::GetControlsRequest* req,
             ::viam::component::inputcontroller::v1::GetControlsResponse* resp) {
               return service->GetControls(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      InputControllerService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< InputControllerService::Service, ::viam::component::inputcontroller::v1::GetEventsRequest, ::viam::component::inputcontroller::v1::GetEventsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](InputControllerService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::inputcontroller::v1::GetEventsRequest* req,
             ::viam::component::inputcontroller::v1::GetEventsResponse* resp) {
               return service->GetEvents(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      InputControllerService_method_names[2],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< InputControllerService::Service, ::viam::component::inputcontroller::v1::StreamEventsRequest, ::viam::component::inputcontroller::v1::StreamEventsResponse>(
          [](InputControllerService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::inputcontroller::v1::StreamEventsRequest* req,
             ::grpc::ServerWriter<::viam::component::inputcontroller::v1::StreamEventsResponse>* writer) {
               return service->StreamEvents(ctx, req, writer);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      InputControllerService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< InputControllerService::Service, ::viam::component::inputcontroller::v1::TriggerEventRequest, ::viam::component::inputcontroller::v1::TriggerEventResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](InputControllerService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::inputcontroller::v1::TriggerEventRequest* req,
             ::viam::component::inputcontroller::v1::TriggerEventResponse* resp) {
               return service->TriggerEvent(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      InputControllerService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< InputControllerService::Service, ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](InputControllerService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::common::v1::DoCommandRequest* req,
             ::viam::common::v1::DoCommandResponse* resp) {
               return service->DoCommand(ctx, req, resp);
             }, this)));
}

InputControllerService::Service::~Service() {
}

::grpc::Status InputControllerService::Service::GetControls(::grpc::ServerContext* context, const ::viam::component::inputcontroller::v1::GetControlsRequest* request, ::viam::component::inputcontroller::v1::GetControlsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status InputControllerService::Service::GetEvents(::grpc::ServerContext* context, const ::viam::component::inputcontroller::v1::GetEventsRequest* request, ::viam::component::inputcontroller::v1::GetEventsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status InputControllerService::Service::StreamEvents(::grpc::ServerContext* context, const ::viam::component::inputcontroller::v1::StreamEventsRequest* request, ::grpc::ServerWriter< ::viam::component::inputcontroller::v1::StreamEventsResponse>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status InputControllerService::Service::TriggerEvent(::grpc::ServerContext* context, const ::viam::component::inputcontroller::v1::TriggerEventRequest* request, ::viam::component::inputcontroller::v1::TriggerEventResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status InputControllerService::Service::DoCommand(::grpc::ServerContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace component
}  // namespace inputcontroller
}  // namespace v1

