#include <viam/sdk/rpc/dial.hpp>

#include <istream>
#include <string>

#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

extern "C" void* init_rust_runtime();
extern "C" int free_rust_runtime(void* ptr);
extern "C" void free_string(const char* s);
extern "C" char* dial(const char* uri,
                      const char* entity,
                      const char* type,
                      const char* payload,
                      bool allow_insecure,
                      const float timeout,
                      void* ptr);
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
    : channel_(channel), path_(path), closed_(false), rust_runtime_(runtime) {}

void DialOptions::set_credentials(boost::optional<Credentials> creds) {
    credentials_ = creds;
}

void DialOptions::set_entity(boost::optional<std::string> entity) {
    auth_entity_ = entity;
}

void DialOptions::set_timeout(const float timeout) {
    timeout_ = timeout;
}

const boost::optional<std::string>& DialOptions::entity() const {
    return auth_entity_;
}

const boost::optional<Credentials>& DialOptions::credentials() const {
    return credentials_;
}

const float DialOptions::timeout() const {
    return timeout_;
}

void DialOptions::set_allow_insecure_downgrade(bool allow) {
    allow_insecure_downgrade_ = allow;
}

bool DialOptions::allows_insecure_downgrade() const {
    return allow_insecure_downgrade_;
}

std::shared_ptr<ViamChannel> ViamChannel::dial(const char* uri,
                                               boost::optional<DialOptions> options) {
    void* ptr = init_rust_runtime();
    const DialOptions opts = options.get_value_or(DialOptions());
    const float timeout = opts.timeout();
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
    char* socket_path = ::dial(uri, entity, type, payload, opts.allows_insecure_downgrade(), timeout , ptr);
    if (socket_path == NULL) {
        free_rust_runtime(ptr);
        // TODO(RSDK-1742) Replace throwing of strings with throwing of
        // runtime_error
        throw "Unable to establish connecting path!";
    }

    std::string address("unix://");
    address += socket_path;
    const std::shared_ptr<grpc::Channel> channel =
        grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
    const std::unique_ptr<viam::robot::v1::RobotService::Stub> st =
        viam::robot::v1::RobotService::NewStub(channel);
    return std::make_shared<ViamChannel>(channel, socket_path, ptr);
};

unsigned int Options::refresh_interval() const {
    return refresh_interval_;
}

const boost::optional<DialOptions>& Options::dial_options() const {
    return dial_options_;
}

Credentials::Credentials(std::string payload)
    : type_("robot-location-secret"), payload_(std::move(payload)){};

Credentials::Credentials(std::string type, std::string payload)
    : type_(std::move(type)), payload_(std::move(payload)){};

}  // namespace sdk
}  // namespace viam
