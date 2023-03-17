// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: proto/rpc/v1/auth.proto
#ifndef GRPC_proto_2frpc_2fv1_2fauth_2eproto__INCLUDED
#define GRPC_proto_2frpc_2fv1_2fauth_2eproto__INCLUDED

#include "proto/rpc/v1/auth.pb.h"

#include <functional>
#include <grpcpp/generic/async_generic_service.h>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/proto_utils.h>
#include <grpcpp/impl/rpc_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>
#include <grpcpp/support/stub_options.h>
#include <grpcpp/support/sync_stream.h>

namespace proto {
namespace rpc {
namespace v1 {

// An AuthService is intended to be used as a means to perform application level
// authentication. Its sole Authenticate method should be used prior to any
// other services that a gRPC server has to offer.
class AuthService final {
 public:
  static constexpr char const* service_full_name() {
    return "proto.rpc.v1.AuthService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    // Authenticate attempts to authenticate the caller. The resulting
    // response contains an access token that should be used for future
    // requests.
    virtual ::grpc::Status Authenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::proto::rpc::v1::AuthenticateResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateResponse>> AsyncAuthenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateResponse>>(AsyncAuthenticateRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateResponse>> PrepareAsyncAuthenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateResponse>>(PrepareAsyncAuthenticateRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      // Authenticate attempts to authenticate the caller. The resulting
      // response contains an access token that should be used for future
      // requests.
      virtual void Authenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest* request, ::proto::rpc::v1::AuthenticateResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Authenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest* request, ::proto::rpc::v1::AuthenticateResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateResponse>* AsyncAuthenticateRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateResponse>* PrepareAsyncAuthenticateRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status Authenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::proto::rpc::v1::AuthenticateResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateResponse>> AsyncAuthenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateResponse>>(AsyncAuthenticateRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateResponse>> PrepareAsyncAuthenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateResponse>>(PrepareAsyncAuthenticateRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void Authenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest* request, ::proto::rpc::v1::AuthenticateResponse* response, std::function<void(::grpc::Status)>) override;
      void Authenticate(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest* request, ::proto::rpc::v1::AuthenticateResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateResponse>* AsyncAuthenticateRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateResponse>* PrepareAsyncAuthenticateRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Authenticate_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    // Authenticate attempts to authenticate the caller. The resulting
    // response contains an access token that should be used for future
    // requests.
    virtual ::grpc::Status Authenticate(::grpc::ServerContext* context, const ::proto::rpc::v1::AuthenticateRequest* request, ::proto::rpc::v1::AuthenticateResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_Authenticate : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Authenticate() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Authenticate() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Authenticate(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateRequest* /*request*/, ::proto::rpc::v1::AuthenticateResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestAuthenticate(::grpc::ServerContext* context, ::proto::rpc::v1::AuthenticateRequest* request, ::grpc::ServerAsyncResponseWriter< ::proto::rpc::v1::AuthenticateResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Authenticate<Service > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_Authenticate : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_Authenticate() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::proto::rpc::v1::AuthenticateRequest, ::proto::rpc::v1::AuthenticateResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::proto::rpc::v1::AuthenticateRequest* request, ::proto::rpc::v1::AuthenticateResponse* response) { return this->Authenticate(context, request, response); }));}
    void SetMessageAllocatorFor_Authenticate(
        ::grpc::MessageAllocator< ::proto::rpc::v1::AuthenticateRequest, ::proto::rpc::v1::AuthenticateResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::proto::rpc::v1::AuthenticateRequest, ::proto::rpc::v1::AuthenticateResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_Authenticate() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Authenticate(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateRequest* /*request*/, ::proto::rpc::v1::AuthenticateResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Authenticate(
      ::grpc::CallbackServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateRequest* /*request*/, ::proto::rpc::v1::AuthenticateResponse* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_Authenticate<Service > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Authenticate : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Authenticate() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Authenticate() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Authenticate(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateRequest* /*request*/, ::proto::rpc::v1::AuthenticateResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Authenticate : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Authenticate() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Authenticate() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Authenticate(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateRequest* /*request*/, ::proto::rpc::v1::AuthenticateResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestAuthenticate(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_Authenticate : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_Authenticate() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Authenticate(context, request, response); }));
    }
    ~WithRawCallbackMethod_Authenticate() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Authenticate(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateRequest* /*request*/, ::proto::rpc::v1::AuthenticateResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Authenticate(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Authenticate : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Authenticate() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::proto::rpc::v1::AuthenticateRequest, ::proto::rpc::v1::AuthenticateResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::proto::rpc::v1::AuthenticateRequest, ::proto::rpc::v1::AuthenticateResponse>* streamer) {
                       return this->StreamedAuthenticate(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Authenticate() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Authenticate(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateRequest* /*request*/, ::proto::rpc::v1::AuthenticateResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedAuthenticate(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::proto::rpc::v1::AuthenticateRequest,::proto::rpc::v1::AuthenticateResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_Authenticate<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_Authenticate<Service > StreamedService;
};

// An ExternalAuthService is intended to be used as a means to perform application level
// authentication but for an external entity that it is responsible for handling auth for.
// An example of its use would be for a entity that only trusts an external source to
// perform authentication for it.
// Its sole AuthenticateTo method should be used prior to any other services that the external
// entity's gRPC server has to offer.
class ExternalAuthService final {
 public:
  static constexpr char const* service_full_name() {
    return "proto.rpc.v1.ExternalAuthService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    // AuthenticateTo attempts to authenticate the caller on behalf of an entity.
    // The resulting response contains an access token that should be used for future
    // requests. This assumes that the caller is already authenticated to the
    // server implementing this service.
    virtual ::grpc::Status AuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::proto::rpc::v1::AuthenticateToResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateToResponse>> AsyncAuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateToResponse>>(AsyncAuthenticateToRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateToResponse>> PrepareAsyncAuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateToResponse>>(PrepareAsyncAuthenticateToRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      // AuthenticateTo attempts to authenticate the caller on behalf of an entity.
      // The resulting response contains an access token that should be used for future
      // requests. This assumes that the caller is already authenticated to the
      // server implementing this service.
      virtual void AuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest* request, ::proto::rpc::v1::AuthenticateToResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void AuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest* request, ::proto::rpc::v1::AuthenticateToResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateToResponse>* AsyncAuthenticateToRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::proto::rpc::v1::AuthenticateToResponse>* PrepareAsyncAuthenticateToRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status AuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::proto::rpc::v1::AuthenticateToResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateToResponse>> AsyncAuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateToResponse>>(AsyncAuthenticateToRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateToResponse>> PrepareAsyncAuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateToResponse>>(PrepareAsyncAuthenticateToRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void AuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest* request, ::proto::rpc::v1::AuthenticateToResponse* response, std::function<void(::grpc::Status)>) override;
      void AuthenticateTo(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest* request, ::proto::rpc::v1::AuthenticateToResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateToResponse>* AsyncAuthenticateToRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::proto::rpc::v1::AuthenticateToResponse>* PrepareAsyncAuthenticateToRaw(::grpc::ClientContext* context, const ::proto::rpc::v1::AuthenticateToRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_AuthenticateTo_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    // AuthenticateTo attempts to authenticate the caller on behalf of an entity.
    // The resulting response contains an access token that should be used for future
    // requests. This assumes that the caller is already authenticated to the
    // server implementing this service.
    virtual ::grpc::Status AuthenticateTo(::grpc::ServerContext* context, const ::proto::rpc::v1::AuthenticateToRequest* request, ::proto::rpc::v1::AuthenticateToResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_AuthenticateTo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_AuthenticateTo() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_AuthenticateTo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status AuthenticateTo(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateToRequest* /*request*/, ::proto::rpc::v1::AuthenticateToResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestAuthenticateTo(::grpc::ServerContext* context, ::proto::rpc::v1::AuthenticateToRequest* request, ::grpc::ServerAsyncResponseWriter< ::proto::rpc::v1::AuthenticateToResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_AuthenticateTo<Service > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_AuthenticateTo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_AuthenticateTo() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::proto::rpc::v1::AuthenticateToRequest, ::proto::rpc::v1::AuthenticateToResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::proto::rpc::v1::AuthenticateToRequest* request, ::proto::rpc::v1::AuthenticateToResponse* response) { return this->AuthenticateTo(context, request, response); }));}
    void SetMessageAllocatorFor_AuthenticateTo(
        ::grpc::MessageAllocator< ::proto::rpc::v1::AuthenticateToRequest, ::proto::rpc::v1::AuthenticateToResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::proto::rpc::v1::AuthenticateToRequest, ::proto::rpc::v1::AuthenticateToResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_AuthenticateTo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status AuthenticateTo(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateToRequest* /*request*/, ::proto::rpc::v1::AuthenticateToResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* AuthenticateTo(
      ::grpc::CallbackServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateToRequest* /*request*/, ::proto::rpc::v1::AuthenticateToResponse* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_AuthenticateTo<Service > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_AuthenticateTo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_AuthenticateTo() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_AuthenticateTo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status AuthenticateTo(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateToRequest* /*request*/, ::proto::rpc::v1::AuthenticateToResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_AuthenticateTo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_AuthenticateTo() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_AuthenticateTo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status AuthenticateTo(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateToRequest* /*request*/, ::proto::rpc::v1::AuthenticateToResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestAuthenticateTo(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_AuthenticateTo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_AuthenticateTo() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->AuthenticateTo(context, request, response); }));
    }
    ~WithRawCallbackMethod_AuthenticateTo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status AuthenticateTo(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateToRequest* /*request*/, ::proto::rpc::v1::AuthenticateToResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* AuthenticateTo(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_AuthenticateTo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_AuthenticateTo() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::proto::rpc::v1::AuthenticateToRequest, ::proto::rpc::v1::AuthenticateToResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::proto::rpc::v1::AuthenticateToRequest, ::proto::rpc::v1::AuthenticateToResponse>* streamer) {
                       return this->StreamedAuthenticateTo(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_AuthenticateTo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status AuthenticateTo(::grpc::ServerContext* /*context*/, const ::proto::rpc::v1::AuthenticateToRequest* /*request*/, ::proto::rpc::v1::AuthenticateToResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedAuthenticateTo(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::proto::rpc::v1::AuthenticateToRequest,::proto::rpc::v1::AuthenticateToResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_AuthenticateTo<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_AuthenticateTo<Service > StreamedService;
};

}  // namespace v1
}  // namespace rpc
}  // namespace proto


#endif  // GRPC_proto_2frpc_2fv1_2fauth_2eproto__INCLUDED
