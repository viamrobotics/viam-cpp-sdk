// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: component/generic/v1/generic.proto

#include "component/generic/v1/generic.pb.h"
#include "component/generic/v1/generic.grpc.pb.h"

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
namespace component {
namespace generic {
namespace v1 {

static const char* GenericService_method_names[] = {
  "/viam.component.generic.v1.GenericService/DoCommand",
};

std::unique_ptr< GenericService::Stub> GenericService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< GenericService::Stub> stub(new GenericService::Stub(channel, options));
  return stub;
}

GenericService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_DoCommand_(GenericService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status GenericService::Stub::DoCommand(::grpc::ClientContext* context, const ::viam::component::generic::v1::DoCommandRequest& request, ::viam::component::generic::v1::DoCommandResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::generic::v1::DoCommandRequest, ::viam::component::generic::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DoCommand_, context, request, response);
}

void GenericService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::component::generic::v1::DoCommandRequest* request, ::viam::component::generic::v1::DoCommandResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::generic::v1::DoCommandRequest, ::viam::component::generic::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, std::move(f));
}

void GenericService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::component::generic::v1::DoCommandRequest* request, ::viam::component::generic::v1::DoCommandResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::generic::v1::DoCommandResponse>* GenericService::Stub::PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::component::generic::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::generic::v1::DoCommandResponse, ::viam::component::generic::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::generic::v1::DoCommandResponse>* GenericService::Stub::AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::component::generic::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoCommandRaw(context, request, cq);
  result->StartCall();
  return result;
}

GenericService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      GenericService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< GenericService::Service, ::viam::component::generic::v1::DoCommandRequest, ::viam::component::generic::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](GenericService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::generic::v1::DoCommandRequest* req,
             ::viam::component::generic::v1::DoCommandResponse* resp) {
               return service->DoCommand(ctx, req, resp);
             }, this)));
}

GenericService::Service::~Service() {
}

::grpc::Status GenericService::Service::DoCommand(::grpc::ServerContext* context, const ::viam::component::generic::v1::DoCommandRequest* request, ::viam::component::generic::v1::DoCommandResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace component
}  // namespace generic
}  // namespace v1

