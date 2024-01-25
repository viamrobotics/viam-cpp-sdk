/// @file robot/client.cpp
///
/// @brief gRPC client implementation for a `robot`.
#pragma once

#include <string>
#include <thread>

#include <grpcpp/channel.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

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
using viam::common::v1::ResourceName;
using viam::robot::v1::RobotService;
using viam::robot::v1::Status;

struct discoveryQuery {
    discoveryQuery(){};
    viam::robot::v1::DiscoveryQuery to_proto() const;
    static discoveryQuery from_proto(const viam::robot::v1::DiscoveryQuery& proto);
    std::string subtype;
    std::string model;
    friend bool operator==(const discoveryQuery& lhs, const discoveryQuery& rhs);
};

struct discovery {
    discovery(){};
    viam::robot::v1::Discovery to_proto() const;
    static discovery from_proto(const viam::robot::v1::Discovery& proto);
    discoveryQuery query;
    AttributeMap results;
    friend bool operator==(const discovery& lhs, const discovery& rhs);
};

struct frameSystemConfig {
    viam::robot::v1::FrameSystemConfig to_proto() const;
    static frameSystemConfig from_proto(const viam::robot::v1::FrameSystemConfig& proto);
    frameSystemConfig(WorldState::transform frame) : frame(std::move(frame)){};

    WorldState::transform frame;
};

// TODO(RSDK-1742) replace all `ResourceName` references in API with `Name`
/// @defgroup Robot Classes related to a Robot representation.

/// @class RobotClient client.h "robot/client.h"
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
    ~RobotClient();
    void refresh();
    void close();

    /// @brief Create a robot client connected to the robot at the provided address.
    /// @param address The address of the robot (IP address, URI, URL, etc.)
    /// @param options Options for connecting and refreshing.
    static std::shared_ptr<RobotClient> at_address(std::string address, Options options);

    /// @brief Creates a robot client connected to the robot at the provided local socket.
    /// @param address The local socket of the robot (a .sock file, etc.).
    /// @param options Options for connecting and refreshing.
    /// Creates a direct connection to the robot using the `unix://` scheme.
    /// Only useful for connecting to robots across Unix sockets.
    static std::shared_ptr<RobotClient> at_local_socket(std::string address, Options options);

    /// @brief Creates a robot client connected to the provided channel.
    /// @param channel The channel to connect with.
    /// @param options Options for connecting and refreshing.
    /// Connects directly to a pre-existing channel. A robot created this way must be
    /// `close()`d manually.
    static std::shared_ptr<RobotClient> with_channel(std::shared_ptr<ViamChannel> channel,
                                                     Options options);
    RobotClient(std::shared_ptr<ViamChannel> channel);
    std::vector<ResourceName>* resource_names();

    /// @brief Lookup and return a `shared_ptr` to a resource.
    /// @param name The `ResourceName` of the resource.
    /// @throws `std::runtime_error` if the requested resource doesn't exist or is the wrong type.
    /// @return a `shared_ptr` to the requested resource as an uncasted `Resource`.
    ///
    /// This method should not be called directly except in specific cases. The
    /// type-annotated `resource_by_name<T>(name)` overload is the preferred method
    /// for obtaining resources.
    ///
    /// Because the return type here is a `Resource`, the user will need to manually
    /// cast to the desired type.
    std::shared_ptr<Resource> resource_by_name(const ResourceName& name);

    template <typename T>
    /// @brief Lookup and return a `shared_ptr` to a resource of the requested type.
    /// @param name The ordinary name of the resource.
    /// @throws `std::runtime_error` if the requested resource doesn't exist or is the wrong type.
    /// @return a `shared_ptr` to the requested resource.
    std::shared_ptr<T> resource_by_name(std::string name) {
        ResourceName r;
        API api = API::get<T>();
        *r.mutable_namespace_() = api.type_namespace();
        *r.mutable_type() = api.resource_type();
        *r.mutable_subtype() = api.resource_subtype();
        *r.mutable_name() = std::move(name);

        auto resource = this->resource_by_name(std::move(r));
        return std::dynamic_pointer_cast<T>(resource);
    }

    /// @brief Get the configuration of the frame system of the given robot.
    /// @return The configuration of the calling robot's frame system.
    std::vector<frameSystemConfig> get_frame_system_config(
        std::vector<WorldState::transform> additional_transforms =
            std::vector<WorldState::transform>());

    /// @brief Get the list of operations currently running on a robot.
    /// @return The list of operations currently running on the calling robot.
    std::vector<viam::robot::v1::Operation> get_operations();

    /// @brief Get the status of the requested robot components.
    /// @param components A list of the specific components for which status is desired.
    /// @return A list of statuses.
    std::vector<Status> get_status(std::vector<ResourceName>& components);

    /// @brief Get the status of all robot components.
    /// @return A list of statuses.
    std::vector<Status> get_status();

    std::vector<discovery> discover_components(std::vector<discoveryQuery> queries);

    /// @brief Transform a given `Pose` to a new specified destination which is a reference frame.
    /// @param query The pose that should be transformed.
    /// @param destination The name of the reference frame to transform the given pose to.
    /// @return the `pose_in_frame` of the transformed pose.
    pose_in_frame transform_pose(pose_in_frame query,
                                 std::string destination,
                                 std::vector<WorldState::transform> additional_transforms =
                                     std::vector<WorldState::transform>());

    /// @brief Blocks on the specified operation of the robot, returning when it is complete.
    /// @param id The ID of the operation to block on.
    void block_for_operation(std::string id);

    /// @brief Cancel all operations for the robot and stop all actuators and movement.
    void stop_all();

    /// @brief Cancel all operations for the robot and stop all actuators and movement.
    /// @param extra Any extra params to pass to resources' `stop` methods, keyed by `ResourceName`.
    void stop_all(std::unordered_map<ResourceName,
                                     std::unordered_map<std::string, std::shared_ptr<ProtoType>>,
                                     ResourceNameHasher,
                                     ResourceNameEqual> extra);

    /// @brief Cancel a specified operation on the robot.
    /// @param id The ID of the operation to cancel.
    void cancel_operation(std::string id);

   private:
    std::vector<std::shared_ptr<std::thread>> threads_;
    std::atomic<bool> should_refresh_;
    unsigned int refresh_interval_;
    std::shared_ptr<ViamChannel> viam_channel_;
    std::shared_ptr<Channel> channel_;
    bool should_close_channel_;
    std::unique_ptr<RobotService::Stub> stub_;
    std::mutex lock_;
    std::vector<ResourceName> resource_names_;
    ResourceManager resource_manager_;
    void refresh_every();
};

}  // namespace sdk
}  // namespace viam
