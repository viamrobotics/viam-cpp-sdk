/// @file common/exception.hpp
///
/// @brief Defines custom exceptions for the SDK.
#pragma once
#include <grpcpp/support/status.h>
#include <stdexcept>
#include <string>

#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

/// @defgroup Exception Classes related to SDK exceptions.

/// @class ErrorCondition
/// @brief Defines a set of a error conditions to be used in conjunction with
/// Exception.
/// @ingroup Exception
enum class ErrorCondition : uint8_t {
    k_general = 0,                 // Default condition
    k_connection = 1,              // Issue during connection establishment
    k_duplicate_registration = 2,  // API or API/Model pair has already been registered
    k_duplicate_resource = 3,      // Resource has already been added
    k_grpc = 4,                    // gRPC error from remote machine
    k_not_supported = 5,           // Behavior not supported by the SDK
    k_resource_not_found = 6       // Resource does not exist
};

std::error_condition make_error_condition(ErrorCondition e);

/// @class Exception
/// @brief Defines an exception type for the SDK.
/// @ingroup Exception
class Exception : public std::runtime_error {
   public:
    explicit Exception(ErrorCondition condition, const std::string& what);
    explicit Exception(const std::string& what);
    virtual ~Exception();

    const std::error_condition& condition() const noexcept;

   private:
    std::error_condition condition_;
};

/// @class GRPCException
/// @brief Defines an exception from a gRPC interaction.
/// @ingroup Exception
class GRPCException : public Exception {
   public:
    explicit GRPCException(grpc::Status status);

    const grpc::Status& status() const noexcept;

   private:
    grpc::Status status_;
};

}  // namespace sdk
}  // namespace viam

namespace std {
template <>
struct is_error_condition_enum<viam::sdk::ErrorCondition> : true_type {};
}  // namespace std
