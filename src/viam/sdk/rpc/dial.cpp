#include <grpc/grpc.h>
#include <viam/sdk/rpc/dial.hpp>

#include <istream>
#include <string>

#include <boost/config.hpp>
#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/utility/string_view.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/api/proto/rpc/v1/auth.grpc.pb.h>
#include <viam/api/proto/rpc/v1/auth.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>
#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/rpc/private/viam_grpc_channel.hpp>
#include <viam/sdk/rpc/private/viam_rust_utils.h>

namespace viam {
namespace sdk {

const std::shared_ptr<grpc::Channel>& ViamChannel::channel() const {
    return channel_;
}

void ViamChannel::close() {
    if (closed_) {
        return;
    }
    closed_ = true;
    free_string(path_);
    free_rust_runtime(rust_runtime_);
};

const std::string& Credentials::type() const {
    return type_;
}

const std::string& Credentials::payload() const {
    return payload_;
}

ViamChannel::ViamChannel(std::shared_ptr<grpc::Channel> channel, const char* path, void* runtime)
    : channel_(std::move(channel)), path_(path), closed_(false), rust_runtime_(runtime) {}

ViamChannel::~ViamChannel() {
    close();
}

std::shared_ptr<ViamChannel> ViamChannel::dial_initial(
    const char* uri, const boost::optional<DialOptions>& options) {
    DialOptions opts = options.get_value_or(DialOptions());
    auto timeout = opts.timeout;
    auto attempts_remaining = opts.initial_connection_attempts;
    if (attempts_remaining == 0) {
        attempts_remaining = -1;
    }
    opts.timeout = opts.initial_connection_attempt_timeout;

    while (attempts_remaining != 0) {
        try {
            auto connection = dial(uri, opts);
            opts.timeout = timeout;
            return connection;
        } catch (const std::exception& e) {
            attempts_remaining -= 1;
            if (attempts_remaining == 0) {
                throw e;
            }
        }
    }
    // the while loop will run until we either return or throw an error, so we can never reach this
    // point
    BOOST_UNREACHABLE_RETURN(nullptr)
}

std::shared_ptr<ViamChannel> ViamChannel::dial(const char* uri,
                                               const boost::optional<DialOptions>& options) {
    const DialOptions opts = options.get_value_or(DialOptions());

    std::cout << "checking if we're disabling webrtc\n";
    if (opts.disable_webrtc) {
        std::cout << "disabling webrtc\n";
        return dial_direct(uri, opts);
    }
    std::cout << "not disabling for some reason\n?";

    const std::chrono::duration<float> float_timeout = opts.timeout;
    void* ptr = init_rust_runtime();
    const char* type = nullptr;
    const char* entity = nullptr;
    const char* payload = nullptr;

    if (opts.credentials) {
        type = opts.credentials->type().c_str();
        payload = opts.credentials->payload().c_str();
    }

    if (opts.auth_entity) {
        entity = opts.auth_entity->c_str();
    }

    char* socket_path = ::dial(
        uri, entity, type, payload, opts.allow_insecure_downgrade, float_timeout.count(), ptr);

    if (socket_path == NULL) {
        free_rust_runtime(ptr);
        throw Exception(ErrorCondition::k_connection, "Unable to establish connecting path");
    }

    std::string address("unix://");
    address += socket_path;

    const std::shared_ptr<grpc::Channel> channel =
        impl::create_viam_channel(address, grpc::InsecureChannelCredentials());

    return std::make_shared<ViamChannel>(channel, socket_path, ptr);
}

std::shared_ptr<ViamChannel> ViamChannel::dial_direct(const char* uri, const DialOptions& opts) {
    std::cerr << "\n\tCreating grpc channel " << uri << "\n";
    // auto grpc_channel = grpc::CreateChannel("app.viam.com:443",
    // grpc::SslCredentialsOptions foo;
    auto creds = grpc::SslCredentials(grpc::SslCredentialsOptions());
    // auto grpc_channel = grpc::CreateChannel(uri, creds);
    auto channel_for_auth = impl::create_viam_auth_channel();
    // auto grpc_channel = grpc::CreateChannel(uri, grpc::InsecureChannelCredentials());
    // grpc_connectivity_state prev = grpc_connectivity_state::GRPC_CHANNEL_READY;
    // while (true) {
    // grpc_connectivity_state foo = grpc_channel->GetState(true);
    // if (foo != prev) {
    // std::cout << "foo is " << foo << "\n";
    // prev = foo;
    //}
    // if (foo == grpc_connectivity_state::GRPC_CHANNEL_READY) {
    // break;
    //}
    //};

    // auto grpc_channel = impl::create_viam_channel(uri, creds);
    //  grpc::InsecureChannelCredentials());
    std::cerr << "\n\ttest\n";

    using namespace proto::rpc::v1;

    // auto sopts = ::grpc::StubOptions();
    // std::cout << "suffix for stats is " << sopts.suffix_for_stats() << "\n";
    auto auth_stub = AuthService::NewStub(channel_for_auth);
    ClientContext ctx;
    AuthenticateRequest req;

    *req.mutable_entity() = opts.auth_entity.get();
    *req.mutable_credentials()->mutable_payload() = opts.credentials->payload();
    *req.mutable_credentials()->mutable_type() = opts.credentials->type();

    std::cerr << req.DebugString() << "\n";

    AuthenticateResponse resp;

    std::cout << "authenticating now\n";
    auto status = auth_stub->Authenticate(ctx, req, &resp);
    std::cout << "authenticated\n";

    std::cerr << "\n\t status: " << status.error_message() << " code: " << status.error_code()
              << "\n\t access token: " << resp.access_token() << "\n\n";

    ClientContext::token = resp.access_token();
    auto grpc_channel =
        impl::create_viam_channel(uri, grpc::InsecureChannelCredentials(), resp.access_token());

    return std::make_shared<ViamChannel>(grpc_channel,  //
                                         nullptr,
                                         nullptr);
}

unsigned int Options::refresh_interval() const {
    return refresh_interval_;
}

const boost::optional<DialOptions>& Options::dial_options() const {
    return dial_options_;
}

Credentials::Credentials(std::string payload)
    : type_("robot-location-secret"), payload_(std::move(payload)) {}

Credentials::Credentials(std::string type, std::string payload)
    : type_(std::move(type)), payload_(std::move(payload)) {}

}  // namespace sdk
}  // namespace viam
