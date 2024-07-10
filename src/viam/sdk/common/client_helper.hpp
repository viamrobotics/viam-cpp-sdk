#pragma once

#include <grpcpp/client_context.h>
#include <grpcpp/support/sync_stream.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

namespace client_helper_details {
[[noreturn]] void errorHandlerReturnedUnexpectedly(const ::grpc::Status&) noexcept;
}  // namespace client_helper_details

// Method type for a gRPC call that returns a response message type.
template <class StubType, class RequestType, class ResponseType>
using SyncMethodType = ::grpc::Status (StubType::*)(::grpc::ClientContext*,
                                                    const RequestType&,
                                                    ResponseType*);

// Method type for a gRPC call that returns a stream of response message type.
template <class StubType, class RequestType, class ResponseType>
using StreamingMethodType = std::unique_ptr<::grpc::ClientReaderInterface<ResponseType>> (
    StubType::*)(::grpc::ClientContext*, const RequestType&);

template <typename ClientType,
          typename StubType,
          typename RequestType,
          typename ResponseType,
          typename MethodType>
class ClientHelper {
    static void default_rsc_(RequestType&) {}
    static void default_rhc_(const ResponseType&) {}
    static void default_ehc_(const ::grpc::Status& status) {
        throw GRPCException(status);
    }

   public:
    explicit ClientHelper(ClientType* client, StubType* stub, MethodType pfn)
        : client_(client), stub_(stub), pfn_(pfn) {}

    ClientHelper& with(const AttributeMap& extra) {
        return with(extra, default_rsc_);
    }

    template <typename RequestSetupCallable>
    ClientHelper& with(RequestSetupCallable&& rsc) {
        std::forward<RequestSetupCallable>(rsc)(request_);
        return *this;
    }

    template <typename RequestSetupCallable>
    ClientHelper& with(const AttributeMap& extra, RequestSetupCallable&& rsc) {
        *request_.mutable_extra() = map_to_struct(extra);
        return with(std::forward<RequestSetupCallable>(rsc));
    }

    template <typename ResponseHandlerCallable = decltype(default_rhc_)>
    auto invoke(ResponseHandlerCallable&& rhc = default_rhc_) {
        return invoke(std::forward<ResponseHandlerCallable>(rhc), default_ehc_);
    }

    template <typename ResponseHandlerCallable, typename ErrorHandlerCallable>
    auto invoke(ResponseHandlerCallable&& rhc, ErrorHandlerCallable&& ehc) {
        *request_.mutable_name() = client_->name();
        ClientContext ctx;

        const auto result = (stub_->*pfn_)(ctx, request_, &response_);
        if (result.ok()) {
            return std::forward<ResponseHandlerCallable>(rhc)(
                const_cast<const ResponseType&>(response_));
        }

        std::forward<ErrorHandlerCallable>(ehc)(result);
        client_helper_details::errorHandlerReturnedUnexpectedly(result);
    }

    template <typename ResponseHandlerCallable>
    auto invoke_stream(ResponseHandlerCallable rhc) {
        *request_.mutable_name() = client_->name();
        ClientContext ctx;

        auto reader = (stub_->*pfn_)(ctx, request_);

        while (reader->Read(&response_)) {
            if (!rhc(response_)) {
                break;
            }
        }
    }

   private:
    ClientType* client_;
    StubType* stub_;
    MethodType pfn_;
    RequestType request_;
    ResponseType response_;
};

template <typename ClientType, typename StubType, typename RequestType, typename ResponseType>
auto make_client_helper(ClientType* client,
                        StubType& stub,
                        SyncMethodType<StubType, RequestType, ResponseType> method) {
    return ClientHelper<ClientType,
                        StubType,
                        RequestType,
                        ResponseType,
                        SyncMethodType<StubType, RequestType, ResponseType>>(client, &stub, method);
}

template <typename ClientType, typename StubType, typename RequestType, typename ResponseType>
auto make_client_helper(ClientType* client,
                        StubType& stub,
                        StreamingMethodType<StubType, RequestType, ResponseType> method) {
    return ClientHelper<ClientType,
                        StubType,
                        RequestType,
                        ResponseType,
                        StreamingMethodType<StubType, RequestType, ResponseType>>(
        client, &stub, method);
}

}  // namespace sdk
}  // namespace viam
