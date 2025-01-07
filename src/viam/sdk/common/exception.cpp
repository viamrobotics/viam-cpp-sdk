#include <viam/sdk/common/exception.hpp>

#include <grpcpp/support/status.h>

namespace viam {
namespace sdk {

Exception::Exception(ErrorCondition condition, const std::string& what)
    : std::runtime_error("viam::sdk::Exception: " + what), condition_(condition) {};

Exception::Exception(const std::string& what) : Exception(ErrorCondition::k_general, what) {};

const std::error_condition& Exception::condition() const noexcept {
    return condition_;
};

std::error_condition make_error_condition(ErrorCondition e) {
    struct ErrorCategory : std::error_category {
        const char* name() const noexcept override {
            return "viam::sdk";
        };
        std::string message(int ev) const override {
            switch (static_cast<ErrorCondition>(ev)) {
                case ErrorCondition::k_general:
                    return "general";
                case ErrorCondition::k_connection:
                    return "connection establishment failure";
                case ErrorCondition::k_duplicate_registration:
                    return "duplicate registration";
                case ErrorCondition::k_duplicate_resource:
                    return "duplicate resource";
                case ErrorCondition::k_grpc:
                    return "gRPC";
                case ErrorCondition::k_not_supported:
                    return "not supported";
                case ErrorCondition::k_resource_not_found:
                    return "resource not found";
                default:
                    return "unknown";
            }
        };
    };

    static const ErrorCategory errorCategory{};
    return {static_cast<int>(e), errorCategory};
}

GRPCException::GRPCException(const grpc::Status* status)
    : Exception(ErrorCondition::k_grpc, status->error_message()),
      status_(std::make_shared<grpc::Status>(*status)) {}

const grpc::Status* GRPCException::status() const noexcept {
    return status_.get();
}

}  // namespace sdk
}  // namespace viam
