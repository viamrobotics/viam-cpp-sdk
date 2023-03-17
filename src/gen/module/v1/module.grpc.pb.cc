// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: module/v1/module.proto

#include "module/v1/module.pb.h"
#include "module/v1/module.grpc.pb.h"

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
namespace module {
namespace v1 {

static const char* ModuleService_method_names[] = {
  "/viam.module.v1.ModuleService/AddResource",
  "/viam.module.v1.ModuleService/ReconfigureResource",
  "/viam.module.v1.ModuleService/RemoveResource",
  "/viam.module.v1.ModuleService/Ready",
};

std::unique_ptr< ModuleService::Stub> ModuleService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< ModuleService::Stub> stub(new ModuleService::Stub(channel, options));
  return stub;
}

ModuleService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_AddResource_(ModuleService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ReconfigureResource_(ModuleService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_RemoveResource_(ModuleService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Ready_(ModuleService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status ModuleService::Stub::AddResource(::grpc::ClientContext* context, const ::viam::module::v1::AddResourceRequest& request, ::viam::module::v1::AddResourceResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::module::v1::AddResourceRequest, ::viam::module::v1::AddResourceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_AddResource_, context, request, response);
}

void ModuleService::Stub::async::AddResource(::grpc::ClientContext* context, const ::viam::module::v1::AddResourceRequest* request, ::viam::module::v1::AddResourceResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::module::v1::AddResourceRequest, ::viam::module::v1::AddResourceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_AddResource_, context, request, response, std::move(f));
}

void ModuleService::Stub::async::AddResource(::grpc::ClientContext* context, const ::viam::module::v1::AddResourceRequest* request, ::viam::module::v1::AddResourceResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_AddResource_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::module::v1::AddResourceResponse>* ModuleService::Stub::PrepareAsyncAddResourceRaw(::grpc::ClientContext* context, const ::viam::module::v1::AddResourceRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::module::v1::AddResourceResponse, ::viam::module::v1::AddResourceRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_AddResource_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::module::v1::AddResourceResponse>* ModuleService::Stub::AsyncAddResourceRaw(::grpc::ClientContext* context, const ::viam::module::v1::AddResourceRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncAddResourceRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status ModuleService::Stub::ReconfigureResource(::grpc::ClientContext* context, const ::viam::module::v1::ReconfigureResourceRequest& request, ::viam::module::v1::ReconfigureResourceResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::module::v1::ReconfigureResourceRequest, ::viam::module::v1::ReconfigureResourceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_ReconfigureResource_, context, request, response);
}

void ModuleService::Stub::async::ReconfigureResource(::grpc::ClientContext* context, const ::viam::module::v1::ReconfigureResourceRequest* request, ::viam::module::v1::ReconfigureResourceResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::module::v1::ReconfigureResourceRequest, ::viam::module::v1::ReconfigureResourceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ReconfigureResource_, context, request, response, std::move(f));
}

void ModuleService::Stub::async::ReconfigureResource(::grpc::ClientContext* context, const ::viam::module::v1::ReconfigureResourceRequest* request, ::viam::module::v1::ReconfigureResourceResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ReconfigureResource_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::module::v1::ReconfigureResourceResponse>* ModuleService::Stub::PrepareAsyncReconfigureResourceRaw(::grpc::ClientContext* context, const ::viam::module::v1::ReconfigureResourceRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::module::v1::ReconfigureResourceResponse, ::viam::module::v1::ReconfigureResourceRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_ReconfigureResource_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::module::v1::ReconfigureResourceResponse>* ModuleService::Stub::AsyncReconfigureResourceRaw(::grpc::ClientContext* context, const ::viam::module::v1::ReconfigureResourceRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncReconfigureResourceRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status ModuleService::Stub::RemoveResource(::grpc::ClientContext* context, const ::viam::module::v1::RemoveResourceRequest& request, ::viam::module::v1::RemoveResourceResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::module::v1::RemoveResourceRequest, ::viam::module::v1::RemoveResourceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_RemoveResource_, context, request, response);
}

void ModuleService::Stub::async::RemoveResource(::grpc::ClientContext* context, const ::viam::module::v1::RemoveResourceRequest* request, ::viam::module::v1::RemoveResourceResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::module::v1::RemoveResourceRequest, ::viam::module::v1::RemoveResourceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RemoveResource_, context, request, response, std::move(f));
}

void ModuleService::Stub::async::RemoveResource(::grpc::ClientContext* context, const ::viam::module::v1::RemoveResourceRequest* request, ::viam::module::v1::RemoveResourceResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RemoveResource_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::module::v1::RemoveResourceResponse>* ModuleService::Stub::PrepareAsyncRemoveResourceRaw(::grpc::ClientContext* context, const ::viam::module::v1::RemoveResourceRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::module::v1::RemoveResourceResponse, ::viam::module::v1::RemoveResourceRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_RemoveResource_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::module::v1::RemoveResourceResponse>* ModuleService::Stub::AsyncRemoveResourceRaw(::grpc::ClientContext* context, const ::viam::module::v1::RemoveResourceRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncRemoveResourceRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status ModuleService::Stub::Ready(::grpc::ClientContext* context, const ::viam::module::v1::ReadyRequest& request, ::viam::module::v1::ReadyResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::module::v1::ReadyRequest, ::viam::module::v1::ReadyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Ready_, context, request, response);
}

void ModuleService::Stub::async::Ready(::grpc::ClientContext* context, const ::viam::module::v1::ReadyRequest* request, ::viam::module::v1::ReadyResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::module::v1::ReadyRequest, ::viam::module::v1::ReadyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Ready_, context, request, response, std::move(f));
}

void ModuleService::Stub::async::Ready(::grpc::ClientContext* context, const ::viam::module::v1::ReadyRequest* request, ::viam::module::v1::ReadyResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Ready_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::module::v1::ReadyResponse>* ModuleService::Stub::PrepareAsyncReadyRaw(::grpc::ClientContext* context, const ::viam::module::v1::ReadyRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::module::v1::ReadyResponse, ::viam::module::v1::ReadyRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Ready_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::module::v1::ReadyResponse>* ModuleService::Stub::AsyncReadyRaw(::grpc::ClientContext* context, const ::viam::module::v1::ReadyRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncReadyRaw(context, request, cq);
  result->StartCall();
  return result;
}

ModuleService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ModuleService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ModuleService::Service, ::viam::module::v1::AddResourceRequest, ::viam::module::v1::AddResourceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ModuleService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::module::v1::AddResourceRequest* req,
             ::viam::module::v1::AddResourceResponse* resp) {
               return service->AddResource(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ModuleService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ModuleService::Service, ::viam::module::v1::ReconfigureResourceRequest, ::viam::module::v1::ReconfigureResourceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ModuleService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::module::v1::ReconfigureResourceRequest* req,
             ::viam::module::v1::ReconfigureResourceResponse* resp) {
               return service->ReconfigureResource(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ModuleService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ModuleService::Service, ::viam::module::v1::RemoveResourceRequest, ::viam::module::v1::RemoveResourceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ModuleService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::module::v1::RemoveResourceRequest* req,
             ::viam::module::v1::RemoveResourceResponse* resp) {
               return service->RemoveResource(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ModuleService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ModuleService::Service, ::viam::module::v1::ReadyRequest, ::viam::module::v1::ReadyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ModuleService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::module::v1::ReadyRequest* req,
             ::viam::module::v1::ReadyResponse* resp) {
               return service->Ready(ctx, req, resp);
             }, this)));
}

ModuleService::Service::~Service() {
}

::grpc::Status ModuleService::Service::AddResource(::grpc::ServerContext* context, const ::viam::module::v1::AddResourceRequest* request, ::viam::module::v1::AddResourceResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ModuleService::Service::ReconfigureResource(::grpc::ServerContext* context, const ::viam::module::v1::ReconfigureResourceRequest* request, ::viam::module::v1::ReconfigureResourceResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ModuleService::Service::RemoveResource(::grpc::ServerContext* context, const ::viam::module::v1::RemoveResourceRequest* request, ::viam::module::v1::RemoveResourceResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ModuleService::Service::Ready(::grpc::ServerContext* context, const ::viam::module::v1::ReadyRequest* request, ::viam::module::v1::ReadyResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace module
}  // namespace v1

