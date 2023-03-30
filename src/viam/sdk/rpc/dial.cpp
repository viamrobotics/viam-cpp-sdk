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
extern "C" char* dial(const char* uri, const char* payload, bool allow_insecure, void* ptr);
namespace viam {
namespace sdk {

void ViamChannel::close() {
    if (closed) {
        return;
    }
    closed = true;
    free_string(path);
    free_rust_runtime(rust_runtime);
};

ViamChannel::ViamChannel(std::shared_ptr<grpc::Channel> channel, const char* path, void* runtime)
    : channel(channel), path(path), closed(false), rust_runtime(runtime) {}

ViamChannel ViamChannel::dial(const char* uri, boost::optional<DialOptions> options) {
    void* ptr = init_rust_runtime();
    DialOptions opts = options.get_value_or(DialOptions());
    const char* payload = nullptr;

    if (opts.credentials) {
        payload = opts.credentials->payload.c_str();
    }
    char* socket_path = ::dial(uri, payload, opts.allow_insecure_downgrade, ptr);
    if (socket_path == NULL) {
        free_rust_runtime(ptr);
        // TODO(RSDK-1742) Replace throwing of strings with throwing of
        // runtime_error
        throw "Unable to establish connecting path!";
    }

    std::string address("unix://");
    address += socket_path;
    std::shared_ptr<grpc::Channel> channel =
        grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
    std::unique_ptr<viam::robot::v1::RobotService::Stub> st =
        viam::robot::v1::RobotService::NewStub(channel);
    const char* path = address.c_str();
    ViamChannel viam_channel = ViamChannel(channel, path, ptr);
    return viam_channel;
};

Credentials::Credentials(std::string payload_) {
    type_ = "robot-location-secret";
    payload = payload_;
};

}  // namespace sdk
}  // namespace viam
