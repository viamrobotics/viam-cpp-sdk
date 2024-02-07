#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {

Exception::Exception(const std::string& what)
    : std::runtime_error("ViamException: " + what), condition_(ErrorCondition::k_general){};

Exception::Exception(ErrorCondition condition, const std::string& what)
    : std::runtime_error("ViamException: " + what), condition_(condition){};

Exception::~Exception() = default;

std::error_condition Exception::condition() const noexcept {
    return condition_;
};

struct ErrorCategory : std::error_category {
    const char* name() const noexcept override;
    std::string message(int ev) const override;
};

const char* ErrorCategory::name() const noexcept {
    return "viam::sdk";
}

std::string ErrorCategory::message(int ev) const {
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
}

std::error_condition make_error_condition(ErrorCondition e) {
    static const ErrorCategory errorCategory{};
    return {static_cast<int>(e), errorCategory};
}

GRPCException::GRPCException(grpc::Status status)
    : Exception(ErrorCondition::k_grpc, status.error_message()), status_(std::move(status)){};

grpc::Status GRPCException::status() const noexcept {
    return status_;
}

}  // namespace sdk
}  // namespace viam
