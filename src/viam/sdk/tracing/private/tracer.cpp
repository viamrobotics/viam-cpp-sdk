#include <viam/sdk/tracing/private/tracer.hpp>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/private/instance.hpp>

#ifdef VIAMCPPSDK_OPENTELEMETRY_TRACING

#include <memory>

#include <opentelemetry/context/propagation/global_propagator.h>
#include <opentelemetry/exporters/otlp/otlp_grpc_exporter_factory.h>
#include <opentelemetry/exporters/otlp/otlp_grpc_exporter_options.h>
#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/sdk/trace/batch_span_processor_factory.h>
#include <opentelemetry/sdk/trace/batch_span_processor_options.h>
#include <opentelemetry/sdk/trace/tracer_provider.h>
#include <opentelemetry/sdk/trace/tracer_provider_factory.h>
#include <opentelemetry/trace/noop.h>
#include <opentelemetry/trace/propagation/http_trace_context.h>
#include <opentelemetry/trace/provider.h>

namespace otel_prop = opentelemetry::context::propagation;
namespace otel_trace = opentelemetry::trace;
namespace otel_otlp = opentelemetry::exporter::otlp;
namespace otel_sdk_resource = opentelemetry::sdk::resource;
namespace otel_sdk_trace = opentelemetry::sdk::trace;

namespace viam {
namespace sdk {
namespace impl {

namespace {

constexpr const char* k_instrumentation_scope = "viam-cpp-sdk";

}  // namespace

void initialize_trace_propagator() noexcept {
    otel_prop::GlobalTextMapPropagator::SetGlobalPropagator(
        opentelemetry::nostd::shared_ptr<otel_prop::TextMapPropagator>(
            new opentelemetry::trace::propagation::HttpTraceContext()));
}

// Holds the SDK provider so Shutdown() can be called explicitly during teardown — the
// global Provider only exposes the abstract trace::TracerProvider interface.
struct Tracer::Impl {
    std::shared_ptr<otel_sdk_trace::TracerProvider> sdk_provider;
};

Tracer::Tracer() : impl_(std::make_unique<Impl>()) {}
Tracer::~Tracer() {
    shutdown_provider();
}

Tracer& Tracer::get() {
    return Instance::current(Instance::Creation::open_existing).impl_->tracer;
}

void Tracer::initialize_provider(const std::string& endpoint) noexcept {
    shutdown_provider();

    // the incoming endpoint is unix:/path/to/socket but opentel's url parser expects
    // a :// for protocol
    std::string ep = "unix://" + endpoint.substr(endpoint.find('/'));

    otel_otlp::OtlpGrpcExporterOptions exporter_opts;
    exporter_opts.endpoint = ep;
    auto exporter = otel_otlp::OtlpGrpcExporterFactory::Create(exporter_opts);

    auto processor = otel_sdk_trace::BatchSpanProcessorFactory::Create(
        std::move(exporter), otel_sdk_trace::BatchSpanProcessorOptions{});

    auto resource = otel_sdk_resource::Resource::Create({
        {"service.name", std::string{k_instrumentation_scope}},
    });

    impl_->sdk_provider = std::shared_ptr<otel_sdk_trace::TracerProvider>(
        otel_sdk_trace::TracerProviderFactory::Create(std::move(processor), resource));

    std::shared_ptr<otel_trace::TracerProvider> base_provider = impl_->sdk_provider;
    otel_trace::Provider::SetTracerProvider(
        opentelemetry::nostd::shared_ptr<otel_trace::TracerProvider>(std::move(base_provider)));
}

void Tracer::shutdown_provider() noexcept {
    if (!impl_->sdk_provider) {
        return;
    }
    impl_->sdk_provider->Shutdown();
    otel_trace::Provider::SetTracerProvider(
        opentelemetry::nostd::shared_ptr<otel_trace::TracerProvider>(
            new otel_trace::NoopTracerProvider()));
    impl_->sdk_provider.reset();
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam

#else  // VIAMCPPSDK_OPENTELEMETRY_TRACING — provide no-op implementations

namespace viam {
namespace sdk {
namespace impl {

void initialize_trace_propagator() noexcept {}

struct Tracer::Impl {};

Tracer::Tracer() = default;
Tracer::~Tracer() = default;

Tracer& Tracer::get() {
    return Instance::current(Instance::Creation::open_existing).impl_->tracer;
}

void Tracer::initialize_provider(const std::string&) noexcept {}
void Tracer::shutdown_provider() noexcept {}

}  // namespace impl
}  // namespace sdk
}  // namespace viam

#endif  // VIAMCPPSDK_OPENTELEMETRY_TRACING
