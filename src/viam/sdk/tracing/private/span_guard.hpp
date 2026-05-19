#pragma once

#include <exception>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/grpc_fwd.hpp>

#ifdef VIAMCPPSDK_OPENTELEMETRY_TRACING
#include <opentelemetry/trace/scope.h>
#include <opentelemetry/trace/span.h>
#endif

namespace viam {
namespace sdk {
namespace impl {

/// @brief RAII guard that creates a server-side OpenTelemetry span for the duration of a gRPC
/// handler invocation. Extracts W3C trace context from the incoming gRPC request metadata and
/// starts a child span (or a new root span if no traceparent header is present). When
/// destroyed, ends the span and records the final gRPC status.
///
/// If OpenTelemetry tracing is not compiled in, or no tracer provider has been configured,
/// uses a no-op implementation.
///
/// @note Instances must be created and destroyed on the same thread (the gRPC handler thread).
class ServerSpanGuard {
   public:
    explicit ServerSpanGuard(const GrpcServerContext* ctx, const char* method) noexcept;
    ~ServerSpanGuard() noexcept;  // NOLINT(performance-trivially-destructible)

    /// @brief Record the final gRPC status before destruction and return it unchanged.
    ::grpc::Status commit(::grpc::Status status) noexcept;

    /// @brief Record @p xcp as an "exception" event and set span status to Error. Caller rethrows.
    void record_exception(const std::exception& xcp) noexcept;

    /// @brief Record an unknown (non-std::exception) failure. Caller rethrows.
    void record_unknown_exception() noexcept;

    ServerSpanGuard(const ServerSpanGuard&) = delete;
    ServerSpanGuard& operator=(const ServerSpanGuard&) = delete;

#ifdef VIAMCPPSDK_OPENTELEMETRY_TRACING
   private:
    // Builds the span and makes it active; runs before `scope_` in the constructor init list.
    static opentelemetry::nostd::shared_ptr<opentelemetry::trace::Span> start_span_(
        const GrpcServerContext* ctx, const char* method) noexcept;

    opentelemetry::nostd::shared_ptr<opentelemetry::trace::Span> span_;
    opentelemetry::trace::Scope scope_;
    bool committed_ = false;
#endif
};

/// @brief Inject the currently-active OpenTelemetry trace context as W3C @c traceparent /
/// @c tracestate metadata entries into an outgoing gRPC client context.
///
/// This propagates trace context from any server span that is currently active on the calling
/// thread (e.g., one created by @c ServerSpanGuard) to downstream gRPC calls. It is a no-op
/// when no span is active or when OpenTelemetry tracing is not compiled in.
void inject_trace_context(GrpcClientContext* ctx) noexcept;

#ifdef VIAMCPPSDK_OPENTELEMETRY_TRACING
/// @brief Record @p xcp as an "exception" event on @p span and set status to Error.
void record_exception(opentelemetry::trace::Span* span, const std::exception& xcp) noexcept;

/// @brief Record an unknown (non-std::exception) failure as an "exception" event on @p span
/// and set status to Error.
void record_unknown_exception(opentelemetry::trace::Span* span) noexcept;
#endif

}  // namespace impl
}  // namespace sdk
}  // namespace viam
