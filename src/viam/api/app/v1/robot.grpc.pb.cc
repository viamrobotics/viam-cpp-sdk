// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: app/v1/robot.proto

#include "app/v1/robot.pb.h"
#include "app/v1/robot.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace viam {
namespace app {
namespace v1 {

static const char* RobotService_method_names[] = {
  "/viam.app.v1.RobotService/Config",
  "/viam.app.v1.RobotService/Certificate",
  "/viam.app.v1.RobotService/Log",
  "/viam.app.v1.RobotService/NeedsRestart",
};

std::unique_ptr< RobotService::Stub> RobotService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< RobotService::Stub> stub(new RobotService::Stub(channel));
  return stub;
}

RobotService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Config_(RobotService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Certificate_(RobotService_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Log_(RobotService_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_NeedsRestart_(RobotService_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status RobotService::Stub::Config(::grpc::ClientContext* context, const ::viam::app::v1::ConfigRequest& request, ::viam::app::v1::ConfigResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Config_, context, request, response);
}

void RobotService::Stub::experimental_async::Config(::grpc::ClientContext* context, const ::viam::app::v1::ConfigRequest* request, ::viam::app::v1::ConfigResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Config_, context, request, response, std::move(f));
}

void RobotService::Stub::experimental_async::Config(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::viam::app::v1::ConfigResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Config_, context, request, response, std::move(f));
}

void RobotService::Stub::experimental_async::Config(::grpc::ClientContext* context, const ::viam::app::v1::ConfigRequest* request, ::viam::app::v1::ConfigResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Config_, context, request, response, reactor);
}

void RobotService::Stub::experimental_async::Config(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::viam::app::v1::ConfigResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Config_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::ConfigResponse>* RobotService::Stub::AsyncConfigRaw(::grpc::ClientContext* context, const ::viam::app::v1::ConfigRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::viam::app::v1::ConfigResponse>::Create(channel_.get(), cq, rpcmethod_Config_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::ConfigResponse>* RobotService::Stub::PrepareAsyncConfigRaw(::grpc::ClientContext* context, const ::viam::app::v1::ConfigRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::viam::app::v1::ConfigResponse>::Create(channel_.get(), cq, rpcmethod_Config_, context, request, false);
}

::grpc::Status RobotService::Stub::Certificate(::grpc::ClientContext* context, const ::viam::app::v1::CertificateRequest& request, ::viam::app::v1::CertificateResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Certificate_, context, request, response);
}

void RobotService::Stub::experimental_async::Certificate(::grpc::ClientContext* context, const ::viam::app::v1::CertificateRequest* request, ::viam::app::v1::CertificateResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Certificate_, context, request, response, std::move(f));
}

void RobotService::Stub::experimental_async::Certificate(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::viam::app::v1::CertificateResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Certificate_, context, request, response, std::move(f));
}

void RobotService::Stub::experimental_async::Certificate(::grpc::ClientContext* context, const ::viam::app::v1::CertificateRequest* request, ::viam::app::v1::CertificateResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Certificate_, context, request, response, reactor);
}

void RobotService::Stub::experimental_async::Certificate(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::viam::app::v1::CertificateResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Certificate_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::CertificateResponse>* RobotService::Stub::AsyncCertificateRaw(::grpc::ClientContext* context, const ::viam::app::v1::CertificateRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::viam::app::v1::CertificateResponse>::Create(channel_.get(), cq, rpcmethod_Certificate_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::CertificateResponse>* RobotService::Stub::PrepareAsyncCertificateRaw(::grpc::ClientContext* context, const ::viam::app::v1::CertificateRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::viam::app::v1::CertificateResponse>::Create(channel_.get(), cq, rpcmethod_Certificate_, context, request, false);
}

::grpc::Status RobotService::Stub::Log(::grpc::ClientContext* context, const ::viam::app::v1::LogRequest& request, ::viam::app::v1::LogResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Log_, context, request, response);
}

void RobotService::Stub::experimental_async::Log(::grpc::ClientContext* context, const ::viam::app::v1::LogRequest* request, ::viam::app::v1::LogResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Log_, context, request, response, std::move(f));
}

void RobotService::Stub::experimental_async::Log(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::viam::app::v1::LogResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Log_, context, request, response, std::move(f));
}

