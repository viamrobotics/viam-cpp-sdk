#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {

ViamException::ViamException(const std::string& what)
    : std::runtime_error("ViamException: " + what), code_(ViamErrorCode::Unknown){};

ViamException::ViamException(const std::string& what, ViamErrorCode code)
    : std::runtime_error("ViamException: " + what), code_(code){};

ViamException::~ViamException() = default;

std::error_code ViamException::code() const noexcept {
    return code_;
};

struct ViamErrorCategory : std::error_category {
    const char* name() const noexcept override;
    std::string message(int ev) const override;
};

const char* ViamErrorCategory::name() const noexcept {
    return "viam";
}

std::string ViamErrorCategory::message(int ev) const {
    switch (static_cast<ViamErrorCode>(ev)) {
        case ViamErrorCode::Unknown:
            return "unknown";
        case ViamErrorCode::Connection:
            return "connection establishment failure";
        case ViamErrorCode::DuplicateRegistration:
            return "duplicate registration";
        case ViamErrorCode::DuplicateResource:
            return "duplicate resource";
        case ViamErrorCode::GRPC:
            return "gRPC";
        case ViamErrorCode::NotSupported:
            return "not supported";
        case ViamErrorCode::ResourceNotFound:
            return "resource not found";
        default:
            return "unknown";
    }
}

const ViamErrorCategory viamErrorCategory{};

std::error_code make_error_code(ViamErrorCode e) {
    return {static_cast<int>(e), viamErrorCategory};
}

}  // namespace sdk
}  // namespace viam
