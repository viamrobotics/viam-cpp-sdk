#include <viam/sdk/tracing/private/tracer.hpp>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/private/instance.hpp>
#include <viam/sdk/common/utils.hpp>

#ifdef VIAMCPPSDK_OPENTELEMETRY_TRACING

#include <atomic>
#include <chrono>
#include <memory>
#include <utility>

#include <opentelemetry/context/propagation/global_propagator.h>
#include <opentelemetry/exporters/otlp/otlp_recordable.h>
#include <opentelemetry/exporters/otlp/otlp_recordable_utils.h>
#include <opentelemetry/proto/collector/trace/v1/trace_service.pb.h>
#include <opentelemetry/sdk/common/exporter_utils.h>
#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/sdk/trace/batch_span_processor_factory.h>
#include <opentelemetry/sdk/trace/batch_span_processor_options.h>
#include <opentelemetry/sdk/trace/exporter.h>
#include <opentelemetry/sdk/trace/recordable.h>
#include <opentelemetry/sdk/trace/tracer_provider.h>
#include <opentelemetry/sdk/trace/tracer_provider_factory.h>
#include <opentelemetry/trace/noop.h>
#include <opentelemetry/trace/propagation/http_trace_context.h>
#include <opentelemetry/trace/provider.h>

#include <viam/api/robot/v1/robot.pb.h>
#include <viam/sdk/robot/client.hpp>

namespace otel_common = opentelemetry::sdk::common;
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

// Ships OTLP-encoded spans to the parent process via RobotClient::send_traces.
class ParentSendTracesExporter final : public otel_sdk_trace::SpanExporter {
   public:
    explicit ParentSendTracesExporter(RobotClient* client) noexcept : client_(client) {}

    std::unique_ptr<otel_sdk_trace::Recordable> MakeRecordable() noexcept override {
        return std::unique_ptr<otel_sdk_trace::Recordable>(new otel_otlp::OtlpRecordable());
    }

    otel_common::ExportResult Export(
        const opentelemetry::nostd::span<std::unique_ptr<otel_sdk_trace::Recordable>>&
            spans) noexcept override {
        if (is_shutdown_.load(std::memory_order_acquire)) {
            return otel_common::ExportResult::kFailure;
        }
        if (spans.empty()) {
            return otel_common::ExportResult::kSuccess;
        }

        opentelemetry::proto::collector::trace::v1::ExportTraceServiceRequest otlp_req;
        otel_otlp::OtlpRecordableUtils::PopulateRequest(spans, &otlp_req);

        viam::robot::v1::SendTracesRequest req;
        req.mutable_resource_spans()->Swap(otlp_req.mutable_resource_spans());

        return client_->send_traces(&req) ? otel_common::ExportResult::kSuccess
                                          : otel_common::ExportResult::kFailure;
    }

    bool ForceFlush(std::chrono::microseconds /*timeout*/) noexcept override {
        return true;
    }

    bool Shutdown(std::chrono::microseconds /*timeout*/) noexcept override {
        is_shutdown_.store(true, std::memory_order_release);
        return true;
    }

   private:
    RobotClient* client_;
    std::atomic<bool> is_shutdown_{false};
};

void Tracer::initialize_propagator() noexcept {
    otel_prop::GlobalTextMapPropagator::SetGlobalPropagator(
        opentelemetry::nostd::shared_ptr<otel_prop::TextMapPropagator>(
            new opentelemetry::trace::propagation::HttpTraceContext()));
}

Tracer::Tracer() = default;
Tracer::~Tracer() {
    shutdown_provider();
}

Tracer& Tracer::get() {
    return Instance::current(Instance::Creation::open_existing).impl_->tracer;
}

void Tracer::initialize_provider(RobotClient* client) noexcept {
    shutdown_provider();

    if (is_env_var_false("VIAM_MODULE_TRACING")) {
        return;
    }

    auto exporter =
        std::unique_ptr<otel_sdk_trace::SpanExporter>(new ParentSendTracesExporter(client));

    auto processor = otel_sdk_trace::BatchSpanProcessorFactory::Create(
        std::move(exporter), otel_sdk_trace::BatchSpanProcessorOptions{});

    auto resource = otel_sdk_resource::Resource::Create({
        {"service.name", std::string{k_instrumentation_scope}},
    });

    sdk_provider_ = std::shared_ptr<otel_sdk_trace::TracerProvider>(
        otel_sdk_trace::TracerProviderFactory::Create(std::move(processor), resource));

    std::shared_ptr<otel_trace::TracerProvider> base_provider = sdk_provider_;
    otel_trace::Provider::SetTracerProvider(
        opentelemetry::nostd::shared_ptr<otel_trace::TracerProvider>(std::move(base_provider)));
}

void Tracer::shutdown_provider() noexcept {
    if (!sdk_provider_) {
        return;
    }
    sdk_provider_->Shutdown();
    otel_trace::Provider::SetTracerProvider(
        opentelemetry::nostd::shared_ptr<otel_trace::TracerProvider>(
            new otel_trace::NoopTracerProvider()));
    sdk_provider_.reset();
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam

#else  // VIAMCPPSDK_OPENTELEMETRY_TRACING — provide no-op implementations

namespace viam {
namespace sdk {

class RobotClient;

namespace impl {

void Tracer::initialize_propagator() noexcept {}

Tracer::Tracer() = default;
Tracer::~Tracer() = default;

Tracer& Tracer::get() {
    return Instance::current(Instance::Creation::open_existing).impl_->tracer;
}

void Tracer::initialize_provider(RobotClient*) noexcept {}
void Tracer::shutdown_provider() noexcept {}

}  // namespace impl
}  // namespace sdk
}  // namespace viam

#endif  // VIAMCPPSDK_OPENTELEMETRY_TRACING
