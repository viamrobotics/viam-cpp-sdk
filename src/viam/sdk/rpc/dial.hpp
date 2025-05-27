#pragma once

#include <chrono>
#include <memory>
#include <string>

#include <boost/optional.hpp>

#include <viam/sdk/common/grpc_fwd.hpp>

namespace viam {
namespace sdk {

struct DialOptions;

class ViamChannel {
    ViamChannel(std::shared_ptr<GrpcChannel> channel, const char* path, void* runtime);

   public:
    explicit ViamChannel(std::shared_ptr<GrpcChannel> channel);

    ViamChannel(ViamChannel&&) noexcept;

    ViamChannel& operator=(ViamChannel&&) noexcept;

    ~ViamChannel();

    /// @brief Connects to a robot at the given URI address, using the provided dial options (or
    /// default options is none are provided). Ignores initial connection options specifying
    /// how many times to attempt to connect and with what timeout.
    /// In general, use of this method is discouraged. `RobotClient::at_address(...)` is the
    /// preferred method to connect to a robot, and creates the channel itself.
    /// @throws Exception if it is unable to establish a connection to the provided URI
    static ViamChannel dial(const char* uri, const boost::optional<DialOptions>& options);

    /// @brief Dials to a robot at the given URI address, using the provided dial options (or
    /// default options is none are provided).
    /// Additionally specifies that this dial is an initial
    /// connection attempt and so uses the initial connection options. In general, use of this
    /// method is discouraged. `RobotClient::at_address(...)` is the preferred method to connect to
    /// a robot, and creates the channel itself.
    /// @throws Exception if it is unable to establish a connection to the provided URI within
    /// the given number of initial connection attempts
    static ViamChannel dial_initial(const char* uri, const boost::optional<DialOptions>& options);

    const std::shared_ptr<GrpcChannel>& channel() const;

    /// @brief Returns the bearer token for connecting to the robot if one is needed; else returns
    /// null.
    /// @remark When dialing with disable_webrtc = true and grpc >= 1.43.0, a bearer token is needed
    /// for all client requests. If you use ClientHelper for client requests this is handled
    /// automatically, otherwise you will have to add this to the client context of a grpc call.
    const boost::optional<std::string>& auth_token() const;

    void close();

   private:
    struct impl;

    static ViamChannel dial_direct(const char* uri, const DialOptions& opts);

    std::shared_ptr<GrpcChannel> channel_;

    boost::optional<std::string> auth_token_;

    std::unique_ptr<impl> pimpl_;
};

class Credentials {
   public:
    /// @deprecated use `Credentials(std::string type, std::string payload)`
    Credentials(std::string payload);
    Credentials(std::string type, std::string payload);
    const std::string& type() const;
    const std::string& payload() const;

   private:
    std::string type_;
    std::string payload_;
};

struct DialOptions {
    /// @brief the URL to authenticate against.
    boost::optional<std::string> auth_entity;

    /// @brief Credentials for connecting to the robot.
    boost::optional<Credentials> credentials;

    /// @brief Allows the RPC connection to be downgraded to an insecure connection if detected.
    /// This is only used when credentials are not present.
    bool allow_insecure_downgrade = false;

    /// @brief Bypass WebRTC and connect directly to the robot.
    /// This dials directly through grpc bypassing rust utils.
    /// @remark Direct dialing should generally be done with a machine URI of the form
    /// <part>.<location>.local.viam.cloud:8080
    bool disable_webrtc = false;

    /// @brief Duration before the dial connection times out
    /// Set to 20sec to match _defaultOfferDeadline in goutils/rpc/wrtc_call_queue.go
    std::chrono::duration<float> timeout{20};

    /// @brief Number of attempts to make when initially connecting to a robot
    /// If set to 0 or a negative integer, will attempt to reconnect forever.
    int initial_connection_attempts = 3;

    /// @brief Timeout of connection attempts when initially dialing a robot
    /// Defaults to 20sec to match the default timeout duration
    std::chrono::duration<float> initial_connection_attempt_timeout{20};
};

}  // namespace sdk
}  // namespace viam
