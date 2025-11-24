#include <viam/sdk/rpc/dial.hpp>

#include <algorithm>
#include <istream>
#include <string>

#include <boost/config.hpp>
#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/utility/string_view.hpp>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>

#include <viam/api/proto/rpc/v1/auth.grpc.pb.h>
#include <viam/api/proto/rpc/v1/auth.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/private/instance.hpp>
#include <viam/sdk/rpc/private/viam_grpc_channel.hpp>
#include <viam/sdk/rpc/private/viam_rust_utils.h>

namespace viam {
namespace sdk {

struct ViamChannel::impl {
    struct cstr_delete {
        void operator()(const char* str) noexcept {
            free_string(str);
        }
    };

    struct rust_rt_delete {
        void operator()(void* rt) noexcept {
            free_rust_runtime(rt);
        }
    };

    impl(const char* path, void* runtime) : path(path), rust_runtime(runtime) {}

    std::unique_ptr<const char, cstr_delete> path;
    std::unique_ptr<void, rust_rt_delete> rust_runtime;
};

ViamChannel::ViamChannel(std::shared_ptr<grpc::Channel> channel, const char* path, void* runtime)
    : channel_(std::move(channel)), pimpl_(std::make_unique<ViamChannel::impl>(path, runtime)) {}

ViamChannel::ViamChannel(std::shared_ptr<grpc::Channel> channel) : channel_(std::move(channel)) {}

ViamChannel::ViamChannel(ViamChannel&&) noexcept = default;

ViamChannel& ViamChannel::operator=(ViamChannel&&) noexcept = default;

ViamChannel::~ViamChannel() {
    close();
}

const std::string& Credentials::type() const {
    return type_;
}

const std::string& Credentials::payload() const {
    return payload_;
}

DialOptions::DialOptions() = default;

DialOptions& DialOptions::set_credentials(boost::optional<Credentials> creds) {
    credentials_ = std::move(creds);

    return *this;
}

DialOptions& DialOptions::set_entity(boost::optional<std::string> entity) {
    auth_entity_ = std::move(entity);

    return *this;
}

DialOptions& DialOptions::set_initial_connection_attempts(int attempts) {
    initial_connection_attempts_ = attempts;

    return *this;
}

DialOptions& DialOptions::set_timeout(std::chrono::duration<float> timeout) {
    timeout_ = timeout;

    return *this;
}

DialOptions& DialOptions::set_initial_connection_attempt_timeout(
    std::chrono::duration<float> timeout) {
    initial_connection_attempt_timeout_ = timeout;

    return *this;
}

const boost::optional<std::string>& DialOptions::entity() const {
    return auth_entity_;
}

const boost::optional<Credentials>& DialOptions::credentials() const {
    return credentials_;
}

int DialOptions::initial_connection_attempts() const {
    return initial_connection_attempts_;
}

const std::chrono::duration<float>& DialOptions::timeout() const {
    return timeout_;
}

std::chrono::duration<float> DialOptions::initial_connection_attempt_timeout() const {
    return initial_connection_attempt_timeout_;
}

DialOptions& DialOptions::set_allow_insecure_downgrade(bool allow) {
    allow_insecure_downgrade_ = allow;

    return *this;
}

bool DialOptions::allows_insecure_downgrade() const {
    return allow_insecure_downgrade_;
}

bool DialOptions::webrtc_disabled() const {
    return disable_webrtc_;
}

DialOptions& DialOptions::set_webrtc_disabled(bool disable_webrtc) {
    disable_webrtc_ = disable_webrtc;

    return *this;
}

ViamChannel ViamChannel::dial_initial(const char* uri,
                                      const boost::optional<DialOptions>& options) {
    DialOptions opts = options.get_value_or(DialOptions());
    auto timeout = opts.timeout();
    auto attempts_remaining = opts.initial_connection_attempts();
    if (attempts_remaining == 0) {
        attempts_remaining = -1;
    }
    opts.set_timeout(opts.initial_connection_attempt_timeout());

    while (attempts_remaining != 0) {
        try {
            auto connection = dial(uri, opts);
            opts.set_timeout(timeout);
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
    BOOST_UNREACHABLE_RETURN(ViamChannel(nullptr))
}

ViamChannel ViamChannel::dial(const char* uri, const boost::optional<DialOptions>& options) {
    const DialOptions opts = options.get_value_or(DialOptions());

    // If this flag is passed, try to dial directly through grpc if possible.
    // If grpc is too old to do a direct dial, we fall back to the rust version below even if
    // disable_webrtc was pased. This is consistent with a hoped-for future semantic where rust
    // would get the disable_webrtc flag as well rather than deducing it from the URI format, which
    // is what it currently does.
    if (opts.webrtc_disabled()) {
#ifndef VIAMCPPSDK_GRPCXX_NO_DIRECT_DIAL
        return dial_direct(uri, opts);
#endif
    }

    const std::chrono::duration<float> timeout = opts.timeout();
    void* ptr = init_rust_runtime();
    const char* type = nullptr;
    const char* entity = nullptr;
    const char* payload = nullptr;

    if (opts.credentials()) {
        type = opts.credentials()->type().c_str();
        payload = opts.credentials()->payload().c_str();
    }

    if (opts.entity()) {
        entity = opts.entity()->c_str();
    }

    char* proxy_path =
        ::dial(uri, entity, type, payload, opts.allows_insecure_downgrade(), timeout.count(), ptr);

    if (!proxy_path) {
        free_rust_runtime(ptr);
        throw Exception(ErrorCondition::k_connection, "Unable to establish connecting path");
    }

    const std::string localhost_prefix("127.0.0.1");
    std::string address;
    if (std::string(proxy_path).find(localhost_prefix) == std::string::npos) {
        // proxy path is not a localhost address and is therefore a unix domain socket (UDS)
        address += "unix:";
    }
    address += proxy_path;

    return ViamChannel(
        sdk::impl::create_viam_grpc_channel(address, grpc::InsecureChannelCredentials()),
        proxy_path,
        ptr);
}

ViamChannel ViamChannel::dial_direct(const char* uri, const DialOptions& opts) {
#ifndef VIAMCPPSDK_GRPCXX_NO_DIRECT_DIAL
    // TODO: if we ever drop older grpc support the logic below might make sense as a
    // set_bearer_token helper function, but for now it just proliferates the ifdef messiness so
    // we'll leave it inline
    auto channel_for_auth = sdk::impl::create_viam_auth_channel(uri);
    using namespace proto::rpc::v1;

    auto auth_stub = AuthService::NewStub(channel_for_auth);
    ClientContext ctx;
    AuthenticateRequest req;

    *req.mutable_entity() = opts.entity().get();
    *req.mutable_credentials()->mutable_payload() = opts.credentials()->payload();
    *req.mutable_credentials()->mutable_type() = opts.credentials()->type();

    AuthenticateResponse resp;

    auto status = auth_stub->Authenticate(ctx, req, &resp);

    if (!status.ok()) {
        VIAM_SDK_LOG(error) << "Direct dial authentication request failed: "
                            << status.error_message();
        throw GRPCException(&status);
    }

    grpc::experimental::TlsChannelCredentialsOptions c_opts;
    c_opts.set_check_call_host(false);
    auto creds = grpc::experimental::TlsCredentials(c_opts);
    auto result = ViamChannel(sdk::impl::create_viam_grpc_channel(uri, creds));
    result.auth_token_ = resp.access_token();

    return result;
#else
    (void)uri;
    (void)opts;
    throw std::logic_error("Tried to call dial_direct on unsupported grpc version " +
                           grpc::Version());
#endif
}

const std::shared_ptr<grpc::Channel>& ViamChannel::channel() const {
    return channel_;
}

const boost::optional<std::string>& ViamChannel::auth_token() const {
    return auth_token_;
}

void ViamChannel::close() {
    pimpl_.reset();
}

const char* ViamChannel::get_channel_addr() const {
    return uri_;
}

Options& Options::set_check_every_interval(std::chrono::seconds interval) {
    check_every_interval_ = interval;
    return *this;
}

Options& Options::set_reconnect_every_interval(std::chrono::seconds interval) {
    reconnect_every_interval_ = interval;
    return *this;
}

std::chrono::seconds Options::check_every_interval() const {
    return check_every_interval_;
}

std::chrono::seconds Options::reconnect_every_interval() const {
    return reconnect_every_interval_;
}

std::chrono::seconds Options::refresh_interval() const {
    return refresh_interval_;
}

const boost::optional<DialOptions>& Options::dial_options() const {
    return dial_options_;
}

Credentials::Credentials(std::string type, std::string payload)
    : type_(std::move(type)), payload_(std::move(payload)) {}

}  // namespace sdk
}  // namespace viam
