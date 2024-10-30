#include <viam/sdk/common/client_helper.hpp>

#include <cstdlib>

#include <viam/sdk/common/logger.hpp>

namespace viam {
namespace sdk {

namespace client_helper_details {

[[noreturn]] void errorHandlerReturnedUnexpectedly(const ::grpc::Status& status) noexcept {
    VIAM_SDK_TRIVIAL_CUSTOM_FORMATTED_LOG(
        log_level::fatal,
        "ClientHelper error handler callback returned instead of throwing: "
            << status.error_message() << '(' << status.error_details() << ')');
    std::abort();
}

}  // namespace client_helper_details
}  // namespace sdk
}  // namespace viam
