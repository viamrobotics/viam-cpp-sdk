/// @file robot/client.hpp
///
/// @brief gRPC client implementation for a `robot`.
#pragma once

#include <chrono>
#include <string>
#include <thread>

#include <grpcpp/channel.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/common/world_state.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

using grpc::Channel;

/// @defgroup Robot Classes related to a Robot representation.

/// @class RobotClient client.hpp "robot/client.hpp"
/// @brief gRPC client for a robot, to be used for all interactions with a robot.
/// There are two ways to instantiate a robot:
///   - `RobotClient::at_address(...)`
///   - `RobotClient::with_channel(...)`
/// @ingroup Robot
///
/// You must `close()` a robot when finished with it in order to release its resources.
/// Robots creates via `at_address` will automatically close, but robots created via
/// `with_channel` require a user call to `close()`.
class RobotClient {
   public:
    struct discovery_query {
        std::string subtype;
        std::string model;
        friend bool operator==(const discovery_query& lhs, const discovery_query& rhs);
    };

    struct discovery {
        discovery_query query;
        ProtoStruct results;
        friend bool operator==(const discovery& lhs, const discovery& rhs);
    };

    struct frame_system_config {
        WorldState::transform frame;
        ProtoStruct kinematics;
        friend bool operator==(const frame_system_config& lhs, const frame_system_config& rhs);
    };

    struct status {
        boost::optional<Name> name;
        ProtoStruct status_map;
        // TODO: RSDK-6574: revisit time_point
        boost::optional<std::chrono::time_point<long long, std::chrono::nanoseconds>>
            last_reconfigured;
        friend bool operator==(const status& lhs, const status& rhs);
    };

    struct operation {
        std::string id;
        std::string method;
        boost::optional<std::string> session_id;
        ProtoStruct arguments;
        // TODO: RSDK-6574: revisit time_point
        boost::optional<std::chrono::time_point<long long, std::chrono::nanoseconds>> started;
        friend bool operator==(const operation& lhs, const operation& rhs);
    };

    ~RobotClient();
    void refresh();
    void close();

    /// @brief Create a robot client connected to the robot at the provided address.
    /// @param address The address of the robot (IP address, URI, URL, etc.)
    /// @param options Options for connecting and refreshing.
    static std::shared_ptr<RobotClient> at_address(const std::string& address,
                                                   const Options& options);

    /// @brief Creates a robot client connected to the robot at the provided local socket.
    /// @param address The local socket of the robot (a .sock file, etc.).
    /// @param options Options for connecting and refreshing.
    /// Creates a direct connection to the robot using the `unix://` scheme.
    /// Only useful for connecting to robots across Unix sockets.
    static std::shared_ptr<RobotClient> at_local_socket(const std::string& address,
                                                        const Options& options);

    /// @brief Creates a robot client connected to the provided channel.
    /// @param channel The channel to connect with.
    /// @param options Options for connecting and refreshing.
    /// Connects directly to a pre-existing channel. A robot created this way must be
    /// `close()`d manually.
    static std::shared_ptr<RobotClient> with_channel(std::shared_ptr<ViamChannel> channel,
                                                     const Options& options);
    RobotClient(std::shared_ptr<ViamChannel> channel);
    std::vector<Name> resource_names() const;

    /// @brief Lookup and return a `shared_ptr` to a resource.
    /// @param name The `Name` of the resource.
    /// @throws `Exception` if the requested resource doesn't exist or is the wrong type.
    /// @return a `shared_ptr` to the requested resource as an uncasted `Resource`.
    ///
    /// This method should not be called directly except in specific cases. The
    /// type-annotated `resource_by_name<T>(name)` overload is the preferred method
    /// for obtaining resources.
    ///
    /// Because the return type here is a `Resource`, the user will need to manually
    /// cast to the desired type.
    std::shared_ptr<Resource> resource_by_name(const Name& name);

    void log(
        std::string name,
        std::string level,
        std::string message,
        const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>& time,
        std::string stack);

    template <typename T>
    /// @brief Lookup and return a `shared_ptr` to a resource of the requested type.
    /// @param name The ordinary name of the resource.
    /// @throws `Exception` if the requested resource doesn't exist or is the wrong type.
    /// @return a `shared_ptr` to the requested resource.
    std::shared_ptr<T> resource_by_name(std::string name) {
        return std::dynamic_pointer_cast<T>(resource_by_name({API::get<T>(), "", std::move(name)}));
    }

    /// @brief Get the configuration of the frame system of the given robot.
    /// @return The configuration of the calling robot's frame system.
    std::vector<frame_system_config> get_frame_system_config(
        const std::vector<WorldState::transform>& additional_transforms = {});

    /// @brief Get the list of operations currently running on a robot.
    /// @return The list of operations currently running on the calling robot.
    std::vector<operation> get_operations();

    /// @brief Get the status of the requested robot components.
    /// @param components A list of the specific components for which status is desired.
    /// @return A list of statuses.
    std::vector<status> get_status(std::vector<Name>& components);

    /// @brief Get the status of all robot components.
    /// @return A list of statuses.
    std::vector<status> get_status();

    std::vector<discovery> discover_components(const std::vector<discovery_query>& queries);

    /// @brief Transform a given `Pose` to a new specified destination which is a reference frame.
    /// @param query The pose that should be transformed.
    /// @param destination The name of the reference frame to transform the given pose to.
    /// @return the `pose_in_frame` of the transformed pose.
    pose_in_frame transform_pose(
        const pose_in_frame& query,
        std::string destination,
        const std::vector<WorldState::transform>& additional_transforms = {});

    /// @brief Blocks on the specified operation of the robot, returning when it is complete.
    /// @param id The ID of the operation to block on.
    void block_for_operation(std::string id);

    /// @brief Cancel all operations for the robot and stop all actuators and movement.
    void stop_all();

    /// @brief Cancel all operations for the robot and stop all actuators and movement.
    /// @param extra Any extra params to pass to resources' `stop` methods, keyed by `Name`.
    void stop_all(const std::unordered_map<Name, ProtoStruct>& extra);

    /// @brief Cancel a specified operation on the robot.
    /// @param id The ID of the operation to cancel.
    void cancel_operation(std::string id);

   private:
    std::vector<std::shared_ptr<std::thread>> threads_;
    std::atomic<bool> should_refresh_;
    unsigned int refresh_interval_;
    std::shared_ptr<Channel> channel_;
    std::shared_ptr<ViamChannel> viam_channel_;
    bool should_close_channel_;
    struct impl;
    std::unique_ptr<impl> impl_;
    mutable std::mutex lock_;
    std::vector<Name> resource_names_;
    ResourceManager resource_manager_;
    void refresh_every();
};

}  // namespace sdk
}  // namespace viam
