#include <viam/sdk/common/client_helper.hpp>

#include <cstdlib>

#include <grpcpp/client_context.h>
#include <grpcpp/support/status.h>

#include <viam/sdk/common/private/version_metadata.hpp>
#include <viam/sdk/log/logging.hpp>

namespace viam {
namespace sdk {

namespace client_helper_details {

[[noreturn]] void errorHandlerReturnedUnexpectedly(const ::grpc::Status* status) noexcept {
    VIAM_SDK_LOG(fatal) << "ClientHelper error handler callback returned instead of throwing: "
                        << status->error_message() << '(' << status->error_details() << ')';
    std::abort();
}

bool isStatusCancelled(int status) noexcept {
    return status == ::grpc::StatusCode::CANCELLED;
}

void set_name(...) {}

}  // namespace client_helper_details

ClientContext::ClientContext() : wrapped_context_(std::make_unique<GrpcClientContext>()) {
    set_client_ctx_authority_();
    add_viam_client_version_();
}

ClientContext::~ClientContext() = default;

ClientContext::operator const GrpcClientContext*() const {
    return wrapped_context_.get();
}

ClientContext::operator GrpcClientContext*() {
    return wrapped_context_.get();
}

void ClientContext::try_cancel() {
    wrapped_context_->TryCancel();
}

void ClientContext::set_debug_key(const std::string& debug_key) {
    wrapped_context_->AddMetadata("dtname", debug_key);
}

void ClientContext::set_client_ctx_authority_() {
    wrapped_context_->set_authority("viam-placeholder");
}

void ClientContext::add_viam_client_version_() {
    wrapped_context_->AddMetadata("viam_client", impl::k_version);
}

}  // namespace sdk
}  // namespace viam
