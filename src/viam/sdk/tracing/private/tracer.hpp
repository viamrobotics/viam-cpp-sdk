#pragma once

#include <memory>
#include <string>

namespace viam {
namespace sdk {
namespace impl {

/// @brief Install the W3C Trace Context propagator as the global OpenTelemetry text-map
/// propagator. Called once during @c Instance construction.
void initialize_trace_propagator() noexcept;

/// @brief Holds the SDK-side OpenTelemetry tracer provider, owned by @c Instance.
/// Spans created by @c ServerSpanGuard go nowhere until @c initialize_provider has
/// installed an exporter.
class Tracer {
   public:
    Tracer();
    ~Tracer();

    Tracer(const Tracer&) = delete;
    Tracer& operator=(const Tracer&) = delete;

    /// @brief Returns the @c Tracer owned by the current @c Instance.
    static Tracer& get();

    /// @brief Install a tracer provider that exports spans via OTLP/gRPC to @p endpoint.
    /// Replaces any previously-installed provider. No-op when tracing is not compiled in.
    void initialize_provider(const std::string& endpoint) noexcept;

    /// @brief Flush and shut down the installed tracer provider. No-op when tracing is
    /// not compiled in.
    void shutdown_provider() noexcept;

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
