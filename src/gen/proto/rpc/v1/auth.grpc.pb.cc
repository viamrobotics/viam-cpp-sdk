// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: proto/rpc/v1/auth.proto

#include "proto/rpc/v1/auth.pb.h"
#include "proto/rpc/v1/auth.grpc.pb.h"

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
namespace proto {
namespace rpc {
namespace v1 {

static const char* AuthService_method_names[] = {
  "/proto.rpc.v1.AuthService/Authenticate",
};

std::unique_ptr< AuthService::Stub> AuthService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< AuthService::Stub> stub(new AuthService::Stub(channel, options));
  return stub;
}

AuthService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Authenticate_(AuthService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status AuthService::Stub::Authenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::proto::rpc::v1::AuthenticateResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::proto::rpc::v1::AuthenticateRequest, ::proto::rpc::v1::AuthenticateResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Authenticate_, context, request, response);
}

void AuthService::Stub::async::Authenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest* request, ::proto::rpc::v1::AuthenticateResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::proto::rpc::v1::AuthenticateRequest, ::proto::rpc::v1::AuthenticateResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Authenticate_, context, request, response, std::move(f));
}

void AuthService::Stub::async::Authenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest* request, ::proto::rpc::v1::AuthenticateResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Authenticate_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateResponse>* AuthService::Stub::PrepareAsyncAuthenticateRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::proto::rpc::v1::AuthenticateResponse, ::proto::rpc::v1::AuthenticateRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Authenticate_, context, request);
}

::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateResponse>* AuthService::Stub::AsyncAuthenticateRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncAuthenticateRaw(context, request, cq);
  result->StartCall();
  return result;
}

AuthService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      AuthService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< AuthService::Service, ::proto::rpc::v1::AuthenticateRequest, ::proto::rpc::v1::AuthenticateResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](AuthService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::proto::rpc::v1::AuthenticateRequest* req,
             ::proto::rpc::v1::AuthenticateResponse* resp) {
               return service->Authenticate(ctx, req, resp);
             }, this)));
}

AuthService::Service::~Service() {
}

::grpc::Status AuthService::Service::Authenticate(::grpc::ServerContext* context, const ::proto::rpc::v1::AuthenticateRequest* request, ::proto::rpc::v1::AuthenticateResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* ExternalAuthService_method_names[] = {
  "/proto.rpc.v1.ExternalAuthService/AuthenticateTo",
};

std::unique_ptr< ExternalAuthService::Stub> ExternalAuthService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< ExternalAuthService::Stub> stub(new ExternalAuthService::Stub(channel, options));
  return stub;
}

ExternalAuthService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_AuthenticateTo_(ExternalAuthService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status ExternalAuthService::Stub::AuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::proto::rpc::v1::AuthenticateToResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::proto::rpc::v1::AuthenticateToRequest, ::proto::rpc::v1::AuthenticateToResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_AuthenticateTo_, context, request, response);
}

void ExternalAuthService::Stub::async::AuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest* request, ::proto::rpc::v1::AuthenticateToResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::proto::rpc::v1::AuthenticateToRequest, ::proto::rpc::v1::AuthenticateToResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_AuthenticateTo_, context, request, response, std::move(f));
}

void ExternalAuthService::Stub::async::AuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest* request, ::proto::rpc::v1::AuthenticateToResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_AuthenticateTo_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateToResponse>* ExternalAuthService::Stub::PrepareAsyncAuthenticateToRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::proto::rpc::v1::AuthenticateToResponse, ::proto::rpc::v1::AuthenticateToRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_AuthenticateTo_, context, request);
}

::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateToResponse>* ExternalAuthService::Stub::AsyncAuthenticateToRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncAuthenticateToRaw(context, request, cq);
  result->StartCall();
  return result;
}

ExternalAuthService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ExternalAuthService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ExternalAuthService::Service, ::proto::rpc::v1::AuthenticateToRequest, ::proto::rpc::v1::AuthenticateToResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ExternalAuthService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::proto::rpc::v1::AuthenticateToRequest* req,
             ::proto::rpc::v1::AuthenticateToResponse* resp) {
               return service->AuthenticateTo(ctx, req, resp);
             }, this)));
}

ExternalAuthService::Service::~Service() {
}

::grpc::Status ExternalAuthService::Service::AuthenticateTo(::grpc::ServerContext* context, const ::proto::rpc::v1::AuthenticateToRequest* request, ::proto::rpc::v1::AuthenticateToResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace proto
}  // namespace rpc
}  // namespace v1

