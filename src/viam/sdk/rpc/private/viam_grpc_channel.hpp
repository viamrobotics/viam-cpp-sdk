#pragma once

#include <memory>

#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>

namespace viam {
namespace sdk {
namespace impl {

/// @brief Like grpc::CreateChannel, but returns a channel suitable for transmitting messages of
/// size kMaxMessageSize.
std::shared_ptr<grpc::Channel> create_viam_grpc_channel(
    const grpc::string& target, const std::shared_ptr<grpc::ChannelCredentials>& credentials);

/// @brief Like grpc::CreateChannel, but for the express purpose of returning a channel for making
/// an AuthService request.
std::shared_ptr<grpc::Channel> create_viam_auth_channel(const std::string& address);

}  // namespace impl
}  // namespace sdk
}  // namespace viam
