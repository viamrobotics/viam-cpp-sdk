#include <viam/sdk/tracing/span.hpp>

#ifdef VIAMCPPSDK_OPENTELEMETRY_TRACING

#include <opentelemetry/context/runtime_context.h>
#include <opentelemetry/nostd/string_view.h>
#include <opentelemetry/trace/context.h>
#include <opentelemetry/trace/default_span.h>
#include <opentelemetry/trace/provider.h>
#include <opentelemetry/trace/scope.h>
#include <opentelemetry/trace/span.h>
#include <opentelemetry/trace/span_context.h>
#include <opentelemetry/trace/tracer.h>

namespace otel_ctx = opentelemetry::context;
namespace otel_trace = opentelemetry::trace;

namespace viam {
namespace sdk {

namespace {

constexpr const char k_instrumentation_scope[] = "viam-cpp-sdk.module";

bool has_active_span() noexcept {
    return otel_trace::GetSpan(otel_ctx::RuntimeContext::GetCurrent())->GetContext().IsValid();
}

opentelemetry::nostd::shared_ptr<otel_trace::Span> start_or_noop(const char* name) noexcept {
    if (!has_active_span()) {
        return opentelemetry::nostd::shared_ptr<otel_trace::Span>{
            new otel_trace::DefaultSpan{otel_trace::SpanContext::GetInvalid()}};
    }
    return otel_trace::Provider::GetTracerProvider()
        ->GetTracer(k_instrumentation_scope)
        ->StartSpan(name);
}

}  // namespace

struct TracingSpan::Impl {
    opentelemetry::nostd::shared_ptr<otel_trace::Span> span;
    otel_trace::Scope scope;

    explicit Impl(opentelemetry::nostd::shared_ptr<otel_trace::Span> s) noexcept
        : span(std::move(s)), scope(span) {}
};

TracingSpan::TracingSpan(const char* name) noexcept
    : impl_(std::make_unique<Impl>(start_or_noop(name))) {}

TracingSpan::~TracingSpan() noexcept {
    end();
}

template <typename T>
void TracingSpan::set_attribute(const char* key, T value) noexcept {
    impl_->span->SetAttribute(key, value);
}

// std::string requires an explicit conversion to nostd::string_view because OTel's
// AttributeValue does not accept std::string directly.
template <>
void TracingSpan::set_attribute<std::string>(const char* key, std::string value) noexcept {
    impl_->span->SetAttribute(key, opentelemetry::nostd::string_view{value.data(), value.size()});
}

// -- Explicit instantiations -- //
template void TracingSpan::set_attribute<bool>(const char*, bool) noexcept;
template void TracingSpan::set_attribute<std::int64_t>(const char*, std::int64_t) noexcept;
template void TracingSpan::set_attribute<double>(const char*, double) noexcept;
template void TracingSpan::set_attribute<const char*>(const char*, const char*) noexcept;
template void TracingSpan::set_attribute<std::string>(const char*, std::string) noexcept;

void TracingSpan::add_event(const char* name) noexcept {
    impl_->span->AddEvent(name);
}

void TracingSpan::set_status_ok() noexcept {
    impl_->span->SetStatus(otel_trace::StatusCode::kOk);
}

void TracingSpan::set_status_error(const char* description) noexcept {
    impl_->span->SetStatus(otel_trace::StatusCode::kError, description);
}

void TracingSpan::end() noexcept {
    impl_->span->End();
}

}  // namespace sdk
}  // namespace viam

#else  // VIAMCPPSDK_OPENTELEMETRY_TRACING — no-op implementations

namespace viam {
namespace sdk {

struct TracingSpan::Impl {};

TracingSpan::TracingSpan(const char*) noexcept {}
TracingSpan::~TracingSpan() noexcept = default;

template <typename T>
void TracingSpan::set_attribute(const char*, T) noexcept {}

template void TracingSpan::set_attribute<bool>(const char*, bool) noexcept;
template void TracingSpan::set_attribute<std::int64_t>(const char*, std::int64_t) noexcept;
template void TracingSpan::set_attribute<double>(const char*, double) noexcept;
template void TracingSpan::set_attribute<const char*>(const char*, const char*) noexcept;
template void TracingSpan::set_attribute<std::string>(const char*, std::string) noexcept;

void TracingSpan::add_event(const char*) noexcept {}
void TracingSpan::set_status_ok() noexcept {}
void TracingSpan::set_status_error(const char*) noexcept {}
void TracingSpan::end() noexcept {}

}  // namespace sdk
}  // namespace viam

#endif  // VIAMCPPSDK_OPENTELEMETRY_TRACING
