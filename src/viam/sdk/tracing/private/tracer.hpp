#pragma once

#include <memory>

namespace viam {
namespace sdk {

class RobotClient;

namespace impl {

/// @brief Install the W3C Trace Context propagator. Called once during @c Instance construction.
void initialize_trace_propagator() noexcept;

/// @brief Holds the SDK-side OpenTelemetry tracer provider. Spans go nowhere until
/// @c initialize_provider installs an exporter that ships them to the parent.
class Tracer {
   public:
    Tracer();
    ~Tracer();

    Tracer(const Tracer&) = delete;
    Tracer& operator=(const Tracer&) = delete;

    /// @brief Returns the @c Tracer owned by the current @c Instance.
    static Tracer& get();

    /// @brief Install an exporter sourced from @p client. The caller must keep @p client alive
    /// until @c shutdown_provider returns.
    void initialize_provider(RobotClient* client) noexcept;
    void shutdown_provider() noexcept;

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
