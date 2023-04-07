// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: app/v1/billing.proto

#include "app/v1/billing.pb.h"
#include "app/v1/billing.grpc.pb.h"

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
namespace v1 {

static const char* BillingService_method_names[] = {
  "/viam.app.v1.BillingService/GetCurrentMonthUsageSummary",
  "/viam.app.v1.BillingService/GetUnpaidBalance",
  "/viam.app.v1.BillingService/GetInvoiceHistory",
  "/viam.app.v1.BillingService/GetItemizedInvoice",
  "/viam.app.v1.BillingService/GetBillingSummary",
};

std::unique_ptr< BillingService::Stub> BillingService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< BillingService::Stub> stub(new BillingService::Stub(channel, options));
  return stub;
}

BillingService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetCurrentMonthUsageSummary_(BillingService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetUnpaidBalance_(BillingService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetInvoiceHistory_(BillingService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetItemizedInvoice_(BillingService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetBillingSummary_(BillingService_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status BillingService::Stub::GetCurrentMonthUsageSummary(::grpc::ClientContext* context, const ::viam::app::v1::GetCurrentMonthUsageSummaryRequest& request, ::viam::app::v1::GetCurrentMonthUsageSummaryResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::v1::GetCurrentMonthUsageSummaryRequest, ::viam::app::v1::GetCurrentMonthUsageSummaryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetCurrentMonthUsageSummary_, context, request, response);
}

void BillingService::Stub::async::GetCurrentMonthUsageSummary(::grpc::ClientContext* context, const ::viam::app::v1::GetCurrentMonthUsageSummaryRequest* request, ::viam::app::v1::GetCurrentMonthUsageSummaryResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::v1::GetCurrentMonthUsageSummaryRequest, ::viam::app::v1::GetCurrentMonthUsageSummaryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetCurrentMonthUsageSummary_, context, request, response, std::move(f));
}

void BillingService::Stub::async::GetCurrentMonthUsageSummary(::grpc::ClientContext* context, const ::viam::app::v1::GetCurrentMonthUsageSummaryRequest* request, ::viam::app::v1::GetCurrentMonthUsageSummaryResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetCurrentMonthUsageSummary_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::GetCurrentMonthUsageSummaryResponse>* BillingService::Stub::PrepareAsyncGetCurrentMonthUsageSummaryRaw(::grpc::ClientContext* context, const ::viam::app::v1::GetCurrentMonthUsageSummaryRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::v1::GetCurrentMonthUsageSummaryResponse, ::viam::app::v1::GetCurrentMonthUsageSummaryRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetCurrentMonthUsageSummary_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::GetCurrentMonthUsageSummaryResponse>* BillingService::Stub::AsyncGetCurrentMonthUsageSummaryRaw(::grpc::ClientContext* context, const ::viam::app::v1::GetCurrentMonthUsageSummaryRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetCurrentMonthUsageSummaryRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status BillingService::Stub::GetUnpaidBalance(::grpc::ClientContext* context, const ::viam::app::v1::GetUnpaidBalanceRequest& request, ::viam::app::v1::GetUnpaidBalanceResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::v1::GetUnpaidBalanceRequest, ::viam::app::v1::GetUnpaidBalanceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetUnpaidBalance_, context, request, response);
}

void BillingService::Stub::async::GetUnpaidBalance(::grpc::ClientContext* context, const ::viam::app::v1::GetUnpaidBalanceRequest* request, ::viam::app::v1::GetUnpaidBalanceResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::v1::GetUnpaidBalanceRequest, ::viam::app::v1::GetUnpaidBalanceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetUnpaidBalance_, context, request, response, std::move(f));
}

void BillingService::Stub::async::GetUnpaidBalance(::grpc::ClientContext* context, const ::viam::app::v1::GetUnpaidBalanceRequest* request, ::viam::app::v1::GetUnpaidBalanceResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetUnpaidBalance_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::GetUnpaidBalanceResponse>* BillingService::Stub::PrepareAsyncGetUnpaidBalanceRaw(::grpc::ClientContext* context, const ::viam::app::v1::GetUnpaidBalanceRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::v1::GetUnpaidBalanceResponse, ::viam::app::v1::GetUnpaidBalanceRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetUnpaidBalance_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::GetUnpaidBalanceResponse>* BillingService::Stub::AsyncGetUnpaidBalanceRaw(::grpc::ClientContext* context, const ::viam::app::v1::GetUnpaidBalanceRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetUnpaidBalanceRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status BillingService::Stub::GetInvoiceHistory(::grpc::ClientContext* context, const ::viam::app::v1::GetInvoiceHistoryRequest& request, ::viam::app::v1::GetInvoiceHistoryResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::v1::GetInvoiceHistoryRequest, ::viam::app::v1::GetInvoiceHistoryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetInvoiceHistory_, context, request, response);
}

void BillingService::Stub::async::GetInvoiceHistory(::grpc::ClientContext* context, const ::viam::app::v1::GetInvoiceHistoryRequest* request, ::viam::app::v1::GetInvoiceHistoryResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::v1::GetInvoiceHistoryRequest, ::viam::app::v1::GetInvoiceHistoryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetInvoiceHistory_, context, request, response, std::move(f));
}

void BillingService::Stub::async::GetInvoiceHistory(::grpc::ClientContext* context, const ::viam::app::v1::GetInvoiceHistoryRequest* request, ::viam::app::v1::GetInvoiceHistoryResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetInvoiceHistory_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::GetInvoiceHistoryResponse>* BillingService::Stub::PrepareAsyncGetInvoiceHistoryRaw(::grpc::ClientContext* context, const ::viam::app::v1::GetInvoiceHistoryRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::v1::GetInvoiceHistoryResponse, ::viam::app::v1::GetInvoiceHistoryRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetInvoiceHistory_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::GetInvoiceHistoryResponse>* BillingService::Stub::AsyncGetInvoiceHistoryRaw(::grpc::ClientContext* context, const ::viam::app::v1::GetInvoiceHistoryRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetInvoiceHistoryRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status BillingService::Stub::GetItemizedInvoice(::grpc::ClientContext* context, const ::viam::app::v1::GetItemizedInvoiceRequest& request, ::viam::app::v1::GetItemizedInvoiceResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::v1::GetItemizedInvoiceRequest, ::viam::app::v1::GetItemizedInvoiceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetItemizedInvoice_, context, request, response);
}

void BillingService::Stub::async::GetItemizedInvoice(::grpc::ClientContext* context, const ::viam::app::v1::GetItemizedInvoiceRequest* request, ::viam::app::v1::GetItemizedInvoiceResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::v1::GetItemizedInvoiceRequest, ::viam::app::v1::GetItemizedInvoiceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetItemizedInvoice_, context, request, response, std::move(f));
}

void BillingService::Stub::async::GetItemizedInvoice(::grpc::ClientContext* context, const ::viam::app::v1::GetItemizedInvoiceRequest* request, ::viam::app::v1::GetItemizedInvoiceResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetItemizedInvoice_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::GetItemizedInvoiceResponse>* BillingService::Stub::PrepareAsyncGetItemizedInvoiceRaw(::grpc::ClientContext* context, const ::viam::app::v1::GetItemizedInvoiceRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::v1::GetItemizedInvoiceResponse, ::viam::app::v1::GetItemizedInvoiceRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetItemizedInvoice_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::GetItemizedInvoiceResponse>* BillingService::Stub::AsyncGetItemizedInvoiceRaw(::grpc::ClientContext* context, const ::viam::app::v1::GetItemizedInvoiceRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetItemizedInvoiceRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status BillingService::Stub::GetBillingSummary(::grpc::ClientContext* context, const ::viam::app::v1::GetBillingSummaryRequest& request, ::viam::app::v1::GetBillingSummaryResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::viam::app::v1::GetBillingSummaryRequest, ::viam::app::v1::GetBillingSummaryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetBillingSummary_, context, request, response);
}

void BillingService::Stub::async::GetBillingSummary(::grpc::ClientContext* context, const ::viam::app::v1::GetBillingSummaryRequest* request, ::viam::app::v1::GetBillingSummaryResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::viam::app::v1::GetBillingSummaryRequest, ::viam::app::v1::GetBillingSummaryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetBillingSummary_, context, request, response, std::move(f));
}

void BillingService::Stub::async::GetBillingSummary(::grpc::ClientContext* context, const ::viam::app::v1::GetBillingSummaryRequest* request, ::viam::app::v1::GetBillingSummaryResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetBillingSummary_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::GetBillingSummaryResponse>* BillingService::Stub::PrepareAsyncGetBillingSummaryRaw(::grpc::ClientContext* context, const ::viam::app::v1::GetBillingSummaryRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::app::v1::GetBillingSummaryResponse, ::viam::app::v1::GetBillingSummaryRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetBillingSummary_, context, request);
}

::grpc::ClientAsyncResponseReader< ::viam::app::v1::GetBillingSummaryResponse>* BillingService::Stub::AsyncGetBillingSummaryRaw(::grpc::ClientContext* context, const ::viam::app::v1::GetBillingSummaryRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetBillingSummaryRaw(context, request, cq);
  result->StartCall();
  return result;
}

BillingService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      BillingService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< BillingService::Service, ::viam::app::v1::GetCurrentMonthUsageSummaryRequest, ::viam::app::v1::GetCurrentMonthUsageSummaryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](BillingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::v1::GetCurrentMonthUsageSummaryRequest* req,
             ::viam::app::v1::GetCurrentMonthUsageSummaryResponse* resp) {
               return service->GetCurrentMonthUsageSummary(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      BillingService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< BillingService::Service, ::viam::app::v1::GetUnpaidBalanceRequest, ::viam::app::v1::GetUnpaidBalanceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](BillingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::v1::GetUnpaidBalanceRequest* req,
             ::viam::app::v1::GetUnpaidBalanceResponse* resp) {
               return service->GetUnpaidBalance(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      BillingService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< BillingService::Service, ::viam::app::v1::GetInvoiceHistoryRequest, ::viam::app::v1::GetInvoiceHistoryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](BillingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::v1::GetInvoiceHistoryRequest* req,
             ::viam::app::v1::GetInvoiceHistoryResponse* resp) {
               return service->GetInvoiceHistory(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      BillingService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< BillingService::Service, ::viam::app::v1::GetItemizedInvoiceRequest, ::viam::app::v1::GetItemizedInvoiceResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](BillingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::v1::GetItemizedInvoiceRequest* req,
             ::viam::app::v1::GetItemizedInvoiceResponse* resp) {
               return service->GetItemizedInvoice(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      BillingService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< BillingService::Service, ::viam::app::v1::GetBillingSummaryRequest, ::viam::app::v1::GetBillingSummaryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](BillingService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::viam::app::v1::GetBillingSummaryRequest* req,
             ::viam::app::v1::GetBillingSummaryResponse* resp) {
               return service->GetBillingSummary(ctx, req, resp);
             }, this)));
}

BillingService::Service::~Service() {
}

::grpc::Status BillingService::Service::GetCurrentMonthUsageSummary(::grpc::ServerContext* context, const ::viam::app::v1::GetCurrentMonthUsageSummaryRequest* request, ::viam::app::v1::GetCurrentMonthUsageSummaryResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status BillingService::Service::GetUnpaidBalance(::grpc::ServerContext* context, const ::viam::app::v1::GetUnpaidBalanceRequest* request, ::viam::app::v1::GetUnpaidBalanceResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status BillingService::Service::GetInvoiceHistory(::grpc::ServerContext* context, const ::viam::app::v1::GetInvoiceHistoryRequest* request, ::viam::app::v1::GetInvoiceHistoryResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status BillingService::Service::GetItemizedInvoice(::grpc::ServerContext* context, const ::viam::app::v1::GetItemizedInvoiceRequest* request, ::viam::app::v1::GetItemizedInvoiceResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status BillingService::Service::GetBillingSummary(::grpc::ServerContext* context, const ::viam::app::v1::GetBillingSummaryRequest* request, ::viam::app::v1::GetBillingSummaryResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace viam
}  // namespace app
}  // namespace v1

