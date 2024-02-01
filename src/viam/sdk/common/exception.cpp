#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {

ViamException::ViamException(const std::string& what)
    : std::runtime_error("ViamException: " + what){};

ViamException::ViamException(const std::string& what, const std::string& type)
    : std::runtime_error("ViamException(" + type + "): " + what){};

ViamException::~ViamException() = default;

ConnectionException::ConnectionException(const std::string& what)
    : ViamException(what, "Connection"){};

DuplicateRegistrationException::DuplicateRegistrationException(const std::string& what)
    : ViamException(what, "DuplicateRegistration"){};

DuplicateResourceException::DuplicateResourceException(const std::string& what)
    : ViamException(what, "DuplicateResource"){};

GRPCException::GRPCException(const std::string& what, int grpc_error_code)
    : ViamException(what, "gRPC"), grpc_error_code_(std::move(grpc_error_code)){};

int GRPCException::error_code() const noexcept {
    return grpc_error_code_;
}

NotSupportedException::NotSupportedException(const std::string& what)
    : ViamException(what, "NotSupported"){};

ResourceNotFoundException::ResourceNotFoundException(const std::string& what)
    : ViamException(what, "ResourceNotFound"){};

}  // namespace sdk
}  // namespace viam
