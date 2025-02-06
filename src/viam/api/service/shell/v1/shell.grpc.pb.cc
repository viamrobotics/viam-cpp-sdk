// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: service/shell/v1/shell.proto

#include "service/shell/v1/shell.pb.h"
#include "service/shell/v1/shell.grpc.pb.h"

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
namespace service {
namespace shell {
namespace v1 {

static const char* ShellService_method_names[] = {
  "/viam.service.shell.v1.ShellService/Shell",
  "/viam.service.shell.v1.ShellService/CopyFilesToMachine",
  "/viam.service.shell.v1.ShellService/CopyFilesFromMachine",
  "/viam.service.shell.v1.ShellService/DoCommand",
};

std::unique_ptr< ShellService::Stub> ShellService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< ShellService::Stub> stub(new ShellService::Stub(channel));
  return stub;
}

ShellService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Shell_(ShellService_method_names[0], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_CopyFilesToMachine_(ShellService_method_names[1], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_CopyFilesFromMachine_(ShellService_method_names[2], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_DoCommand_(ShellService_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::ClientReaderWriter< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>* ShellService::Stub::ShellRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>::Create(channel_.get(), rpcmethod_Shell_, context);
}

void ShellService::Stub::experimental_async::Shell(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::viam::service::shell::v1::ShellRequest,::viam::service::shell::v1::ShellResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::viam::service::shell::v1::ShellRequest,::viam::service::shell::v1::ShellResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_Shell_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>* ShellService::Stub::AsyncShellRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>::Create(channel_.get(), cq, rpcmethod_Shell_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>* ShellService::Stub::PrepareAsyncShellRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>::Create(channel_.get(), cq, rpcmethod_Shell_, context, false, nullptr);
}

::grpc::ClientReaderWriter< ::viam::service::shell::v1::CopyFilesToMachineRequest, ::viam::service::shell::v1::CopyFilesToMachineResponse>* ShellService::Stub::CopyFilesToMachineRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::viam::service::shell::v1::CopyFilesToMachineRequest, ::viam::service::shell::v1::CopyFilesToMachineResponse>::Create(channel_.get(), rpcmethod_CopyFilesToMachine_, context);
}

void ShellService::Stub::experimental_async::CopyFilesToMachine(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::viam::service::shell::v1::CopyFilesToMachineRequest,::viam::service::shell::v1::CopyFilesToMachineResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::viam::service::shell::v1::CopyFilesToMachineRequest,::viam::service::shell::v1::CopyFilesToMachineResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_CopyFilesToMachine_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::viam::service::shell::v1::CopyFilesToMachineRequest, ::viam::service::shell::v1::CopyFilesToMachineResponse>* ShellService::Stub::AsyncCopyFilesToMachineRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::viam::service::shell::v1::CopyFilesToMachineRequest, ::viam::service::shell::v1::CopyFilesToMachineResponse>::Create(channel_.get(), cq, rpcmethod_CopyFilesToMachine_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::viam::service::shell::v1::CopyFilesToMachineRequest, ::viam::service::shell::v1::CopyFilesToMachineResponse>* ShellService::Stub::PrepareAsyncCopyFilesToMachineRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::viam::service::shell::v1::CopyFilesToMachineRequest, ::viam::service::shell::v1::CopyFilesToMachineResponse>::Create(channel_.get(), cq, rpcmethod_CopyFilesToMachine_, context, false, nullptr);
}

::grpc::ClientReaderWriter< ::viam::service::shell::v1::CopyFilesFromMachineRequest, ::viam::service::shell::v1::CopyFilesFromMachineResponse>* ShellService::Stub::CopyFilesFromMachineRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::viam::service::shell::v1::CopyFilesFromMachineRequest, ::viam::service::shell::v1::CopyFilesFromMachineResponse>::Create(channel_.get(), rpcmethod_CopyFilesFromMachine_, context);
}

void ShellService::Stub::experimental_async::CopyFilesFromMachine(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::viam::service::shell::v1::CopyFilesFromMachineRequest,::viam::service::shell::v1::CopyFilesFromMachineResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::viam::service::shell::v1::CopyFilesFromMachineRequest,::viam::service::shell::v1::CopyFilesFromMachineResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_CopyFilesFromMachine_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::viam::service::shell::v1::CopyFilesFromMachineRequest, ::viam::service::shell::v1::CopyFilesFromMachineResponse>* ShellService::Stub::AsyncCopyFilesFromMachineRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::viam::service::shell::v1::CopyFilesFromMachineRequest, ::viam::service::shell::v1::CopyFilesFromMachineResponse>::Create(channel_.get(), cq, rpcmethod_CopyFilesFromMachine_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::viam::service::shell::v1::CopyFilesFromMachineRequest, ::viam::service::shell::v1::CopyFilesFromMachineResponse>* ShellService::Stub::PrepareAsyncCopyFilesFromMachineRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::viam::service::shell::v1::CopyFilesFromMachineRequest, ::viam::service::shell::v1::CopyFilesFromMachineResponse>::Create(channel_.get(), cq, rpcmethod_CopyFilesFromMachine_, context, false, nullptr);
}

::grpc::Status ShellService::Stub::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::viam::common::v1::DoCommandResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_DoCommand_, context, request, response);
}

void ShellService::Stub::experimental_async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, std::move(f));
}

void ShellService::Stub::experimental_async::DoCommand(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, std::move(f));
}

void ShellService::Stub::experimental_async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, reactor);
}

