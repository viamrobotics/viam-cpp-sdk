// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: service/navigation/v1/navigation.proto

#include "service/navigation/v1/navigation.pb.h"
#include "service/navigation/v1/navigation.grpc.pb.h"

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
namespace navigation {
namespace v1 {

static const char* NavigationService_method_names[] = {
  "/viam.service.navigation.v1.NavigationService/GetMode",
  "/viam.service.navigation.v1.NavigationService/SetMode",
  "/viam.service.navigation.v1.NavigationService/GetLocation",
  "/viam.service.navigation.v1.NavigationService/GetWaypoints",
  "/viam.service.navigation.v1.NavigationService/AddWaypoint",
  "/viam.service.navigation.v1.NavigationService/RemoveWaypoint",
  "/viam.service.navigation.v1.NavigationService/GetObstacles",
  "/viam.service.navigation.v1.NavigationService/GetPaths",
  "/viam.service.navigation.v1.NavigationService/GetProperties",
  "/viam.service.navigation.v1.NavigationService/DoCommand",
};

std::unique_ptr< NavigationService::Stub> NavigationService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< NavigationService::Stub> stub(new NavigationService::Stub(channel, options));
  return stub;
}

NavigationService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetMode_(NavigationService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_SetMode_(NavigationService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetLocation_(NavigationService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetWaypoints_(NavigationService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_AddWaypoint_(NavigationService_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_RemoveWaypoint_(NavigationService_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetObstacles_(NavigationService_method_names[6], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetPaths_(NavigationService_method_names[7], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetProperties_(NavigationService_method_names[8], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DoCommand_(NavigationService_method_names[9], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status NavigationService::Stub::GetMode(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetModeRequest& request, ::viam::service::navigation::v1::GetModeResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::navigation::v1::GetModeRequest, ::viam::service::navigation::v1::GetModeResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetMode_, context, request, response);
}

void NavigationService::Stub::async::GetMode(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetModeRequest* request, ::viam::service::navigation::v1::GetModeResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::navigation::v1::GetModeRequest, ::viam::service::navigation::v1::GetModeResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetMode_, context, request, response, std::move(f));
}

void NavigationService::Stub::async::GetMode(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetModeRequest* request, ::viam::service::navigation::v1::GetModeResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetMode_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetModeResponse>* NavigationService::Stub::PrepareAsyncGetModeRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetModeRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::navigation::v1::GetModeResponse, ::viam::service::navigation::v1::GetModeRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetMode_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetModeResponse>* NavigationService::Stub::AsyncGetModeRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetModeRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetModeRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status NavigationService::Stub::SetMode(::grpc::ClientContext* context, const ::viam::service::navigation::v1::SetModeRequest& request, ::viam::service::navigation::v1::SetModeResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::navigation::v1::SetModeRequest, ::viam::service::navigation::v1::SetModeResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SetMode_, context, request, response);
}

void NavigationService::Stub::async::SetMode(::grpc::ClientContext* context, const ::viam::service::navigation::v1::SetModeRequest* request, ::viam::service::navigation::v1::SetModeResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::navigation::v1::SetModeRequest, ::viam::service::navigation::v1::SetModeResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SetMode_, context, request, response, std::move(f));
}

void NavigationService::Stub::async::SetMode(::grpc::ClientContext* context, const ::viam::service::navigation::v1::SetModeRequest* request, ::viam::service::navigation::v1::SetModeResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SetMode_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::SetModeResponse>* NavigationService::Stub::PrepareAsyncSetModeRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::SetModeRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::navigation::v1::SetModeResponse, ::viam::service::navigation::v1::SetModeRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SetMode_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::SetModeResponse>* NavigationService::Stub::AsyncSetModeRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::SetModeRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSetModeRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status NavigationService::Stub::GetLocation(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetLocationRequest& request, ::viam::service::navigation::v1::GetLocationResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::navigation::v1::GetLocationRequest, ::viam::service::navigation::v1::GetLocationResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetLocation_, context, request, response);
}

void NavigationService::Stub::async::GetLocation(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetLocationRequest* request, ::viam::service::navigation::v1::GetLocationResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::navigation::v1::GetLocationRequest, ::viam::service::navigation::v1::GetLocationResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetLocation_, context, request, response, std::move(f));
}

void NavigationService::Stub::async::GetLocation(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetLocationRequest* request, ::viam::service::navigation::v1::GetLocationResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetLocation_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetLocationResponse>* NavigationService::Stub::PrepareAsyncGetLocationRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetLocationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::navigation::v1::GetLocationResponse, ::viam::service::navigation::v1::GetLocationRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetLocation_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetLocationResponse>* NavigationService::Stub::AsyncGetLocationRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetLocationRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetLocationRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status NavigationService::Stub::GetWaypoints(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetWaypointsRequest& request, ::viam::service::navigation::v1::GetWaypointsResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::navigation::v1::GetWaypointsRequest, ::viam::service::navigation::v1::GetWaypointsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetWaypoints_, context, request, response);
}

void NavigationService::Stub::async::GetWaypoints(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetWaypointsRequest* request, ::viam::service::navigation::v1::GetWaypointsResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::navigation::v1::GetWaypointsRequest, ::viam::service::navigation::v1::GetWaypointsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetWaypoints_, context, request, response, std::move(f));
}

void NavigationService::Stub::async::GetWaypoints(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetWaypointsRequest* request, ::viam::service::navigation::v1::GetWaypointsResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetWaypoints_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetWaypointsResponse>* NavigationService::Stub::PrepareAsyncGetWaypointsRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetWaypointsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::navigation::v1::GetWaypointsResponse, ::viam::service::navigation::v1::GetWaypointsRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetWaypoints_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetWaypointsResponse>* NavigationService::Stub::AsyncGetWaypointsRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetWaypointsRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetWaypointsRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status NavigationService::Stub::AddWaypoint(::grpc::ClientContext* context, const ::viam::service::navigation::v1::AddWaypointRequest& request, ::viam::service::navigation::v1::AddWaypointResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::navigation::v1::AddWaypointRequest, ::viam::service::navigation::v1::AddWaypointResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_AddWaypoint_, context, request, response);
}

void NavigationService::Stub::async::AddWaypoint(::grpc::ClientContext* context, const ::viam::service::navigation::v1::AddWaypointRequest* request, ::viam::service::navigation::v1::AddWaypointResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::navigation::v1::AddWaypointRequest, ::viam::service::navigation::v1::AddWaypointResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_AddWaypoint_, context, request, response, std::move(f));
}

void NavigationService::Stub::async::AddWaypoint(::grpc::ClientContext* context, const ::viam::service::navigation::v1::AddWaypointRequest* request, ::viam::service::navigation::v1::AddWaypointResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_AddWaypoint_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::AddWaypointResponse>* NavigationService::Stub::PrepareAsyncAddWaypointRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::AddWaypointRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::navigation::v1::AddWaypointResponse, ::viam::service::navigation::v1::AddWaypointRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_AddWaypoint_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::AddWaypointResponse>* NavigationService::Stub::AsyncAddWaypointRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::AddWaypointRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncAddWaypointRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status NavigationService::Stub::RemoveWaypoint(::grpc::ClientContext* context, const ::viam::service::navigation::v1::RemoveWaypointRequest& request, ::viam::service::navigation::v1::RemoveWaypointResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::navigation::v1::RemoveWaypointRequest, ::viam::service::navigation::v1::RemoveWaypointResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_RemoveWaypoint_, context, request, response);
}

void NavigationService::Stub::async::RemoveWaypoint(::grpc::ClientContext* context, const ::viam::service::navigation::v1::RemoveWaypointRequest* request, ::viam::service::navigation::v1::RemoveWaypointResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::navigation::v1::RemoveWaypointRequest, ::viam::service::navigation::v1::RemoveWaypointResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RemoveWaypoint_, context, request, response, std::move(f));
}

void NavigationService::Stub::async::RemoveWaypoint(::grpc::ClientContext* context, const ::viam::service::navigation::v1::RemoveWaypointRequest* request, ::viam::service::navigation::v1::RemoveWaypointResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RemoveWaypoint_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::RemoveWaypointResponse>* NavigationService::Stub::PrepareAsyncRemoveWaypointRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::RemoveWaypointRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::navigation::v1::RemoveWaypointResponse, ::viam::service::navigation::v1::RemoveWaypointRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_RemoveWaypoint_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::RemoveWaypointResponse>* NavigationService::Stub::AsyncRemoveWaypointRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::RemoveWaypointRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncRemoveWaypointRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status NavigationService::Stub::GetObstacles(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetObstaclesRequest& request, ::viam::service::navigation::v1::GetObstaclesResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::navigation::v1::GetObstaclesRequest, ::viam::service::navigation::v1::GetObstaclesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetObstacles_, context, request, response);
}

void NavigationService::Stub::async::GetObstacles(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetObstaclesRequest* request, ::viam::service::navigation::v1::GetObstaclesResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::navigation::v1::GetObstaclesRequest, ::viam::service::navigation::v1::GetObstaclesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetObstacles_, context, request, response, std::move(f));
}

void NavigationService::Stub::async::GetObstacles(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetObstaclesRequest* request, ::viam::service::navigation::v1::GetObstaclesResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetObstacles_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetObstaclesResponse>* NavigationService::Stub::PrepareAsyncGetObstaclesRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetObstaclesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::navigation::v1::GetObstaclesResponse, ::viam::service::navigation::v1::GetObstaclesRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetObstacles_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetObstaclesResponse>* NavigationService::Stub::AsyncGetObstaclesRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetObstaclesRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetObstaclesRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status NavigationService::Stub::GetPaths(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetPathsRequest& request, ::viam::service::navigation::v1::GetPathsResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::navigation::v1::GetPathsRequest, ::viam::service::navigation::v1::GetPathsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetPaths_, context, request, response);
}

void NavigationService::Stub::async::GetPaths(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetPathsRequest* request, ::viam::service::navigation::v1::GetPathsResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::navigation::v1::GetPathsRequest, ::viam::service::navigation::v1::GetPathsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetPaths_, context, request, response, std::move(f));
}

void NavigationService::Stub::async::GetPaths(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetPathsRequest* request, ::viam::service::navigation::v1::GetPathsResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetPaths_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetPathsResponse>* NavigationService::Stub::PrepareAsyncGetPathsRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetPathsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::navigation::v1::GetPathsResponse, ::viam::service::navigation::v1::GetPathsRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetPaths_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetPathsResponse>* NavigationService::Stub::AsyncGetPathsRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetPathsRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetPathsRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status NavigationService::Stub::GetProperties(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetPropertiesRequest& request, ::viam::service::navigation::v1::GetPropertiesResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::service::navigation::v1::GetPropertiesRequest, ::viam::service::navigation::v1::GetPropertiesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetProperties_, context, request, response);
}

void NavigationService::Stub::async::GetProperties(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetPropertiesRequest* request, ::viam::service::navigation::v1::GetPropertiesResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::service::navigation::v1::GetPropertiesRequest, ::viam::service::navigation::v1::GetPropertiesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetProperties_, context, request, response, std::move(f));
}

void NavigationService::Stub::async::GetProperties(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetPropertiesRequest* request, ::viam::service::navigation::v1::GetPropertiesResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetProperties_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetPropertiesResponse>* NavigationService::Stub::PrepareAsyncGetPropertiesRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetPropertiesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::service::navigation::v1::GetPropertiesResponse, ::viam::service::navigation::v1::GetPropertiesRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetProperties_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::service::navigation::v1::GetPropertiesResponse>* NavigationService::Stub::AsyncGetPropertiesRaw(::grpc::ClientContext* context, const ::viam::service::navigation::v1::GetPropertiesRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetPropertiesRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status NavigationService::Stub::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::viam::common::v1::DoCommandResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DoCommand_, context, request, response);
}

void NavigationService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, std::move(f));
}

