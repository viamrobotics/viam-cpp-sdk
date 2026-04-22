#include "viam/sdk/app/app_client.hpp"

#include <grpcpp/grpcpp.h>

#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {

AppClient::AppClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(viam::app::v1::AppService::NewStub(channel)) {}

OAuthAppUser AppClient::create_oauth_app_user(const std::string& org_id,
                                              const std::string& application_id,
                                              const std::string& email,
                                              const std::string& first_name,
                                              const std::string& last_name,
                                              const std::string& password) {
    grpc::ClientContext context;
    viam::app::v1::CreateOAuthAppUserRequest request;
    viam::app::v1::CreateOAuthAppUserResponse response;

    request.set_org_id(org_id);
    request.set_application_id(application_id);
    request.set_email(email);
    request.set_first_name(first_name);
    request.set_last_name(last_name);
    request.set_password(password);

    grpc::Status status = stub_->CreateOAuthAppUser(&context, request, &response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }

    OAuthAppUser oauth_app_user;
    oauth_app_user.auth_token = response.auth_token();
    oauth_app_user.registration_id = response.registration_id();
    oauth_app_user.user_id = response.user_id();
    oauth_app_user.refresh_token = response.refresh_token();

    return oauth_app_user;
}

}  // namespace sdk
}  // namespace viam
