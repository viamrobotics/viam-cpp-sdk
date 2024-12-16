#pragma once

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/private/utils.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>

namespace grpc {

class Status;

template <typename T>
class ClientReaderInterface;

}  // namespace grpc

namespace viam {
namespace sdk {

namespace client_helper_details {

[[noreturn]] void errorHandlerReturnedUnexpectedly(const ::grpc::Status&) noexcept;

}  // namespace client_helper_details

// Method type for a gRPC call that returns a response message type.
template <typename StubType, typename RequestType, typename ResponseType>
using SyncMethodType = ::grpc::Status (StubType::*)(::grpc::ClientContext*,
                                                    const RequestType&,
                                                    ResponseType*);

// Method type for a gRPC call that returns a stream of response message type.
template <typename StubType, typename RequestType, typename ResponseType>
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

    ClientHelper& with(const ProtoStruct& extra) {
        return with(extra, default_rsc_);
    }

    template <typename RequestSetupCallable>
    ClientHelper& with(RequestSetupCallable&& rsc) {
        std::forward<RequestSetupCallable>(rsc)(request_);
        return *this;
    }

    template <typename RequestSetupCallable>
    ClientHelper& with(const ProtoStruct& extra, RequestSetupCallable&& rsc) {
        auto key = extra.find(impl::debug_map_key);
        if (key != extra.end()) {
            ProtoValue value = key->second;
            debug_key_ = *value.get<std::string>();
        }

        proto_convert_details::to_proto<ProtoStruct>{}(extra, request_.mutable_extra());
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

        if (debug_key_ != "") {
            ctx.set_debug_key(debug_key_);
        }
        const auto result = (stub_->*pfn_)(ctx, request_, &response_);
        if (result.ok()) {
            return std::forward<ResponseHandlerCallable>(rhc)(
                const_cast<const ResponseType&>(response_));
        }

        std::forward<ErrorHandlerCallable>(ehc)(result);
        client_helper_details::errorHandlerReturnedUnexpectedly(result);
    }

    // A version of invoke for gRPC calls returning `(stream ResponseType)`.
    // ResponseHandlerCallable will be called for every response in the reader, and should return
    // false to indicate it is no longer interested in the stream.
    template <typename ResponseHandlerCallable,
              typename ErrorHandlerCallable = decltype(default_ehc_)>
    auto invoke_stream(ResponseHandlerCallable rhc, ErrorHandlerCallable&& ehc = default_ehc_) {
        *request_.mutable_name() = client_->name();
        ClientContext ctx;

        auto reader = (stub_->*pfn_)(ctx, request_);

        bool cancelled_by_handler = false;

        while (reader->Read(&response_)) {
            if (!rhc(response_)) {
                cancelled_by_handler = true;
                ctx.try_cancel();
                break;
            }
        }

        const auto result = reader->Finish();

        if (result.ok() ||
            (cancelled_by_handler && result.error_code() == ::grpc::StatusCode::CANCELLED)) {
            return;
        }

        std::forward<ErrorHandlerCallable>(ehc)(result);
        client_helper_details::errorHandlerReturnedUnexpectedly(result);
    }

   private:
    ClientType* client_;
    StubType* stub_;
    std::string debug_key_;
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
