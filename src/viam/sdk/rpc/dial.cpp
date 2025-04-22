#include <viam/sdk/rpc/dial.hpp>

#include <istream>
#include <string>

#include <boost/config.hpp>
#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/rpc/private/viam_grpc_channel.hpp>
#include <viam/sdk/rpc/private/viam_rust_utils.h>

namespace viam {
namespace sdk {

ViamChannel::RustDialData::RustDialData(const char* path_, void* runtime)
    : path(path_), rust_runtime(runtime) {}

ViamChannel::RustDialData::RustDialData(RustDialData&& other) noexcept
    : path(std::exchange(other.path, nullptr)),
      rust_runtime(std::exchange(other.rust_runtime, nullptr)) {}

ViamChannel::RustDialData& ViamChannel::RustDialData::operator=(RustDialData&& other) noexcept {
    path = std::exchange(other.path, nullptr);
    rust_runtime = std::exchange(other.rust_runtime, nullptr);

    return *this;
}

ViamChannel::RustDialData::~RustDialData() {
    free_string(path);
    free_rust_runtime(rust_runtime);
}

ViamChannel::ViamChannel(std::shared_ptr<grpc::Channel> channel, const char* path, void* runtime)
    : channel_(std::move(channel)), rust_data_(RustDialData(path, runtime)) {}

ViamChannel::ViamChannel(std::shared_ptr<grpc::Channel> channel) : channel_(std::move(channel)) {}

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

std::shared_ptr<ViamChannel> ViamChannel::dial_initial(
    const char* uri, const boost::optional<DialOptions>& options) {
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
    BOOST_UNREACHABLE_RETURN(nullptr)
}

std::shared_ptr<ViamChannel> ViamChannel::dial(const char* uri,
                                               const boost::optional<DialOptions>& options) {
    void* ptr = init_rust_runtime();
    const DialOptions opts = options.get_value_or(DialOptions());
    const std::chrono::duration<float> float_timeout = opts.timeout();
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
    char* socket_path = ::dial(
        uri, entity, type, payload, opts.allows_insecure_downgrade(), float_timeout.count(), ptr);
    if (socket_path == NULL) {
        free_rust_runtime(ptr);
        throw Exception(ErrorCondition::k_connection, "Unable to establish connecting path");
    }

    std::string address("unix://");
    address += socket_path;
    const std::shared_ptr<grpc::Channel> channel =
        impl::create_viam_channel(address, grpc::InsecureChannelCredentials());
    const std::unique_ptr<viam::robot::v1::RobotService::Stub> st =
        viam::robot::v1::RobotService::NewStub(channel);
    return std::make_shared<ViamChannel>(channel, socket_path, ptr);
}

const std::shared_ptr<grpc::Channel>& ViamChannel::channel() const {
    return channel_;
}

void ViamChannel::close() {
    rust_data_.reset();
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
