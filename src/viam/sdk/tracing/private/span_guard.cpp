#include <viam/sdk/tracing/private/span_guard.hpp>

#include <grpcpp/client_context.h>
#include <grpcpp/server_context.h>

#ifdef VIAMCPPSDK_OPENTELEMETRY_TRACING

#include <utility>

#include <opentelemetry/common/attribute_value.h>
#include <opentelemetry/context/propagation/global_propagator.h>
#include <opentelemetry/context/runtime_context.h>
#include <opentelemetry/nostd/string_view.h>
#include <opentelemetry/trace/context.h>
#include <opentelemetry/trace/propagation/http_trace_context.h>
#include <opentelemetry/trace/provider.h>
#include <opentelemetry/trace/scope.h>
#include <opentelemetry/trace/span.h>
#include <opentelemetry/trace/span_startoptions.h>
#include <opentelemetry/trace/tracer.h>

namespace otel_ctx = opentelemetry::context;
namespace otel_prop = opentelemetry::context::propagation;
namespace otel_trace = opentelemetry::trace;

namespace viam {
namespace sdk {
namespace impl {

namespace {

constexpr const char* k_instrumentation_scope = "viam-cpp-sdk";

// Carrier for reading W3C trace context from incoming gRPC request metadata (server side).
class GrpcServerCarrier : public otel_prop::TextMapCarrier {
   public:
    explicit GrpcServerCarrier(const grpc::ServerContext& ctx) noexcept : ctx_(ctx) {}

    opentelemetry::nostd::string_view Get(
        opentelemetry::nostd::string_view key) const noexcept override {
        const auto& metadata = ctx_.client_metadata();
        const auto it = metadata.find(grpc::string_ref{key.data(), key.size()});
        if (it != metadata.end()) {
            return {it->second.data(), it->second.size()};
        }
        return {};
    }

    void Set(opentelemetry::nostd::string_view,
             opentelemetry::nostd::string_view) noexcept override {}

   private:
    const grpc::ServerContext& ctx_;
};

// Carrier for writing W3C trace context into outgoing gRPC request metadata (client side).
class GrpcClientCarrier : public otel_prop::TextMapCarrier {
   public:
    explicit GrpcClientCarrier(grpc::ClientContext* ctx) noexcept : ctx_(ctx) {}

    opentelemetry::nostd::string_view Get(
        opentelemetry::nostd::string_view) const noexcept override {
        return {};
    }

    void Set(opentelemetry::nostd::string_view key,
             opentelemetry::nostd::string_view value) noexcept override {
        ctx_->AddMetadata(std::string(key), std::string(value));
    }

   private:
    grpc::ClientContext* ctx_;
};

}  // namespace

opentelemetry::nostd::shared_ptr<otel_trace::Span> ServerSpanGuard::start_span_(
    const GrpcServerContext* ctx, const char* method) noexcept {
    auto tracer = otel_trace::Provider::GetTracerProvider()->GetTracer(k_instrumentation_scope);

    otel_trace::StartSpanOptions opts;
    opts.kind = otel_trace::SpanKind::kServer;

    if (ctx) {
        GrpcServerCarrier carrier{*ctx};
        auto current_ctx = otel_ctx::RuntimeContext::GetCurrent();
        const auto extracted = otel_prop::GlobalTextMapPropagator::GetGlobalPropagator()->Extract(
            carrier, current_ctx);
        opts.parent = otel_trace::GetSpan(extracted)->GetContext();
    }

    auto span = tracer->StartSpan(method, opts);
    span->SetAttribute("rpc.system", "grpc");
    return span;
}

// Constructing `scope_` makes the span active on the current thread.
ServerSpanGuard::ServerSpanGuard(const GrpcServerContext* ctx, const char* method) noexcept
    : span_(start_span_(ctx, method)), scope_(span_) {}

ServerSpanGuard::~ServerSpanGuard() noexcept {
    if (!committed_) {
        span_->SetStatus(otel_trace::StatusCode::kError, "handler threw an exception");
    }
    span_->End();
}

::grpc::Status ServerSpanGuard::commit(::grpc::Status status) noexcept {
    committed_ = true;
    if (status.error_code() == ::grpc::StatusCode::OK) {
        span_->SetStatus(otel_trace::StatusCode::kOk);
    } else {
        span_->SetStatus(otel_trace::StatusCode::kError);
        span_->SetAttribute("rpc.grpc.status_code", static_cast<int>(status.error_code()));
    }
    return status;
}

void ServerSpanGuard::record_exception(const std::exception& xcp) noexcept {
    committed_ = true;
    const opentelemetry::nostd::string_view what{xcp.what()};
    span_->SetAttribute("error.type", "std::exception");
    span_->AddEvent("exception",
                    {{"exception.type", opentelemetry::common::AttributeValue{"std::exception"}},
                     {"exception.message", opentelemetry::common::AttributeValue{what}}});
    span_->SetStatus(otel_trace::StatusCode::kError, what);
}

void ServerSpanGuard::record_unknown_exception() noexcept {
    committed_ = true;
    span_->SetAttribute("error.type", "unknown");
    span_->AddEvent("exception",
                    {{"exception.type", opentelemetry::common::AttributeValue{"unknown"}}});
    span_->SetStatus(otel_trace::StatusCode::kError, "unknown exception");
}

void inject_trace_context(GrpcClientContext* ctx) noexcept {
    GrpcClientCarrier carrier{ctx};
    otel_prop::GlobalTextMapPropagator::GetGlobalPropagator()->Inject(
        carrier, otel_ctx::RuntimeContext::GetCurrent());
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam

#else  // VIAMCPPSDK_OPENTELEMETRY_TRACING — provide no-op implementations

namespace viam {
namespace sdk {
namespace impl {

ServerSpanGuard::ServerSpanGuard(const GrpcServerContext*, const char*) noexcept {}

ServerSpanGuard::~ServerSpanGuard() noexcept = default;

::grpc::Status ServerSpanGuard::commit(  // NOLINT(readability-convert-member-functions-to-static)
    ::grpc::Status status) noexcept {
    return status;
}

void ServerSpanGuard::record_exception(  // NOLINT(readability-convert-member-functions-to-static)
    const std::exception&) noexcept {}

void ServerSpanGuard::
    record_unknown_exception()  // NOLINT(readability-convert-member-functions-to-static)
    noexcept {}

void inject_trace_context(GrpcClientContext*) noexcept {}

}  // namespace impl
}  // namespace sdk
}  // namespace viam

#endif  // VIAMCPPSDK_OPENTELEMETRY_TRACING
