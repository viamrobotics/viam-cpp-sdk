#include <viam/sdk/common/service_helper.hpp>

namespace viam {
namespace sdk {

::grpc::Status ServiceHelperBase::fail(::grpc::StatusCode code, const char* message) const noexcept
    try {
    std::ostringstream stream;
    stream << '[' << method_ << "]: " << message;
    return {code, stream.str()};
} catch (...) {
    return {code, message};
}

::grpc::Status ServiceHelperBase::failNoRequest() const noexcept {
    return fail(::grpc::INVALID_ARGUMENT, "Called without a `request` object");
}

::grpc::Status ServiceHelperBase::failNoResource(const std::string& name) const noexcept try {
    std::ostringstream stream;
    stream << "Failed to find resource `" << name << "`";
    return fail(::grpc::INVALID_ARGUMENT, stream.str().c_str());
} catch (...) {
    return fail(::grpc::INVALID_ARGUMENT, "Failed to find resource");
}

::grpc::Status ServiceHelperBase::failStdException(const std::exception& xcp) const noexcept try {
    std::ostringstream stream;
    stream << "Failed with a std::exception: " << xcp.what();
    return fail(::grpc::INTERNAL, stream.str().c_str());
} catch (...) {
    return fail(::grpc::INTERNAL, "Failed with a std::exception: <unknown>");
}

::grpc::Status ServiceHelperBase::failUnknownException() const noexcept {
    return fail(::grpc::INTERNAL, "Failed with an unknown exception");
}

}  // namespace sdk
}  // namespace viam
