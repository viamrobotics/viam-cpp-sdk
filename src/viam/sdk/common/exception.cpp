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

DuplicateResourceException::DuplicateResourceException(const std::string& what, Name resource_name)
    : ViamException(what, "DuplicateResource"), resource_name_(std::move(resource_name)){};

Name DuplicateResourceException::resource_name() const noexcept {
    return resource_name_;
}

GRPCException::GRPCException(const std::string& what, int grpc_error_code)
    : ViamException(what, "gRPC"), grpc_error_code_(std::move(grpc_error_code)){};

int GRPCException::error_code() const noexcept {
    return grpc_error_code_;
}

ResourceNotFoundException::ResourceNotFoundException(const std::string& what, Name resource_name)
    : ViamException(what, "ResourceNotFound"), resource_name_(std::move(resource_name)){};

Name ResourceNotFoundException::resource_name() const noexcept {
    return resource_name_;
}

}  // namespace sdk
}  // namespace viam
