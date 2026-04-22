#pragma once

#include <memory>
#include <string>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>

#include <viam/api/app/v1/app.grpc.pb.h>
#include <viam/api/app/v1/app.pb.h>

#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {

/// @struct OAuthAppUser
/// @brief Represents an OAuth application user.
struct OAuthAppUser {
    std::string auth_token;
    std::string registration_id;
    std::string user_id;
    std::string refresh_token;
};

/// @class AppClient
/// @brief gRPC client for the AppService.
class AppClient {
   public:
    explicit AppClient(std::shared_ptr<grpc::Channel> channel);

    /// @brief Create an OAuth application user.
    /// @param org_id The organization ID.
    /// @param application_id The application ID.
    /// @param email The user's email.
    /// @param first_name The user's first name.
    /// @param last_name The user's last name.
    /// @param password The user's password.
    /// @return An OAuthAppUser object containing authentication details.
    OAuthAppUser create_oauth_app_user(const std::string& org_id,
                                       const std::string& application_id,
                                       const std::string& email,
                                       const std::string& first_name,
                                       const std::string& last_name,
                                       const std::string& password);

   private:
    std::unique_ptr<viam::app::v1::AppService::Stub> stub_;
};

}  // namespace sdk
}  // namespace viam
