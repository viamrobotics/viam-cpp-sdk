#include <viam/sdk/rpc/private/viam_grpc_channel.hpp>

#include <grpcpp/create_channel.h>
#include <grpcpp/support/channel_arguments.h>

#include <viam/sdk/rpc/message_sizes.hpp>

namespace viam {
namespace sdk {
namespace impl {

std::shared_ptr<grpc::Channel> create_viam_auth_channel(const std::string& address) {
    grpc::experimental::TlsChannelCredentialsOptions opts;
    opts.set_check_call_host(false);
    auto tls_creds = grpc::experimental::TlsCredentials(opts);

    return grpc::CreateChannel(address, tls_creds);
}

std::shared_ptr<grpc::Channel> create_viam_channel(
    const grpc::string& target, const std::shared_ptr<grpc::ChannelCredentials>& credentials) {
    grpc::ChannelArguments args;
    args.SetMaxSendMessageSize(kMaxMessageSize);
    args.SetMaxReceiveMessageSize(kMaxMessageSize);

    return grpc::CreateCustomChannel(target, credentials, args);
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
