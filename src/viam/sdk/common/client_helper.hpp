#pragma once

#include <grpcpp/client_context.h>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

namespace client_helper_details {
[[noreturn]] void errorHandlerReturnedUnexpectedly(const ::grpc::Status&) noexcept;
}  // namespace client_helper_details

template <typename ClientType, typename StubType, typename RequestType, typename ResponseType>
class ClientHelper {
    static void default_rsc_(RequestType&) {}
    static void default_rhc_(const ResponseType&) {}
    static void default_ehc_(const ::grpc::Status& status) {
        throw GRPCException(status.error_message(), status.error_code());
    }

   public:
    using PFn = ::grpc::Status (StubType::*)(::grpc::ClientContext*,
                                             const RequestType&,
                                             ResponseType*);
    explicit ClientHelper(ClientType* client, StubType* stub, PFn pfn)
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

   private:
    ClientType* client_;
    StubType* stub_;
    PFn pfn_;
    RequestType request_;
    ResponseType response_;
};

template <typename ClientType, typename StubType, typename RequestType, typename ResponseType>
auto make_client_helper(ClientType* client,
                        StubType& stub,
                        ::grpc::Status (StubType::*method)(::grpc::ClientContext*,
                                                           const RequestType&,
                                                           ResponseType*)) {
    return ClientHelper<ClientType, StubType, RequestType, ResponseType>(client, &stub, method);
}

}  // namespace sdk
}  // namespace viam
