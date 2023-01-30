#pragma once

#include <grpcpp/channel.h>

#include <boost/optional.hpp>
#include <memory>
#include <string>
namespace Viam {
namespace SDK {

class DialOptions;
class ViamChannel {
   public:
    std::shared_ptr<grpc::Channel> channel;
    void close();
    ViamChannel(std::shared_ptr<grpc::Channel> channel, const char* path, void* runtime);
    static ViamChannel dial(const char* uri, boost::optional<DialOptions> options);

   private:
    const char* path;
    bool closed;
    void* rust_runtime;
};

class Credentials {
   public:
    std::string type_;
    std::string payload;

    Credentials(std::string payload);
};

class DialOptions {
   public:
    /// Bypass webRTC and connect directly to the robot
    // TODO (RSDK-917): This field is currently just for show, we
    // should update dial logic to actually care about this
    bool disable_webrtc;

    /// the URL to authenticate against
    boost::optional<std::string> auth_entity;

    /// Credentials for connecting to the robot
    boost::optional<Credentials> credentials;

    /// Determine if the connection is TLS-based. Must be set to
    /// `true` to establish an insecure connection. Otherwise, a
    /// TLS-based connection will be assumed
    bool insecure;

    /// allow the RPC connection to be downgraded to an insecure
    /// connection if detected. This is only used when credentials
    /// are not present
    bool allow_insecure_downgrade;

    /// allow the RPC connection to be downgraded to an insecure
    /// connection if detected, even with credentials present. This
    /// is generally unsafe to do, but can be requested
    bool allow_insecure_with_creds_downgrade;
    DialOptions() {
        disable_webrtc = false;
        auth_entity = boost::none;
        credentials = boost::none;
        insecure = false;
        allow_insecure_downgrade = false;
        allow_insecure_with_creds_downgrade = false;
    }
};

class Options {
   public:
    /// How often to refresh the status/parts of the robot, in seconds.
    /// if set to 0, the robot will not automatically refresh.
    unsigned int refresh_interval;
    boost::optional<DialOptions> dial_options;
    Options(unsigned int refresh_interval_, boost::optional<DialOptions> dial_options_) {
        refresh_interval = refresh_interval_;
        dial_options = dial_options_;
    }
};

}  // namespace SDK
}  // namespace Viam
