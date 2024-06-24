// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: app/mltraining/v1/ml_training.proto

#include "app/mltraining/v1/ml_training.pb.h"
#include "app/mltraining/v1/ml_training.grpc.pb.h"

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
namespace app {
namespace mltraining {
namespace v1 {

static const char* MLTrainingService_method_names[] = {
  "/viam.app.mltraining.v1.MLTrainingService/SubmitTrainingJob",
  "/viam.app.mltraining.v1.MLTrainingService/SubmitCustomTrainingJob",
  "/viam.app.mltraining.v1.MLTrainingService/GetTrainingJob",
  "/viam.app.mltraining.v1.MLTrainingService/ListTrainingJobs",
  "/viam.app.mltraining.v1.MLTrainingService/CancelTrainingJob",
  "/viam.app.mltraining.v1.MLTrainingService/DeleteCompletedTrainingJob",
};

std::unique_ptr< MLTrainingService::Stub> MLTrainingService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< MLTrainingService::Stub> stub(new MLTrainingService::Stub(channel, options));
  return stub;
}

MLTrainingService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_SubmitTrainingJob_(MLTrainingService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_SubmitCustomTrainingJob_(MLTrainingService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetTrainingJob_(MLTrainingService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ListTrainingJobs_(MLTrainingService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_CancelTrainingJob_(MLTrainingService_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DeleteCompletedTrainingJob_(MLTrainingService_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status MLTrainingService::Stub::SubmitTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::SubmitTrainingJobRequest& request, ::viam::app::mltraining::v1::SubmitTrainingJobResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::mltraining::v1::SubmitTrainingJobRequest, ::viam::app::mltraining::v1::SubmitTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SubmitTrainingJob_, context, request, response);
}

void MLTrainingService::Stub::async::SubmitTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::SubmitTrainingJobRequest* request, ::viam::app::mltraining::v1::SubmitTrainingJobResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::mltraining::v1::SubmitTrainingJobRequest, ::viam::app::mltraining::v1::SubmitTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SubmitTrainingJob_, context, request, response, std::move(f));
}

void MLTrainingService::Stub::async::SubmitTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::SubmitTrainingJobRequest* request, ::viam::app::mltraining::v1::SubmitTrainingJobResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SubmitTrainingJob_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::SubmitTrainingJobResponse>* MLTrainingService::Stub::PrepareAsyncSubmitTrainingJobRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::SubmitTrainingJobRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::mltraining::v1::SubmitTrainingJobResponse, ::viam::app::mltraining::v1::SubmitTrainingJobRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SubmitTrainingJob_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::SubmitTrainingJobResponse>* MLTrainingService::Stub::AsyncSubmitTrainingJobRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::SubmitTrainingJobRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSubmitTrainingJobRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MLTrainingService::Stub::SubmitCustomTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest& request, ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest, ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SubmitCustomTrainingJob_, context, request, response);
}

void MLTrainingService::Stub::async::SubmitCustomTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest* request, ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest, ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SubmitCustomTrainingJob_, context, request, response, std::move(f));
}

void MLTrainingService::Stub::async::SubmitCustomTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest* request, ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SubmitCustomTrainingJob_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse>* MLTrainingService::Stub::PrepareAsyncSubmitCustomTrainingJobRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse, ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SubmitCustomTrainingJob_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse>* MLTrainingService::Stub::AsyncSubmitCustomTrainingJobRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSubmitCustomTrainingJobRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MLTrainingService::Stub::GetTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::GetTrainingJobRequest& request, ::viam::app::mltraining::v1::GetTrainingJobResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::mltraining::v1::GetTrainingJobRequest, ::viam::app::mltraining::v1::GetTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetTrainingJob_, context, request, response);
}

void MLTrainingService::Stub::async::GetTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::GetTrainingJobRequest* request, ::viam::app::mltraining::v1::GetTrainingJobResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::mltraining::v1::GetTrainingJobRequest, ::viam::app::mltraining::v1::GetTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetTrainingJob_, context, request, response, std::move(f));
}

void MLTrainingService::Stub::async::GetTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::GetTrainingJobRequest* request, ::viam::app::mltraining::v1::GetTrainingJobResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetTrainingJob_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::GetTrainingJobResponse>* MLTrainingService::Stub::PrepareAsyncGetTrainingJobRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::GetTrainingJobRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::mltraining::v1::GetTrainingJobResponse, ::viam::app::mltraining::v1::GetTrainingJobRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetTrainingJob_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::GetTrainingJobResponse>* MLTrainingService::Stub::AsyncGetTrainingJobRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::GetTrainingJobRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetTrainingJobRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MLTrainingService::Stub::ListTrainingJobs(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::ListTrainingJobsRequest& request, ::viam::app::mltraining::v1::ListTrainingJobsResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::mltraining::v1::ListTrainingJobsRequest, ::viam::app::mltraining::v1::ListTrainingJobsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_ListTrainingJobs_, context, request, response);
}

void MLTrainingService::Stub::async::ListTrainingJobs(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::ListTrainingJobsRequest* request, ::viam::app::mltraining::v1::ListTrainingJobsResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::mltraining::v1::ListTrainingJobsRequest, ::viam::app::mltraining::v1::ListTrainingJobsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ListTrainingJobs_, context, request, response, std::move(f));
}