void ShellService::Stub::experimental_async::DoCommand(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* ShellService::Stub::AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::viam::common::v1::DoCommandResponse>::Create(channel_.get(), cq, rpcmethod_DoCommand_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* ShellService::Stub::PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::viam::common::v1::DoCommandResponse>::Create(channel_.get(), cq, rpcmethod_DoCommand_, context, request, false);
}

ShellService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ShellService_method_names[0],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< ShellService::Service, ::viam::service::shell::v1::ShellRequest, ::viam::service::shell::v1::ShellResponse>(
          [](ShellService::Service* service,
             ::grpc_impl::ServerContext* ctx,
             ::grpc_impl::ServerReaderWriter<::viam::service::shell::v1::ShellResponse,
             ::viam::service::shell::v1::ShellRequest>* stream) {
               return service->Shell(ctx, stream);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ShellService_method_names[1],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< ShellService::Service, ::viam::service::shell::v1::CopyFilesToMachineRequest, ::viam::service::shell::v1::CopyFilesToMachineResponse>(
          [](ShellService::Service* service,
             ::grpc_impl::ServerContext* ctx,
             ::grpc_impl::ServerReaderWriter<::viam::service::shell::v1::CopyFilesToMachineResponse,
             ::viam::service::shell::v1::CopyFilesToMachineRequest>* stream) {
               return service->CopyFilesToMachine(ctx, stream);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ShellService_method_names[2],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< ShellService::Service, ::viam::service::shell::v1::CopyFilesFromMachineRequest, ::viam::service::shell::v1::CopyFilesFromMachineResponse>(
          [](ShellService::Service* service,
             ::grpc_impl::ServerContext* ctx,
             ::grpc_impl::ServerReaderWriter<::viam::service::shell::v1::CopyFilesFromMachineResponse,
             ::viam::service::shell::v1::CopyFilesFromMachineRequest>* stream) {
               return service->CopyFilesFromMachine(ctx, stream);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ShellService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ShellService::Service, ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse>(
          [](ShellService::Service* service,
             ::grpc_impl::ServerContext* ctx,
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

::grpc::Status ShellService::Service::CopyFilesToMachine(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::viam::service::shell::v1::CopyFilesToMachineResponse, ::viam::service::shell::v1::CopyFilesToMachineRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ShellService::Service::CopyFilesFromMachine(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::viam::service::shell::v1::CopyFilesFromMachineResponse, ::viam::service::shell::v1::CopyFilesFromMachineRequest>* stream) {
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

