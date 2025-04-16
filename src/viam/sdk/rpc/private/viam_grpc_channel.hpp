#pragma once

#include <memory>

#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>

namespace viam {
namespace sdk {
namespace impl {

/// @brief Like grpc::CreateChannel, but returns a channel suitable for transmitting messages of
/// size kMaxMessageSize.
std::shared_ptr<grpc::Channel> create_viam_channel(
    const grpc::string& target, const std::shared_ptr<grpc::ChannelCredentials>& credentials);
std::shared_ptr<grpc::Channel> create_viam_channel(
    const grpc::string& target,
    const std::shared_ptr<grpc::ChannelCredentials>& credentials,
    const std::string& token);
std::shared_ptr<grpc::Channel> create_viam_auth_channel();

}  // namespace impl
}  // namespace sdk
}  // namespace viam