void MLTrainingService::Stub::async::ListTrainingJobs(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::ListTrainingJobsRequest* request, ::viam::app::mltraining::v1::ListTrainingJobsResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ListTrainingJobs_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::ListTrainingJobsResponse>* MLTrainingService::Stub::PrepareAsyncListTrainingJobsRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::ListTrainingJobsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::mltraining::v1::ListTrainingJobsResponse, ::viam::app::mltraining::v1::ListTrainingJobsRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_ListTrainingJobs_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::ListTrainingJobsResponse>* MLTrainingService::Stub::AsyncListTrainingJobsRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::ListTrainingJobsRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncListTrainingJobsRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MLTrainingService::Stub::CancelTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::CancelTrainingJobRequest& request, ::viam::app::mltraining::v1::CancelTrainingJobResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::mltraining::v1::CancelTrainingJobRequest, ::viam::app::mltraining::v1::CancelTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_CancelTrainingJob_, context, request, response);
}

void MLTrainingService::Stub::async::CancelTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::CancelTrainingJobRequest* request, ::viam::app::mltraining::v1::CancelTrainingJobResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::mltraining::v1::CancelTrainingJobRequest, ::viam::app::mltraining::v1::CancelTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_CancelTrainingJob_, context, request, response, std::move(f));
}

void MLTrainingService::Stub::async::CancelTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::CancelTrainingJobRequest* request, ::viam::app::mltraining::v1::CancelTrainingJobResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_CancelTrainingJob_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::CancelTrainingJobResponse>* MLTrainingService::Stub::PrepareAsyncCancelTrainingJobRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::CancelTrainingJobRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::mltraining::v1::CancelTrainingJobResponse, ::viam::app::mltraining::v1::CancelTrainingJobRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_CancelTrainingJob_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::CancelTrainingJobResponse>* MLTrainingService::Stub::AsyncCancelTrainingJobRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::CancelTrainingJobRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncCancelTrainingJobRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MLTrainingService::Stub::DeleteCompletedTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest& request, ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest, ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DeleteCompletedTrainingJob_, context, request, response);
}

void MLTrainingService::Stub::async::DeleteCompletedTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest* request, ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest, ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DeleteCompletedTrainingJob_, context, request, response, std::move(f));
}

void MLTrainingService::Stub::async::DeleteCompletedTrainingJob(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest* request, ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DeleteCompletedTrainingJob_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse>* MLTrainingService::Stub::PrepareAsyncDeleteCompletedTrainingJobRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse, ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DeleteCompletedTrainingJob_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse>* MLTrainingService::Stub::AsyncDeleteCompletedTrainingJobRaw(::grpc::ClientContext* context, const ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDeleteCompletedTrainingJobRaw(context, request, cq);
  result->StartCall();
  return result;
}

MLTrainingService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MLTrainingService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MLTrainingService::Service, ::viam::app::mltraining::v1::SubmitTrainingJobRequest, ::viam::app::mltraining::v1::SubmitTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MLTrainingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::mltraining::v1::SubmitTrainingJobRequest* req,
             ::viam::app::mltraining::v1::SubmitTrainingJobResponse* resp) {
               return service->SubmitTrainingJob(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MLTrainingService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MLTrainingService::Service, ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest, ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MLTrainingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest* req,
             ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse* resp) {
               return service->SubmitCustomTrainingJob(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MLTrainingService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MLTrainingService::Service, ::viam::app::mltraining::v1::GetTrainingJobRequest, ::viam::app::mltraining::v1::GetTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MLTrainingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::mltraining::v1::GetTrainingJobRequest* req,
             ::viam::app::mltraining::v1::GetTrainingJobResponse* resp) {
               return service->GetTrainingJob(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MLTrainingService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MLTrainingService::Service, ::viam::app::mltraining::v1::ListTrainingJobsRequest, ::viam::app::mltraining::v1::ListTrainingJobsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MLTrainingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::mltraining::v1::ListTrainingJobsRequest* req,
             ::viam::app::mltraining::v1::ListTrainingJobsResponse* resp) {
               return service->ListTrainingJobs(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MLTrainingService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MLTrainingService::Service, ::viam::app::mltraining::v1::CancelTrainingJobRequest, ::viam::app::mltraining::v1::CancelTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MLTrainingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::mltraining::v1::CancelTrainingJobRequest* req,
             ::viam::app::mltraining::v1::CancelTrainingJobResponse* resp) {
               return service->CancelTrainingJob(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MLTrainingService_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MLTrainingService::Service, ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest, ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MLTrainingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest* req,
             ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse* resp) {
               return service->DeleteCompletedTrainingJob(ctx, req, resp);
             }, this)));
}

MLTrainingService::Service::~Service() {
}

::grpc::Status MLTrainingService::Service::SubmitTrainingJob(::grpc::ServerContext* context, const ::viam::app::mltraining::v1::SubmitTrainingJobRequest* request, ::viam::app::mltraining::v1::SubmitTrainingJobResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MLTrainingService::Service::SubmitCustomTrainingJob(::grpc::ServerContext* context, const ::viam::app::mltraining::v1::SubmitCustomTrainingJobRequest* request, ::viam::app::mltraining::v1::SubmitCustomTrainingJobResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MLTrainingService::Service::GetTrainingJob(::grpc::ServerContext* context, const ::viam::app::mltraining::v1::GetTrainingJobRequest* request, ::viam::app::mltraining::v1::GetTrainingJobResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MLTrainingService::Service::ListTrainingJobs(::grpc::ServerContext* context, const ::viam::app::mltraining::v1::ListTrainingJobsRequest* request, ::viam::app::mltraining::v1::ListTrainingJobsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MLTrainingService::Service::CancelTrainingJob(::grpc::ServerContext* context, const ::viam::app::mltraining::v1::CancelTrainingJobRequest* request, ::viam::app::mltraining::v1::CancelTrainingJobResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MLTrainingService::Service::DeleteCompletedTrainingJob(::grpc::ServerContext* context, const ::viam::app::mltraining::v1::DeleteCompletedTrainingJobRequest* request, ::viam::app::mltraining::v1::DeleteCompletedTrainingJobResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace app
}  // namespace mltraining
}  // namespace v1

