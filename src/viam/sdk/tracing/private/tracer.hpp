#pragma once

#ifdef VIAMCPPSDK_OPENTELEMETRY_TRACING
#include <memory>

#include <opentelemetry/sdk/trace/tracer_provider.h>
#endif

namespace viam {
namespace sdk {

class RobotClient;

namespace impl {

/// @brief Holds the SDK-side OpenTelemetry tracer provider. Spans go nowhere until
/// @c initialize_provider installs an exporter that ships them to the parent.
class Tracer {
   public:
    Tracer();
    ~Tracer() noexcept;  // NOLINT(performance-trivially-destructible)

    Tracer(const Tracer&) = delete;
    Tracer& operator=(const Tracer&) = delete;

    /// @brief Returns the @c Tracer owned by the current @c Instance.
    static Tracer& get();

    /// @brief Install the W3C Trace Context propagator. Called once during @c Instance
    /// construction.
    static void initialize_propagator() noexcept;

    /// @brief Install an exporter sourced from @p client. The caller must keep @p client alive
    /// until @c shutdown_provider returns.
    void initialize_provider(RobotClient* client) noexcept;
    void shutdown_provider() noexcept;

#ifdef VIAMCPPSDK_OPENTELEMETRY_TRACING
   private:
    std::shared_ptr<opentelemetry::sdk::trace::TracerProvider> sdk_provider_;
#endif
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
