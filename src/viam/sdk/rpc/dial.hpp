#pragma once

#include <chrono>
#include <memory>
#include <string>

#include <boost/optional.hpp>

#include <viam/sdk/common/grpc_fwd.hpp>

namespace viam {
namespace sdk {

class DialOptions;
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

    // @brief Dials to a robot at the given URI address, using the provided dial options (or default
    // options is none are provided). Additionally specifies that this dial is an initial connection
    // attempt and so uses the initial connection options.
    /// In general, use of this method is discouraged. `RobotClient::at_address(...)` is the
    /// preferred method to connect to a robot, and creates the channel itself.
    /// @throws Exception if it is unable to establish a connection to the provided URI within
    /// the given number of initial connection attempts
    static ViamChannel dial_initial(const char* uri, const boost::optional<DialOptions>& options);

    const std::shared_ptr<GrpcChannel>& channel() const;

    void close();

    const char* get_channel_addr() const;

   private:
    const char* uri_;
    struct impl;

    std::shared_ptr<GrpcChannel> channel_;

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

class DialOptions {
   public:
    DialOptions();

    const boost::optional<Credentials>& credentials() const;
    const boost::optional<std::string>& entity() const;
    bool allows_insecure_downgrade() const;
    const std::chrono::duration<float>& timeout() const;
    int initial_connection_attempts() const;
    std::chrono::duration<float> initial_connection_attempt_timeout() const;

    DialOptions& set_entity(boost::optional<std::string> entity);
    DialOptions& set_credentials(boost::optional<Credentials> creds);
    DialOptions& set_allow_insecure_downgrade(bool allow);
    DialOptions& set_timeout(std::chrono::duration<float> timeout);
    DialOptions& set_initial_connection_attempts(int attempts);
    DialOptions& set_initial_connection_attempt_timeout(std::chrono::duration<float> timeout);

   private:
    // TODO (RSDK-917): We currently don't provide a flag for disabling webRTC, instead relying on a
    // `local` uri. We should update dial logic to consider such a flag.

    /// @brief the URL to authenticate against.
    boost::optional<std::string> auth_entity_;

    /// @brief Credentials for connecting to the robot.
    boost::optional<Credentials> credentials_;

    /// @brief Allows the RPC connection to be downgraded to an insecure connection if detected.
    /// This is only used when credentials are not present.
    bool allow_insecure_downgrade_ = false;

    /// @brief Duration before the dial connection times out
    /// Set to 20sec to match _defaultOfferDeadline in goutils/rpc/wrtc_call_queue.go
    std::chrono::duration<float> timeout_{20};

    /// @brief Number of attempts to make when initially connecting to a robot
    /// If set to 0 or a negative integer, will attempt to reconnect forever.
    int initial_connection_attempts_ = 3;

    /// @brief Timeout of connection attempts when initially dialing a robot
    /// Defaults to 20sec to match the default timeout duration
    std::chrono::duration<float> initial_connection_attempt_timeout_{20};
};

class Options {
   public:
    Options(unsigned int refresh_interval, boost::optional<DialOptions> dial_options)
        : refresh_interval_(std::move(refresh_interval)), dial_options_(std::move(dial_options)) {}

    unsigned int refresh_interval() const;
    std::chrono::seconds check_every_interval() const;
    std::chrono::seconds reconnect_every_interval() const;

    /// @brief Sets the frequency (in seconds) to verify connectivity
    Options& set_check_every_interval(std::chrono::seconds interval);

    /// @brief Sets the frequency (in seconds) to attempt to reconnect when connectivity is lost
    Options& set_reconnect_every_interval(std::chrono::seconds interval);
    const boost::optional<DialOptions>& dial_options() const;

   private:
    /// @brief How often to refresh the status/parts of the robot, in seconds. If set to 0, the
    /// robot will not automatically refresh.
    std::chrono::seconds refresh_interval_{0};

    /// @brief How often to verify connectivity to the robot, in seconds. If set to 0, will not
    /// check, will default to the `reconnect_every_interval_` value. Defaults to 0.
    /// @note Setting to a non-zero value is useful in modules but may result in delays shutting
    /// down client code
    std::chrono::seconds check_every_interval_{0};

    /// @brief How often to attempt to reconnect to the robot when disconnected. If set to 0,
    /// will not attempt to reconnect. Defaults to 0.
    /// @note Setting to a non-zero value is useful in modules but may result in delays shutting
    /// down client code
    std::chrono::seconds reconnect_every_interval_{0};
    boost::optional<DialOptions> dial_options_;
};

}  // namespace sdk
}  // namespace viam
