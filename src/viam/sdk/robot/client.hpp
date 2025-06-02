/// @file robot/client.hpp
///
/// @brief gRPC client implementation for a `robot`.
#pragma once

#include <atomic>
#include <string>
#include <thread>

#include <viam/sdk/common/grpc_fwd.hpp>
#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/common/world_state.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {

namespace robot {
namespace v1 {

class FrameSystemConfig;
class Operation;

}  // namespace v1
}  // namespace robot

namespace sdk {

namespace impl {
struct LogBackend;
}

/// @defgroup Robot Classes related to a Robot representation.

/// @class RobotClient client.hpp "robot/client.hpp"
/// @brief gRPC client for a robot, to be used for all interactions with a robot.
/// There are two ways to instantiate a robot:
///   - `RobotClient::at_address(...)`
///   - `RobotClient::with_channel(...)`
/// @ingroup Robot
class RobotClient {
   public:
    /// @enum status
    /// @brief the current status of the robot
    /// @ingroup Robot
    enum class status : uint8_t {
        k_initializing,
        k_running,
        k_unspecified,
    };

    friend std::ostream& operator<<(std::ostream& os, const status& v);

    struct frame_system_config {
        WorldState::transform frame;
        ProtoStruct kinematics;
        friend bool operator==(const frame_system_config& lhs, const frame_system_config& rhs);
    };

    struct operation {
        std::string id;
        std::string method;
        boost::optional<std::string> session_id;
        ProtoStruct arguments;
        boost::optional<time_pt> started;
        friend bool operator==(const operation& lhs, const operation& rhs);
    };

    explicit RobotClient(ViamChannel channel);

    ~RobotClient();

    /// @brief Call out to the robot to see if there are any new resources that need to be
    /// registered. Compares the currently registered resources to the ones from the robot, seeing
    /// if any updates have been made. If so, they are registered and updated in @ref
    /// ResourceManager. This method can be called manually, or it will be called periodically and
    /// automatically if a positive refresh_interval is passed in the Options of the named
    /// constructors.
    void refresh();

    /// @brief Disconnect this robot client from any robot to which it is connected.
    /// After calling this method it is no longer valid to call any methods which communicate with
    /// the robot.
    void close();

    /// @brief Create a robot client connected to the robot at the provided address.
    /// @param address The address of the robot (IP address, URI, URL, etc.)
    /// @param options Options for connecting and refreshing.
    static std::shared_ptr<RobotClient> at_address(const std::string& address,
                                                   const Options& options);

    /// @brief Creates a robot client connected to the robot at the provided local socket.
    /// @param address The local socket of the robot (a .sock file, etc.).
    /// @param options Options for connecting and refreshing.
    /// Creates a direct connection to the robot using the `unix:` scheme.
    /// Only useful for connecting to robots across Unix sockets.
    static std::shared_ptr<RobotClient> at_local_socket(const std::string& address,
                                                        const Options& options);

    /// @brief Creates a robot client connected to the provided channel.
    /// @param channel The channel to connect with.
    /// @param options Options for connecting and refreshing.
    static std::shared_ptr<RobotClient> with_channel(ViamChannel channel, const Options& options);

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

    /// @brief gets the current status of the machine
    status get_machine_status() const;

   private:
    friend class ModuleService;
    friend struct impl::LogBackend;

    void log(const std::string& name,
             const std::string& level,
             const std::string& message,
             time_pt time);

    // Makes this RobotClient manage logging by sending logs over grpc to viam-server.
    // This is private and only ever called by ModuleService; in other words it is only called when
    // viam-server is running a Viam C++ SDK application as a module.
    // Disables console logging so as to avoid log message duplication; console logging is
    // re-enabled on destruction.
    void connect_logging();

    void refresh_every();
    void check_connection();

    std::thread refresh_thread_;
    std::thread check_connection_thread_;
    std::atomic<bool> should_refresh_;
    std::atomic<bool> should_check_connection_;
    std::chrono::seconds refresh_interval_;
    unsigned int check_every_interval_;
    unsigned int reconnect_every_interval_;

    ViamChannel viam_channel_;

    struct impl;
    std::unique_ptr<impl> impl_;

    mutable std::mutex lock_;

    std::vector<Name> resource_names_;
    ResourceManager resource_manager_;
};

namespace proto_convert_details {

template <>
struct from_proto_impl<robot::v1::Operation> {
    RobotClient::operation operator()(const robot::v1::Operation*) const;
};

template <>
struct from_proto_impl<robot::v1::FrameSystemConfig> {
    RobotClient::frame_system_config operator()(const robot::v1::FrameSystemConfig*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
