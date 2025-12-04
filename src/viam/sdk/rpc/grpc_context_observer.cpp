#include <viam/sdk/rpc/grpc_context_observer.hpp>

#include <grpcpp/server_context.h>

#include <stdexcept>

#include <viam/sdk/rpc/private/grpc_context_observer.hpp>

namespace viam {
namespace sdk {

namespace {
thread_local boost::optional<const GrpcContextObserver> current_context{};
}  // namespace

const boost::optional<const GrpcContextObserver>& GrpcContextObserver::current() noexcept {
    return current_context;
}

const GrpcServerContext& GrpcContextObserver::context() const noexcept {
    return context_;
}

std::vector<std::string> GrpcContextObserver::get_client_metadata_field_values(
    const std::string& key) const {
    std::vector<std::string> values;
    const auto& metadata = context().client_metadata();
    auto range = metadata.equal_range(grpc::string_ref(key));
    for (auto it = range.first; it != range.second; ++it) {
        values.emplace_back(it->second.data(), it->second.size());
    }
    return values;
}

GrpcContextObserver::Enable::Enable(const GrpcServerContext& context) {
    if (current_context) {
        throw std::runtime_error("Cannot replace existing GrpcContextObserver with a new one");
    }
    current_context.emplace(GrpcContextObserver{context});
}

GrpcContextObserver::Enable::~Enable() {
    current_context.reset();
}

GrpcContextObserver::GrpcContextObserver(const GrpcServerContext& context) noexcept
    : context_{context} {}

}  // namespace sdk
}  // namespace viam
