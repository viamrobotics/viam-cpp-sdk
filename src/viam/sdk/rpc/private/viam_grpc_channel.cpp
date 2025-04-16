#include <viam/sdk/rpc/private/viam_grpc_channel.hpp>

#include <grpcpp/create_channel.h>
#include <grpcpp/support/channel_arguments.h>

#include <viam/sdk/rpc/message_sizes.hpp>

namespace viam {
namespace sdk {
namespace impl {

std::shared_ptr<grpc::Channel> create_viam_auth_channel() {
    grpc::experimental::TlsChannelCredentialsOptions opts;
    opts.set_verify_server_certs(false);
    opts.set_check_call_host(false);
    auto tls_creds = grpc::experimental::TlsCredentials(opts);
    std::cout << "setting ssl target name override\n";
    grpc::ChannelArguments args;
    args.SetMaxSendMessageSize(kMaxMessageSize);
    args.SetMaxReceiveMessageSize(kMaxMessageSize);
    // args.SetSslTargetNameOverride("34.149.207.107");
    // args.SetSslTargetNameOverride("app.viam.com");

    return grpc::CreateCustomChannel("app.viam.com", tls_creds, args);
}

std::shared_ptr<grpc::Channel> create_viam_channel(
    const grpc::string& target, const std::shared_ptr<grpc::ChannelCredentials>& credentials) {
    return create_viam_channel(target, credentials, "");
}

std::shared_ptr<grpc::Channel> create_viam_channel(
    const grpc::string& target,
    const std::shared_ptr<grpc::ChannelCredentials>& credentials,
    const std::string& token) {
    grpc::ChannelArguments args;
    args.SetMaxSendMessageSize(kMaxMessageSize);
    args.SetMaxReceiveMessageSize(kMaxMessageSize);

    if (!token.empty()) {
        args.SetString("authorization:", "Bearer " + token);
    }
    grpc::experimental::TlsChannelCredentialsOptions opts;
    opts.set_verify_server_certs(false);
    opts.set_check_call_host(false);
    auto tls_creds = grpc::experimental::TlsCredentials(opts);

    return grpc::CreateCustomChannel(target, tls_creds, args);
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
