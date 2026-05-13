/// @file tracing/span.hpp
///
/// @brief Public tracing API for module authors.
#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace viam {
namespace sdk {

/// @defgroup Tracing Classes related to distributed tracing

/// @brief RAII child span. Parent is whichever span is active on the current thread,
/// which inside a resource API method body is the SDK-managed gRPC handler span.
///
/// No-op when OpenTelemetry support is not compiled in, no tracer provider is installed,
/// or no parent span is active on the current thread. Must be constructed and destroyed
/// on the same thread, in LIFO order.
///
/// @code
/// std::vector<GeoPoint> MyMovementSensor::get_position(const ProtoStruct& extra) {
///     viam::sdk::Span span{"my_module.read_gps"};
///     span.set_attribute("sensor.model", model_);
///     return points;
/// }
/// @endcode
///
/// @ingroup Tracing
class Span {
   public:
    /// @brief Open a child span named @p name under the currently-active span.
    explicit Span(const char* name) noexcept;

    ~Span() noexcept;

    Span(const Span&) = delete;
    Span& operator=(const Span&) = delete;
    Span(Span&&) = delete;
    Span& operator=(Span&&) = delete;

    /// @brief Attach an attribute to the span. Supported value types: @c bool, @c
    /// std::int64_t, @c double, @c const @c char*, @c std::string. Other types fail to
    /// link.
    template <typename T>
    void set_attribute(const char* key, T value) noexcept;

    /// @brief Record a timestamped event on the span.
    void add_event(const char* name) noexcept;

    /// @brief Mark the span as having completed successfully.
    void set_status_ok() noexcept;

    /// @brief Mark the span as having failed. @p description is optional context.
    void set_status_error(const char* description = "") noexcept;

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// -- Explicit instantiation declarations for the supported attribute value types -- //
extern template void Span::set_attribute<bool>(const char*, bool) noexcept;
extern template void Span::set_attribute<std::int64_t>(const char*, std::int64_t) noexcept;
extern template void Span::set_attribute<double>(const char*, double) noexcept;
extern template void Span::set_attribute<const char*>(const char*, const char*) noexcept;
extern template void Span::set_attribute<std::string>(const char*, std::string) noexcept;

}  // namespace sdk
}  // namespace viam
