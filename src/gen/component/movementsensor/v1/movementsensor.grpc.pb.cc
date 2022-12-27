// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: component/movementsensor/v1/movementsensor.proto

#include "component/movementsensor/v1/movementsensor.pb.h"
#include "component/movementsensor/v1/movementsensor.grpc.pb.h"

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
namespace movementsensor {
namespace v1 {

static const char* MovementSensorService_method_names[] = {
  "/viam.component.movementsensor.v1.MovementSensorService/GetLinearVelocity",
  "/viam.component.movementsensor.v1.MovementSensorService/GetAngularVelocity",
  "/viam.component.movementsensor.v1.MovementSensorService/GetCompassHeading",
  "/viam.component.movementsensor.v1.MovementSensorService/GetOrientation",
  "/viam.component.movementsensor.v1.MovementSensorService/GetPosition",
  "/viam.component.movementsensor.v1.MovementSensorService/GetProperties",
  "/viam.component.movementsensor.v1.MovementSensorService/GetAccuracy",
};

std::unique_ptr< MovementSensorService::Stub> MovementSensorService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< MovementSensorService::Stub> stub(new MovementSensorService::Stub(channel, options));
  return stub;
}

MovementSensorService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetLinearVelocity_(MovementSensorService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetAngularVelocity_(MovementSensorService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetCompassHeading_(MovementSensorService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetOrientation_(MovementSensorService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetPosition_(MovementSensorService_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetProperties_(MovementSensorService_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetAccuracy_(MovementSensorService_method_names[6], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status MovementSensorService::Stub::GetLinearVelocity(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetLinearVelocityRequest& request, ::viam::component::movementsensor::v1::GetLinearVelocityResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::movementsensor::v1::GetLinearVelocityRequest, ::viam::component::movementsensor::v1::GetLinearVelocityResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetLinearVelocity_, context, request, response);
}

void MovementSensorService::Stub::async::GetLinearVelocity(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetLinearVelocityRequest* request, ::viam::component::movementsensor::v1::GetLinearVelocityResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::movementsensor::v1::GetLinearVelocityRequest, ::viam::component::movementsensor::v1::GetLinearVelocityResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetLinearVelocity_, context, request, response, std::move(f));
}

void MovementSensorService::Stub::async::GetLinearVelocity(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetLinearVelocityRequest* request, ::viam::component::movementsensor::v1::GetLinearVelocityResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetLinearVelocity_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetLinearVelocityResponse>* MovementSensorService::Stub::PrepareAsyncGetLinearVelocityRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetLinearVelocityRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::movementsensor::v1::GetLinearVelocityResponse, ::viam::component::movementsensor::v1::GetLinearVelocityRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetLinearVelocity_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetLinearVelocityResponse>* MovementSensorService::Stub::AsyncGetLinearVelocityRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetLinearVelocityRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetLinearVelocityRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MovementSensorService::Stub::GetAngularVelocity(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetAngularVelocityRequest& request, ::viam::component::movementsensor::v1::GetAngularVelocityResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::movementsensor::v1::GetAngularVelocityRequest, ::viam::component::movementsensor::v1::GetAngularVelocityResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetAngularVelocity_, context, request, response);
}

void MovementSensorService::Stub::async::GetAngularVelocity(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetAngularVelocityRequest* request, ::viam::component::movementsensor::v1::GetAngularVelocityResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::movementsensor::v1::GetAngularVelocityRequest, ::viam::component::movementsensor::v1::GetAngularVelocityResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetAngularVelocity_, context, request, response, std::move(f));
}

void MovementSensorService::Stub::async::GetAngularVelocity(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetAngularVelocityRequest* request, ::viam::component::movementsensor::v1::GetAngularVelocityResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetAngularVelocity_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetAngularVelocityResponse>* MovementSensorService::Stub::PrepareAsyncGetAngularVelocityRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetAngularVelocityRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::movementsensor::v1::GetAngularVelocityResponse, ::viam::component::movementsensor::v1::GetAngularVelocityRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetAngularVelocity_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetAngularVelocityResponse>* MovementSensorService::Stub::AsyncGetAngularVelocityRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetAngularVelocityRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetAngularVelocityRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MovementSensorService::Stub::GetCompassHeading(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetCompassHeadingRequest& request, ::viam::component::movementsensor::v1::GetCompassHeadingResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::movementsensor::v1::GetCompassHeadingRequest, ::viam::component::movementsensor::v1::GetCompassHeadingResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetCompassHeading_, context, request, response);
}

void MovementSensorService::Stub::async::GetCompassHeading(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetCompassHeadingRequest* request, ::viam::component::movementsensor::v1::GetCompassHeadingResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::movementsensor::v1::GetCompassHeadingRequest, ::viam::component::movementsensor::v1::GetCompassHeadingResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetCompassHeading_, context, request, response, std::move(f));
}

void MovementSensorService::Stub::async::GetCompassHeading(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetCompassHeadingRequest* request, ::viam::component::movementsensor::v1::GetCompassHeadingResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetCompassHeading_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetCompassHeadingResponse>* MovementSensorService::Stub::PrepareAsyncGetCompassHeadingRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetCompassHeadingRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::movementsensor::v1::GetCompassHeadingResponse, ::viam::component::movementsensor::v1::GetCompassHeadingRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetCompassHeading_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetCompassHeadingResponse>* MovementSensorService::Stub::AsyncGetCompassHeadingRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetCompassHeadingRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetCompassHeadingRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MovementSensorService::Stub::GetOrientation(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetOrientationRequest& request, ::viam::component::movementsensor::v1::GetOrientationResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::movementsensor::v1::GetOrientationRequest, ::viam::component::movementsensor::v1::GetOrientationResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetOrientation_, context, request, response);
}

void MovementSensorService::Stub::async::GetOrientation(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetOrientationRequest* request, ::viam::component::movementsensor::v1::GetOrientationResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::movementsensor::v1::GetOrientationRequest, ::viam::component::movementsensor::v1::GetOrientationResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetOrientation_, context, request, response, std::move(f));
}

void MovementSensorService::Stub::async::GetOrientation(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetOrientationRequest* request, ::viam::component::movementsensor::v1::GetOrientationResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetOrientation_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetOrientationResponse>* MovementSensorService::Stub::PrepareAsyncGetOrientationRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetOrientationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::movementsensor::v1::GetOrientationResponse, ::viam::component::movementsensor::v1::GetOrientationRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetOrientation_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetOrientationResponse>* MovementSensorService::Stub::AsyncGetOrientationRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetOrientationRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetOrientationRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MovementSensorService::Stub::GetPosition(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetPositionRequest& request, ::viam::component::movementsensor::v1::GetPositionResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::movementsensor::v1::GetPositionRequest, ::viam::component::movementsensor::v1::GetPositionResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetPosition_, context, request, response);
}

void MovementSensorService::Stub::async::GetPosition(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetPositionRequest* request, ::viam::component::movementsensor::v1::GetPositionResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::movementsensor::v1::GetPositionRequest, ::viam::component::movementsensor::v1::GetPositionResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetPosition_, context, request, response, std::move(f));
}

void MovementSensorService::Stub::async::GetPosition(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetPositionRequest* request, ::viam::component::movementsensor::v1::GetPositionResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetPosition_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetPositionResponse>* MovementSensorService::Stub::PrepareAsyncGetPositionRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetPositionRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::movementsensor::v1::GetPositionResponse, ::viam::component::movementsensor::v1::GetPositionRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetPosition_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetPositionResponse>* MovementSensorService::Stub::AsyncGetPositionRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetPositionRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetPositionRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MovementSensorService::Stub::GetProperties(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetPropertiesRequest& request, ::viam::component::movementsensor::v1::GetPropertiesResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::movementsensor::v1::GetPropertiesRequest, ::viam::component::movementsensor::v1::GetPropertiesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetProperties_, context, request, response);
}

void MovementSensorService::Stub::async::GetProperties(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetPropertiesRequest* request, ::viam::component::movementsensor::v1::GetPropertiesResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::movementsensor::v1::GetPropertiesRequest, ::viam::component::movementsensor::v1::GetPropertiesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetProperties_, context, request, response, std::move(f));
}

void MovementSensorService::Stub::async::GetProperties(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetPropertiesRequest* request, ::viam::component::movementsensor::v1::GetPropertiesResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetProperties_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetPropertiesResponse>* MovementSensorService::Stub::PrepareAsyncGetPropertiesRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetPropertiesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::movementsensor::v1::GetPropertiesResponse, ::viam::component::movementsensor::v1::GetPropertiesRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetProperties_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetPropertiesResponse>* MovementSensorService::Stub::AsyncGetPropertiesRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetPropertiesRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetPropertiesRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MovementSensorService::Stub::GetAccuracy(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetAccuracyRequest& request, ::viam::component::movementsensor::v1::GetAccuracyResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::component::movementsensor::v1::GetAccuracyRequest, ::viam::component::movementsensor::v1::GetAccuracyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetAccuracy_, context, request, response);
}

void MovementSensorService::Stub::async::GetAccuracy(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetAccuracyRequest* request, ::viam::component::movementsensor::v1::GetAccuracyResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::component::movementsensor::v1::GetAccuracyRequest, ::viam::component::movementsensor::v1::GetAccuracyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetAccuracy_, context, request, response, std::move(f));
}

