// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: service/shell/v1/shell.proto

#include "service/shell/v1/shell.pb.h"
#include "service/shell/v1/shell.grpc.pb.h"

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
namespace service {
namespace shell {
namespace v1 {

static const char* ShellService_method_names[] = {
  "/viam.service.shell.v1.ShellService/Shell",
  "/viam.service.shell.v1.ShellService/DoCommand",
};

std::unique_ptr< ShellService::Stub> ShellService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< ShellService::Stub> stub(new ShellService::Stub(channel, options));
  return stub;
}

ShellService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Shell_(ShellService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_DoCommand_(ShellService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::ClientReaderWriter< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>* ShellService::Stub::ShellRaw(::grpc::ClientContext* context) {
  return ::grpc::internal::ClientReaderWriterFactory< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>::Create(channel_.get(), rpcmethod_Shell_, context);
}

void ShellService::Stub::async::Shell(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::viam::service::shell::v1::ShellRequest,::viam::service::shell::v1::ShellResponse>* reactor) {
  ::grpc::internal::ClientCallbackReaderWriterFactory< ::viam::service::shell::v1::ShellRequest,::viam::service::shell::v1::ShellResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_Shell_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>* ShellService::Stub::AsyncShellRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>::Create(channel_.get(), cq, rpcmethod_Shell_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>* ShellService::Stub::PrepareAsyncShellRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>::Create(channel_.get(), cq, rpcmethod_Shell_, context, false, nullptr);
}

::grpc::Status ShellService::Stub::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::viam::common::v1::DoCommandResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DoCommand_, context, request, response);
}

void ShellService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, std::move(f));
}

void ShellService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* ShellService::Stub::PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse, ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* ShellService::Stub::AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoCommandRaw(context, request, cq);
  result->StartCall();
  return result;
}

ShellService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ShellService_method_names[0],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< ShellService::Service, ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>(
          [](ShellService::Service* service,
             ::grpc::ServerContext* ctx,
             ::grpc::ServerReaderWriter<::viam::service::shell::v1::ShellResponse,
             ::viam::service::shell::v1::ShellRequest>* stream) {
               return service->Shell(ctx, stream);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ShellService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ShellService::Service, ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ShellService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::common::v1::DoCommandRequest* req,
             ::viam::common::v1::DoCommandResponse* resp) {
               return service->DoCommand(ctx, req, resp);
             }, this)));
}

ShellService::Service::~Service() {
}

::grpc::Status ShellService::Service::Shell(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::viam::service::shell::v1::ShellResponse, ::viam::service::shell::v1::ShellRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ShellService::Service::DoCommand(::grpc::ServerContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace service
}  // namespace shell
}  // namespace v1