void NavigationService::Stub::async::DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DoCommand_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* NavigationService::Stub::PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse, ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* NavigationService::Stub::AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoCommandRaw(context, request, cq);
  result->StartCall();
  return result;
}

NavigationService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NavigationService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NavigationService::Service, ::viam::service::navigation::v1::GetModeRequest, ::viam::service::navigation::v1::GetModeResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](NavigationService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::navigation::v1::GetModeRequest* req,
             ::viam::service::navigation::v1::GetModeResponse* resp) {
               return service->GetMode(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NavigationService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NavigationService::Service, ::viam::service::navigation::v1::SetModeRequest, ::viam::service::navigation::v1::SetModeResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](NavigationService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::navigation::v1::SetModeRequest* req,
             ::viam::service::navigation::v1::SetModeResponse* resp) {
               return service->SetMode(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NavigationService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NavigationService::Service, ::viam::service::navigation::v1::GetLocationRequest, ::viam::service::navigation::v1::GetLocationResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](NavigationService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::navigation::v1::GetLocationRequest* req,
             ::viam::service::navigation::v1::GetLocationResponse* resp) {
               return service->GetLocation(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NavigationService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NavigationService::Service, ::viam::service::navigation::v1::GetWaypointsRequest, ::viam::service::navigation::v1::GetWaypointsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](NavigationService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::navigation::v1::GetWaypointsRequest* req,
             ::viam::service::navigation::v1::GetWaypointsResponse* resp) {
               return service->GetWaypoints(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NavigationService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NavigationService::Service, ::viam::service::navigation::v1::AddWaypointRequest, ::viam::service::navigation::v1::AddWaypointResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](NavigationService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::navigation::v1::AddWaypointRequest* req,
             ::viam::service::navigation::v1::AddWaypointResponse* resp) {
               return service->AddWaypoint(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NavigationService_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NavigationService::Service, ::viam::service::navigation::v1::RemoveWaypointRequest, ::viam::service::navigation::v1::RemoveWaypointResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](NavigationService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::navigation::v1::RemoveWaypointRequest* req,
             ::viam::service::navigation::v1::RemoveWaypointResponse* resp) {
               return service->RemoveWaypoint(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NavigationService_method_names[6],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NavigationService::Service, ::viam::service::navigation::v1::GetObstaclesRequest, ::viam::service::navigation::v1::GetObstaclesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](NavigationService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::navigation::v1::GetObstaclesRequest* req,
             ::viam::service::navigation::v1::GetObstaclesResponse* resp) {
               return service->GetObstacles(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NavigationService_method_names[7],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NavigationService::Service, ::viam::service::navigation::v1::GetPathsRequest, ::viam::service::navigation::v1::GetPathsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](NavigationService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::navigation::v1::GetPathsRequest* req,
             ::viam::service::navigation::v1::GetPathsResponse* resp) {
               return service->GetPaths(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NavigationService_method_names[8],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NavigationService::Service, ::viam::service::navigation::v1::GetPropertiesRequest, ::viam::service::navigation::v1::GetPropertiesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](NavigationService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::service::navigation::v1::GetPropertiesRequest* req,
             ::viam::service::navigation::v1::GetPropertiesResponse* resp) {
               return service->GetProperties(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NavigationService_method_names[9],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NavigationService::Service, ::viam::common::v1::DoCommandRequest, ::viam::common::v1::DoCommandResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](NavigationService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::common::v1::DoCommandRequest* req,
             ::viam::common::v1::DoCommandResponse* resp) {
               return service->DoCommand(ctx, req, resp);
             }, this)));
}

NavigationService::Service::~Service() {
}

::grpc::Status NavigationService::Service::GetMode(::grpc::ServerContext* context, const ::viam::service::navigation::v1::GetModeRequest* request, ::viam::service::navigation::v1::GetModeResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NavigationService::Service::SetMode(::grpc::ServerContext* context, const ::viam::service::navigation::v1::SetModeRequest* request, ::viam::service::navigation::v1::SetModeResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NavigationService::Service::GetLocation(::grpc::ServerContext* context, const ::viam::service::navigation::v1::GetLocationRequest* request, ::viam::service::navigation::v1::GetLocationResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NavigationService::Service::GetWaypoints(::grpc::ServerContext* context, const ::viam::service::navigation::v1::GetWaypointsRequest* request, ::viam::service::navigation::v1::GetWaypointsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NavigationService::Service::AddWaypoint(::grpc::ServerContext* context, const ::viam::service::navigation::v1::AddWaypointRequest* request, ::viam::service::navigation::v1::AddWaypointResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NavigationService::Service::RemoveWaypoint(::grpc::ServerContext* context, const ::viam::service::navigation::v1::RemoveWaypointRequest* request, ::viam::service::navigation::v1::RemoveWaypointResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NavigationService::Service::GetObstacles(::grpc::ServerContext* context, const ::viam::service::navigation::v1::GetObstaclesRequest* request, ::viam::service::navigation::v1::GetObstaclesResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NavigationService::Service::GetPaths(::grpc::ServerContext* context, const ::viam::service::navigation::v1::GetPathsRequest* request, ::viam::service::navigation::v1::GetPathsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NavigationService::Service::GetProperties(::grpc::ServerContext* context, const ::viam::service::navigation::v1::GetPropertiesRequest* request, ::viam::service::navigation::v1::GetPropertiesResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NavigationService::Service::DoCommand(::grpc::ServerContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace service
}  // namespace navigation
}  // namespace v1