void MovementSensorService::Stub::async::GetAccuracy(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetAccuracyRequest* request, ::viam::component::movementsensor::v1::GetAccuracyResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetAccuracy_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetAccuracyResponse>* MovementSensorService::Stub::PrepareAsyncGetAccuracyRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetAccuracyRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::movementsensor::v1::GetAccuracyResponse, ::viam::component::movementsensor::v1::GetAccuracyRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetAccuracy_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::component::movementsensor::v1::GetAccuracyResponse>* MovementSensorService::Stub::AsyncGetAccuracyRaw(::grpc::ClientContext* context, const ::viam::component::movementsensor::v1::GetAccuracyRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetAccuracyRaw(context, request, cq);
  result->StartCall();
  return result;
}

MovementSensorService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MovementSensorService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MovementSensorService::Service, ::viam::component::movementsensor::v1::GetLinearVelocityRequest, ::viam::component::movementsensor::v1::GetLinearVelocityResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MovementSensorService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::movementsensor::v1::GetLinearVelocityRequest* req,
             ::viam::component::movementsensor::v1::GetLinearVelocityResponse* resp) {
               return service->GetLinearVelocity(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MovementSensorService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MovementSensorService::Service, ::viam::component::movementsensor::v1::GetAngularVelocityRequest, ::viam::component::movementsensor::v1::GetAngularVelocityResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MovementSensorService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::movementsensor::v1::GetAngularVelocityRequest* req,
             ::viam::component::movementsensor::v1::GetAngularVelocityResponse* resp) {
               return service->GetAngularVelocity(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MovementSensorService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MovementSensorService::Service, ::viam::component::movementsensor::v1::GetCompassHeadingRequest, ::viam::component::movementsensor::v1::GetCompassHeadingResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MovementSensorService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::movementsensor::v1::GetCompassHeadingRequest* req,
             ::viam::component::movementsensor::v1::GetCompassHeadingResponse* resp) {
               return service->GetCompassHeading(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MovementSensorService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MovementSensorService::Service, ::viam::component::movementsensor::v1::GetOrientationRequest, ::viam::component::movementsensor::v1::GetOrientationResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MovementSensorService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::movementsensor::v1::GetOrientationRequest* req,
             ::viam::component::movementsensor::v1::GetOrientationResponse* resp) {
               return service->GetOrientation(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MovementSensorService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MovementSensorService::Service, ::viam::component::movementsensor::v1::GetPositionRequest, ::viam::component::movementsensor::v1::GetPositionResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MovementSensorService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::movementsensor::v1::GetPositionRequest* req,
             ::viam::component::movementsensor::v1::GetPositionResponse* resp) {
               return service->GetPosition(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MovementSensorService_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MovementSensorService::Service, ::viam::component::movementsensor::v1::GetPropertiesRequest, ::viam::component::movementsensor::v1::GetPropertiesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MovementSensorService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::movementsensor::v1::GetPropertiesRequest* req,
             ::viam::component::movementsensor::v1::GetPropertiesResponse* resp) {
               return service->GetProperties(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MovementSensorService_method_names[6],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MovementSensorService::Service, ::viam::component::movementsensor::v1::GetAccuracyRequest, ::viam::component::movementsensor::v1::GetAccuracyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MovementSensorService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::component::movementsensor::v1::GetAccuracyRequest* req,
             ::viam::component::movementsensor::v1::GetAccuracyResponse* resp) {
               return service->GetAccuracy(ctx, req, resp);
             }, this)));
}

MovementSensorService::Service::~Service() {
}

::grpc::Status MovementSensorService::Service::GetLinearVelocity(::grpc::ServerContext* context, const ::viam::component::movementsensor::v1::GetLinearVelocityRequest* request, ::viam::component::movementsensor::v1::GetLinearVelocityResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MovementSensorService::Service::GetAngularVelocity(::grpc::ServerContext* context, const ::viam::component::movementsensor::v1::GetAngularVelocityRequest* request, ::viam::component::movementsensor::v1::GetAngularVelocityResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MovementSensorService::Service::GetCompassHeading(::grpc::ServerContext* context, const ::viam::component::movementsensor::v1::GetCompassHeadingRequest* request, ::viam::component::movementsensor::v1::GetCompassHeadingResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MovementSensorService::Service::GetOrientation(::grpc::ServerContext* context, const ::viam::component::movementsensor::v1::GetOrientationRequest* request, ::viam::component::movementsensor::v1::GetOrientationResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MovementSensorService::Service::GetPosition(::grpc::ServerContext* context, const ::viam::component::movementsensor::v1::GetPositionRequest* request, ::viam::component::movementsensor::v1::GetPositionResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MovementSensorService::Service::GetProperties(::grpc::ServerContext* context, const ::viam::component::movementsensor::v1::GetPropertiesRequest* request, ::viam::component::movementsensor::v1::GetPropertiesResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MovementSensorService::Service::GetAccuracy(::grpc::ServerContext* context, const ::viam::component::movementsensor::v1::GetAccuracyRequest* request, ::viam::component::movementsensor::v1::GetAccuracyResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace component
}  // namespace movementsensor
}  // namespace v1

