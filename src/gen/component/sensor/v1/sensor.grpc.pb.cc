// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: component/sensor/v1/sensor.proto

#include "component/sensor/v1/sensor.pb.h"
#include "component/sensor/v1/sensor.grpc.pb.h"

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
namespace sensor {
namespace v1 {

static const char* SensorService_method_names[] = {
  "/viam.component.sensor.v1.SensorService/GetReadings",
};

std::unique_ptr< SensorService::Stub> SensorService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< SensorService::Stub> stub(new SensorService::Stub(channel, options));
  return stub;
}

SensorService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetReadings_(SensorService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status SensorService::Stub::GetReadings(::grpc::ClientContext* context, const ::viam::component::sensor::v1::GetReadingsRequest& request, ::viam::component::sensor::v1::GetReadingsResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::sensor::v1::GetReadingsRequest, ::viam::component::sensor::v1::GetReadingsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetReadings_, context, request, response);
}

void SensorService::Stub::async::GetReadings(::grpc::ClientContext* context, const ::viam::component::sensor::v1::GetReadingsRequest* request, ::viam::component::sensor::v1::GetReadingsResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::sensor::v1::GetReadingsRequest, ::viam::component::sensor::v1::GetReadingsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetReadings_, context, request, response, std::move(f));
}

void SensorService::Stub::async::GetReadings(::grpc::ClientContext* context, const ::viam::component::sensor::v1::GetReadingsRequest* request, ::viam::component::sensor::v1::GetReadingsResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetReadings_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::sensor::v1::GetReadingsResponse>* SensorService::Stub::PrepareAsyncGetReadingsRaw(::grpc::ClientContext* context, const ::viam::component::sensor::v1::GetReadingsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::sensor::v1::GetReadingsResponse, ::viam::component::sensor::v1::GetReadingsRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetReadings_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::sensor::v1::GetReadingsResponse>* SensorService::Stub::AsyncGetReadingsRaw(::grpc::ClientContext* context, const ::viam::component::sensor::v1::GetReadingsRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetReadingsRaw(context, request, cq);
  result->StartCall();
  return result;
}

SensorService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      SensorService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< SensorService::Service, ::viam::component::sensor::v1::GetReadingsRequest, ::viam::component::sensor::v1::GetReadingsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](SensorService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::sensor::v1::GetReadingsRequest* req,
             ::viam::component::sensor::v1::GetReadingsResponse* resp) {
               return service->GetReadings(ctx, req, resp);
             }, this)));
}

SensorService::Service::~Service() {
}

::grpc::Status SensorService::Service::GetReadings(::grpc::ServerContext* context, const ::viam::component::sensor::v1::GetReadingsRequest* request, ::viam::component::sensor::v1::GetReadingsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace component
}  // namespace sensor
}  // namespace v1

