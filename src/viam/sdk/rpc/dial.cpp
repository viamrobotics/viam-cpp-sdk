#include <viam/sdk/rpc/dial.hpp>

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

ViamChannel ViamChannel::dial_initial(const char* uri,
                                      const boost::optional<DialOptions>& options) {
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
    BOOST_UNREACHABLE_RETURN(ViamChannel(nullptr))
}

ViamChannel ViamChannel::dial(const char* uri, const boost::optional<DialOptions>& options) {
    const DialOptions opts = options.get_value_or(DialOptions());

    // If this flag is passed, try to dial directly through grpc if possible.
    // If grpc is too old to do a direct dial, we fall back to the rust version below even if
    // disable_webrtc was pased. This is consistent with a hoped-for future semantic where rust
    // would get the disable_webrtc flag as well rather than deducing it from the URI format, which
    // is what it currently does.
    if (opts.disable_webrtc) {
#ifndef VIAMCPPSDK_GRPCXX_NO_DIRECT_DIAL
        return dial_direct(uri, opts);
#endif
    }

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

    return ViamChannel(
        sdk::impl::create_viam_grpc_channel(address, grpc::InsecureChannelCredentials()),
        socket_path,
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

    *req.mutable_entity() = opts.auth_entity.get();
    *req.mutable_credentials()->mutable_payload() = opts.credentials->payload();
    *req.mutable_credentials()->mutable_type() = opts.credentials->type();

    AuthenticateResponse resp;

    auto status = auth_stub->Authenticate(ctx, req, &resp);

    if (!status.ok()) {
        VIAM_SDK_LOG(error) << "Direct dial authentication request failed: "
                            << status.error_message();
        throw GRPCException(&status);
    }

    Instance::current(Instance::Creation::open_existing).impl_->direct_dial_token =
        resp.access_token();

    grpc::experimental::TlsChannelCredentialsOptions c_opts;
    c_opts.set_check_call_host(false);
    auto creds = grpc::experimental::TlsCredentials(c_opts);
    return ViamChannel(sdk::impl::create_viam_grpc_channel(uri, creds));
#else
    throw std::logic_error("Tried to call dial_direct on unsupported grpc version " +
                           grpc::Version());
#endif
}

const std::shared_ptr<grpc::Channel>& ViamChannel::channel() const {
    return channel_;
}

void ViamChannel::close() {
    pimpl_.reset();
}

Credentials::Credentials(std::string payload)
    : type_("robot-location-secret"), payload_(std::move(payload)) {}

Credentials::Credentials(std::string type, std::string payload)
    : type_(std::move(type)), payload_(std::move(payload)) {}

}  // namespace sdk
}  // namespace viam
