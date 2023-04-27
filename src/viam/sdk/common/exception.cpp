#include <grpc/status.h>
#include <viam/sdk/common/exception.hpp>
namespace viam {
namespace sdk {

ViamException::ViamException(const std::string& what)
    : std::runtime_error("ViamException: " + what), _error_code(ViamErrorCode::generic){};

ViamException::ViamException(const std::string& type,
                             const ViamErrorCode& code,
                             const std::string& what)
    : std::runtime_error("ViamException(" + type + "): " + what), _error_code(code){};

ViamException::~ViamException() = default;
ViamException::ViamException(const ::grpc::Status& status_code)

    : ViamException(status_code.error_message() + " " + status_code.error_details()) {}

ViamException ViamException::from_viam_error_code(ViamErrorCode code) {
    switch (code) {
        case ViamErrorCode::ok: {
            return ViamException(
                "Ran ViamException::from_viam_error_code on an \'ok\' value. Check the code "
                "value "
                "first.");
        }
        case ViamErrorCode::permission_denied: {
            return PermissionDeniedException();
        }
        case ViamErrorCode::duplicate_resource: {
            return DuplicateResourceException();
        }
        case ViamErrorCode::unimplemented: {
            return UnimplementedException();
        }
        case ViamErrorCode::validation: {
            return ValidationException();
        }
        case ViamErrorCode::connection: {
            return ConnectionException();
        }
        case ViamErrorCode::generic:
        default: {
            return ViamException();
        }
    }
}

ViamErrorCode ViamException::get_error_code() const noexcept {
    return _error_code;
}

PermissionDeniedException::PermissionDeniedException(const std::string& what)
    : ViamException("PermissionDenied", ViamErrorCode::permission_denied, what){};

DuplicateResourceException::DuplicateResourceException(const std::string& what)
    : ViamException("DuplicateResource", ViamErrorCode::duplicate_resource, what){};

UnimplementedException::UnimplementedException(const std::string& what)
    : ViamException("Unimplemented", ViamErrorCode::unimplemented, what){};

ValidationException::ValidationException(const std::string& what)
    : ViamException("Validation", ViamErrorCode::validation, what){};

ConnectionException::ConnectionException(const std::string& what)
    : ViamException("Connection", ViamErrorCode::connection, what){};

}  // namespace sdk
}  // namespace viam
