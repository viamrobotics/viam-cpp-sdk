/// @file common/exception.hpp
///
/// @brief Defines custom exceptions for the SDK.
#pragma once
#include <stdexcept>
#include <string>

#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

/// @defgroup Exception Classes related to SDK exceptions.

/// @class ViamErrorCode
/// @brief Defines a set of a error codes to be used in conjunction with
/// ViamException.
/// @ingroup Exception
enum class ViamErrorCode {
    Unknown = 0,            // Default code
    Connection,             // Issue during connection establishment
    DuplicateRegistration,  // API or API/Model pair has already been registered
    DuplicateResource,      // Resource has already been added
    GRPC,                   // gRPC error from remote machine
    NotSupported,           // Behavior not supported by the SDK
    ResourceNotFound        // Resource does not exist
};

std::error_code make_error_code(ViamErrorCode e);

/// @class ViamException
/// @brief Defines an exception type for the SDK.
/// @ingroup Exception
class ViamException : public std::runtime_error {
   public:
    explicit ViamException(const std::string& what);
    explicit ViamException(const std::string& what, ViamErrorCode code);
    virtual ~ViamException();

    std::error_code code() const noexcept;

   private:
    std::error_code code_;
};

}  // namespace sdk
}  // namespace viam

namespace std {
template <>
struct is_error_code_enum<viam::sdk::ViamErrorCode> : true_type {};
}  // namespace std
