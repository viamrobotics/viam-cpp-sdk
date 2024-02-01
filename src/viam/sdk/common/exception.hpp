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

/// @class ViamException
/// @brief Defines a base exception type to be inherited from by other more
/// specific exceptions.
/// @ingroup Exception
class ViamException : public std::runtime_error {
   public:
    explicit ViamException(const std::string& what);
    explicit ViamException(const std::string& what, const std::string& type);
    virtual ~ViamException();
};

/// @class ConnectionException
/// @brief An exception thrown when an SDK client cannot create a connection to
/// a machine.
/// @ingroup Exception
class ConnectionException : public ViamException {
   public:
    explicit ConnectionException(const std::string& what);
};

/// @class ConnectionException
/// @brief An exception thrown when a resource has already been added to a
/// `ResourceManager`.
/// @ingroup Exception
class DuplicateResourceException : public ViamException {
   public:
    explicit DuplicateResourceException(const std::string& what, Name resource_name);

    /// @brief Returns the resource `Name` that was a duplicate.
    /// @return The resource `Name` that was a duplicate.
    Name resource_name() const noexcept;

   private:
    Name resource_name_;
};

/// @class ConnectionException
/// @brief An exception thrown when a machine has sent an error back to an SDK
/// client over gRPC.
/// @ingroup Exception
class GRPCException : public ViamException {
   public:
    explicit GRPCException(const std::string& what, int grpc_error_code);

    int error_code() const noexcept;

   private:
    int grpc_error_code_;
};

/// @class ConnectionException
/// @brief An exception thrown when a resource cannot be found in a manager.
/// @ingroup Exception
class ResourceNotFoundException : public ViamException {
   public:
    explicit ResourceNotFoundException(const std::string& what, Name resource_name);

    /// @brief Returns the resource `Name` that was not found.
    /// @return The resource `Name` that was not found.
    Name resource_name() const noexcept;

   private:
    Name resource_name_;
};

}  // namespace sdk
}  // namespace viam
