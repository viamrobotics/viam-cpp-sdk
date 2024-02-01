// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: app/packages/v1/packages.proto

#include "app/packages/v1/packages.pb.h"
#include "app/packages/v1/packages.grpc.pb.h"

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
namespace app {
namespace packages {
namespace v1 {

static const char* PackageService_method_names[] = {
  "/viam.app.packages.v1.PackageService/CreatePackage",
  "/viam.app.packages.v1.PackageService/DeletePackage",
  "/viam.app.packages.v1.PackageService/GetPackage",
  "/viam.app.packages.v1.PackageService/ListPackages",
};

std::unique_ptr< PackageService::Stub> PackageService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< PackageService::Stub> stub(new PackageService::Stub(channel, options));
  return stub;
}

PackageService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_CreatePackage_(PackageService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::CLIENT_STREAMING, channel)
  , rpcmethod_DeletePackage_(PackageService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetPackage_(PackageService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ListPackages_(PackageService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::ClientWriter< ::viam::app::packages::v1::CreatePackageRequest>* PackageService::Stub::CreatePackageRaw(::grpc::ClientContext* context, ::viam::app::packages::v1::CreatePackageResponse* response) {
  return ::grpc::internal::ClientWriterFactory< ::viam::app::packages::v1::CreatePackageRequest>::Create(channel_.get(), rpcmethod_CreatePackage_, context, response);
}

void PackageService::Stub::async::CreatePackage(::grpc::ClientContext* context, ::viam::app::packages::v1::CreatePackageResponse* response, ::grpc::ClientWriteReactor< ::viam::app::packages::v1::CreatePackageRequest>* reactor) {
  ::grpc::internal::ClientCallbackWriterFactory< ::viam::app::packages::v1::CreatePackageRequest>::Create(stub_->channel_.get(), stub_->rpcmethod_CreatePackage_, context, response, reactor);
}

::grpc::ClientAsyncWriter< ::viam::app::packages::v1::CreatePackageRequest>* PackageService::Stub::AsyncCreatePackageRaw(::grpc::ClientContext* context, ::viam::app::packages::v1::CreatePackageResponse* response, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::viam::app::packages::v1::CreatePackageRequest>::Create(channel_.get(), cq, rpcmethod_CreatePackage_, context, response, true, tag);
}

::grpc::ClientAsyncWriter< ::viam::app::packages::v1::CreatePackageRequest>* PackageService::Stub::PrepareAsyncCreatePackageRaw(::grpc::ClientContext* context, ::viam::app::packages::v1::CreatePackageResponse* response, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::viam::app::packages::v1::CreatePackageRequest>::Create(channel_.get(), cq, rpcmethod_CreatePackage_, context, response, false, nullptr);
}

::grpc::Status PackageService::Stub::DeletePackage(::grpc::ClientContext* context, const ::viam::app::packages::v1::DeletePackageRequest& request, ::viam::app::packages::v1::DeletePackageResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::packages::v1::DeletePackageRequest, ::viam::app::packages::v1::DeletePackageResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DeletePackage_, context, request, response);
}

void PackageService::Stub::async::DeletePackage(::grpc::ClientContext* context, const ::viam::app::packages::v1::DeletePackageRequest* request, ::viam::app::packages::v1::DeletePackageResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::packages::v1::DeletePackageRequest, ::viam::app::packages::v1::DeletePackageResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DeletePackage_, context, request, response, std::move(f));
}

void PackageService::Stub::async::DeletePackage(::grpc::ClientContext* context, const ::viam::app::packages::v1::DeletePackageRequest* request, ::viam::app::packages::v1::DeletePackageResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DeletePackage_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::packages::v1::DeletePackageResponse>* PackageService::Stub::PrepareAsyncDeletePackageRaw(::grpc::ClientContext* context, const ::viam::app::packages::v1::DeletePackageRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::packages::v1::DeletePackageResponse, ::viam::app::packages::v1::DeletePackageRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DeletePackage_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::packages::v1::DeletePackageResponse>* PackageService::Stub::AsyncDeletePackageRaw(::grpc::ClientContext* context, const ::viam::app::packages::v1::DeletePackageRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDeletePackageRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status PackageService::Stub::GetPackage(::grpc::ClientContext* context, const ::viam::app::packages::v1::GetPackageRequest& request, ::viam::app::packages::v1::GetPackageResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::packages::v1::GetPackageRequest, ::viam::app::packages::v1::GetPackageResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetPackage_, context, request, response);
}

void PackageService::Stub::async::GetPackage(::grpc::ClientContext* context, const ::viam::app::packages::v1::GetPackageRequest* request, ::viam::app::packages::v1::GetPackageResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::packages::v1::GetPackageRequest, ::viam::app::packages::v1::GetPackageResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetPackage_, context, request, response, std::move(f));
}

void PackageService::Stub::async::GetPackage(::grpc::ClientContext* context, const ::viam::app::packages::v1::GetPackageRequest* request, ::viam::app::packages::v1::GetPackageResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetPackage_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::packages::v1::GetPackageResponse>* PackageService::Stub::PrepareAsyncGetPackageRaw(::grpc::ClientContext* context, const ::viam::app::packages::v1::GetPackageRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::packages::v1::GetPackageResponse, ::viam::app::packages::v1::GetPackageRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetPackage_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::packages::v1::GetPackageResponse>* PackageService::Stub::AsyncGetPackageRaw(::grpc::ClientContext* context, const ::viam::app::packages::v1::GetPackageRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetPackageRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status PackageService::Stub::ListPackages(::grpc::ClientContext* context, const ::viam::app::packages::v1::ListPackagesRequest& request, ::viam::app::packages::v1::ListPackagesResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::packages::v1::ListPackagesRequest, ::viam::app::packages::v1::ListPackagesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_ListPackages_, context, request, response);
}

void PackageService::Stub::async::ListPackages(::grpc::ClientContext* context, const ::viam::app::packages::v1::ListPackagesRequest* request, ::viam::app::packages::v1::ListPackagesResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::packages::v1::ListPackagesRequest, ::viam::app::packages::v1::ListPackagesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ListPackages_, context, request, response, std::move(f));
}

void PackageService::Stub::async::ListPackages(::grpc::ClientContext* context, const ::viam::app::packages::v1::ListPackagesRequest* request, ::viam::app::packages::v1::ListPackagesResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ListPackages_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::packages::v1::ListPackagesResponse>* PackageService::Stub::PrepareAsyncListPackagesRaw(::grpc::ClientContext* context, const ::viam::app::packages::v1::ListPackagesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::packages::v1::ListPackagesResponse, ::viam::app::packages::v1::ListPackagesRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_ListPackages_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::packages::v1::ListPackagesResponse>* PackageService::Stub::AsyncListPackagesRaw(::grpc::ClientContext* context, const ::viam::app::packages::v1::ListPackagesRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncListPackagesRaw(context, request, cq);
  result->StartCall();
  return result;
}

PackageService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PackageService_method_names[0],
      ::grpc::internal::RpcMethod::CLIENT_STREAMING,
      new ::grpc::internal::ClientStreamingHandler< PackageService::Service, ::viam::app::packages::v1::CreatePackageRequest, ::viam::app::packages::v1::CreatePackageResponse>(
          [](PackageService::Service* service,
             ::grpc::ServerContext* ctx,
             ::grpc::ServerReader<::viam::app::packages::v1::CreatePackageRequest>* reader,
             ::viam::app::packages::v1::CreatePackageResponse* resp) {
               return service->CreatePackage(ctx, reader, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PackageService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PackageService::Service, ::viam::app::packages::v1::DeletePackageRequest, ::viam::app::packages::v1::DeletePackageResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PackageService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::packages::v1::DeletePackageRequest* req,
             ::viam::app::packages::v1::DeletePackageResponse* resp) {
               return service->DeletePackage(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PackageService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PackageService::Service, ::viam::app::packages::v1::GetPackageRequest, ::viam::app::packages::v1::GetPackageResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PackageService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::packages::v1::GetPackageRequest* req,
             ::viam::app::packages::v1::GetPackageResponse* resp) {
               return service->GetPackage(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PackageService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PackageService::Service, ::viam::app::packages::v1::ListPackagesRequest, ::viam::app::packages::v1::ListPackagesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PackageService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::packages::v1::ListPackagesRequest* req,
             ::viam::app::packages::v1::ListPackagesResponse* resp) {
               return service->ListPackages(ctx, req, resp);
             }, this)));
}

PackageService::Service::~Service() {
}

::grpc::Status PackageService::Service::CreatePackage(::grpc::ServerContext* context, ::grpc::ServerReader< ::viam::app::packages::v1::CreatePackageRequest>* reader, ::viam::app::packages::v1::CreatePackageResponse* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PackageService::Service::DeletePackage(::grpc::ServerContext* context, const ::viam::app::packages::v1::DeletePackageRequest* request, ::viam::app::packages::v1::DeletePackageResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PackageService::Service::GetPackage(::grpc::ServerContext* context, const ::viam::app::packages::v1::GetPackageRequest* request, ::viam::app::packages::v1::GetPackageResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PackageService::Service::ListPackages(::grpc::ServerContext* context, const ::viam::app::packages::v1::ListPackagesRequest* request, ::viam::app::packages::v1::ListPackagesResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace app
}  // namespace packages
}  // namespace v1

