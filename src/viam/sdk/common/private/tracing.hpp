#pragma once

#include <memory>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/grpc_fwd.hpp>

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
    ~ServerSpanGuard() noexcept;

    /// @brief Record the final gRPC status before destruction and return it unchanged.
    ::grpc::Status commit(::grpc::Status status) noexcept;

    ServerSpanGuard(const ServerSpanGuard&) = delete;
    ServerSpanGuard& operator=(const ServerSpanGuard&) = delete;

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

/// @brief Inject the currently-active OpenTelemetry trace context as W3C @c traceparent /
/// @c tracestate metadata entries into an outgoing gRPC client context.
///
/// This propagates trace context from any server span that is currently active on the calling
/// thread (e.g., one created by @c ServerSpanGuard) to downstream gRPC calls. It is a no-op
/// when no span is active or when OpenTelemetry tracing is not compiled in.
void inject_trace_context(GrpcClientContext* ctx) noexcept;

/// @brief Install the W3C Trace Context propagator as the global OpenTelemetry text-map
/// propagator. Called once during @c Instance construction.
void initialize_trace_propagator() noexcept;

}  // namespace impl
}  // namespace sdk
}  // namespace viam
