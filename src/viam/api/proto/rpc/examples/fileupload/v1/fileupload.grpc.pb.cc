// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: proto/rpc/examples/fileupload/v1/fileupload.proto

#include "proto/rpc/examples/fileupload/v1/fileupload.pb.h"
#include "proto/rpc/examples/fileupload/v1/fileupload.grpc.pb.h"

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
namespace proto {
namespace rpc {
namespace examples {
namespace fileupload {
namespace v1 {

static const char* FileUploadService_method_names[] = {
  "/proto.rpc.examples.fileupload.v1.FileUploadService/UploadFile",
};

std::unique_ptr< FileUploadService::Stub> FileUploadService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< FileUploadService::Stub> stub(new FileUploadService::Stub(channel, options));
  return stub;
}

FileUploadService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_UploadFile_(FileUploadService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  {}

::grpc::ClientReaderWriter< ::proto::rpc::examples::fileupload::v1::UploadFileRequest, ::proto::rpc::examples::fileupload::v1::UploadFileResponse>* FileUploadService::Stub::UploadFileRaw(::grpc::ClientContext* context) {
  return ::grpc::internal::ClientReaderWriterFactory< ::proto::rpc::examples::fileupload::v1::UploadFileRequest, ::proto::rpc::examples::fileupload::v1::UploadFileResponse>::Create(channel_.get(), rpcmethod_UploadFile_, context);
}

void FileUploadService::Stub::async::UploadFile(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::proto::rpc::examples::fileupload::v1::UploadFileRequest,::proto::rpc::examples::fileupload::v1::UploadFileResponse>* reactor) {
  ::grpc::internal::ClientCallbackReaderWriterFactory< ::proto::rpc::examples::fileupload::v1::UploadFileRequest,::proto::rpc::examples::fileupload::v1::UploadFileResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_UploadFile_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::proto::rpc::examples::fileupload::v1::UploadFileRequest, ::proto::rpc::examples::fileupload::v1::UploadFileResponse>* FileUploadService::Stub::AsyncUploadFileRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::proto::rpc::examples::fileupload::v1::UploadFileRequest, ::proto::rpc::examples::fileupload::v1::UploadFileResponse>::Create(channel_.get(), cq, rpcmethod_UploadFile_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::proto::rpc::examples::fileupload::v1::UploadFileRequest, ::proto::rpc::examples::fileupload::v1::UploadFileResponse>* FileUploadService::Stub::PrepareAsyncUploadFileRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::proto::rpc::examples::fileupload::v1::UploadFileRequest, ::proto::rpc::examples::fileupload::v1::UploadFileResponse>::Create(channel_.get(), cq, rpcmethod_UploadFile_, context, false, nullptr);
}

FileUploadService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      FileUploadService_method_names[0],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< FileUploadService::Service, ::proto::rpc::examples::fileupload::v1::UploadFileRequest, ::proto::rpc::examples::fileupload::v1::UploadFileResponse>(
          [](FileUploadService::Service* service,
             ::grpc::ServerContext* ctx,
             ::grpc::ServerReaderWriter<::proto::rpc::examples::fileupload::v1::UploadFileResponse,
             ::proto::rpc::examples::fileupload::v1::UploadFileRequest>* stream) {
               return service->UploadFile(ctx, stream);
             }, this)));
}

FileUploadService::Service::~Service() {
}

::grpc::Status FileUploadService::Service::UploadFile(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::proto::rpc::examples::fileupload::v1::UploadFileResponse, ::proto::rpc::examples::fileupload::v1::UploadFileRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace proto
}  // namespace rpc
}  // namespace examples
}  // namespace fileupload
}  // namespace v1

