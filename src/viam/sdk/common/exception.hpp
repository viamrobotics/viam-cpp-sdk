#pragma once
#include <grpc/status.h>
#include <grpcpp/impl/codegen/status.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace viam {
namespace sdk {

enum class ViamErrorCode : uint8_t {
    // Used to indicate no error
    ok = 0,
    unknown = 1,
    permission_denied = 2,
    duplicate_resource = 3,
    unimplemented = 4,
    validation = 5,
    connection = 6
};

class ViamException : public std::runtime_error {
   public:
    explicit ViamException(const std::string& what = "unknown");

    explicit ViamException(const std::string& type,
                           const ViamErrorCode& code,
                           const std::string& what);

    explicit ViamException(const ::grpc::Status& status_code);

    static ViamException from_viam_error_code(ViamErrorCode code);

    virtual ~ViamException();

    ViamErrorCode get_error_code() const noexcept;

   private:
    ViamErrorCode error_code_;
};

class PermissionDeniedException : public ViamException {
   public:
    explicit PermissionDeniedException(const std::string& what = "unknown");
};

class DuplicateResourceException : public ViamException {
   public:
    explicit DuplicateResourceException(const std::string& what = "unknown");
};

class UnimplementedException : public ViamException {
   public:
    explicit UnimplementedException(const std::string& what = "unknown");
};

class ValidationException : public ViamException {
   public:
    explicit ValidationException(const std::string& what = "unknown");
};

class ConnectionException : public ViamException {
   public:
    explicit ConnectionException(const std::string& what = "unknown");
};
}  // namespace sdk
}  // namespace viam