void RobotService::Stub::experimental_async::Log(::grpc::ClientContext* context, const ::viam::app::v1::LogRequest* request, ::viam::app::v1::LogResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Log_, context, request, response, reactor);
}

void RobotService::Stub::experimental_async::Log(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::viam::app::v1::LogResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Log_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::LogResponse>* RobotService::Stub::AsyncLogRaw(::grpc::ClientContext* context, const ::viam::app::v1::LogRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::viam::app::v1::LogResponse>::Create(channel_.get(), cq, rpcmethod_Log_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::LogResponse>* RobotService::Stub::PrepareAsyncLogRaw(::grpc::ClientContext* context, const ::viam::app::v1::LogRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::viam::app::v1::LogResponse>::Create(channel_.get(), cq, rpcmethod_Log_, context, request, false);
}

::grpc::Status RobotService::Stub::NeedsRestart(::grpc::ClientContext* context, const ::viam::app::v1::NeedsRestartRequest& request, ::viam::app::v1::NeedsRestartResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_NeedsRestart_, context, request, response);
}

void RobotService::Stub::experimental_async::NeedsRestart(::grpc::ClientContext* context, const ::viam::app::v1::NeedsRestartRequest* request, ::viam::app::v1::NeedsRestartResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_NeedsRestart_, context, request, response, std::move(f));
}

void RobotService::Stub::experimental_async::NeedsRestart(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::viam::app::v1::NeedsRestartResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_NeedsRestart_, context, request, response, std::move(f));
}

void RobotService::Stub::experimental_async::NeedsRestart(::grpc::ClientContext* context, const ::viam::app::v1::NeedsRestartRequest* request, ::viam::app::v1::NeedsRestartResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_NeedsRestart_, context, request, response, reactor);
}

void RobotService::Stub::experimental_async::NeedsRestart(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::viam::app::v1::NeedsRestartResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_NeedsRestart_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::NeedsRestartResponse>* RobotService::Stub::AsyncNeedsRestartRaw(::grpc::ClientContext* context, const ::viam::app::v1::NeedsRestartRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::viam::app::v1::NeedsRestartResponse>::Create(channel_.get(), cq, rpcmethod_NeedsRestart_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::NeedsRestartResponse>* RobotService::Stub::PrepareAsyncNeedsRestartRaw(::grpc::ClientContext* context, const ::viam::app::v1::NeedsRestartRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::viam::app::v1::NeedsRestartResponse>::Create(channel_.get(), cq, rpcmethod_NeedsRestart_, context, request, false);
}

RobotService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RobotService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RobotService::Service, ::viam::app::v1::ConfigRequest, ::viam::app::v1::ConfigResponse>(
          [](RobotService::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::viam::app::v1::ConfigRequest* req,
             ::viam::app::v1::ConfigResponse* resp) {
               return service->Config(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RobotService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RobotService::Service, ::viam::app::v1::CertificateRequest, ::viam::app::v1::CertificateResponse>(
          [](RobotService::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::viam::app::v1::CertificateRequest* req,
             ::viam::app::v1::CertificateResponse* resp) {
               return service->Certificate(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RobotService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RobotService::Service, ::viam::app::v1::LogRequest, ::viam::app::v1::LogResponse>(
          [](RobotService::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::viam::app::v1::LogRequest* req,
             ::viam::app::v1::LogResponse* resp) {
               return service->Log(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RobotService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RobotService::Service, ::viam::app::v1::NeedsRestartRequest, ::viam::app::v1::NeedsRestartResponse>(
          [](RobotService::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::viam::app::v1::NeedsRestartRequest* req,
             ::viam::app::v1::NeedsRestartResponse* resp) {
               return service->NeedsRestart(ctx, req, resp);
             }, this)));
}

RobotService::Service::~Service() {
}

::grpc::Status RobotService::Service::Config(::grpc::ServerContext* context, const ::viam::app::v1::ConfigRequest* request, ::viam::app::v1::ConfigResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RobotService::Service::Certificate(::grpc::ServerContext* context, const ::viam::app::v1::CertificateRequest* request, ::viam::app::v1::CertificateResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RobotService::Service::Log(::grpc::ServerContext* context, const ::viam::app::v1::LogRequest* request, ::viam::app::v1::LogResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RobotService::Service::NeedsRestart(::grpc::ServerContext* context, const ::viam::app::v1::NeedsRestartRequest* request, ::viam::app::v1::NeedsRestartResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace app
}  // namespace v1

