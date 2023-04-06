#pragma once

#include <string>
#include <thread>

#include <grpcpp/channel.h>

#include <common/v1/common.pb.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>

#include <common/utils.hpp>
#include <components/component_base.hpp>
#include <registry/registry.hpp>
#include <resource/resource_base.hpp>
#include <resource/resource_manager.hpp>
#include <rpc/dial.hpp>
#include <services/service_base.hpp>

using grpc::Channel;
using viam::common::v1::ResourceName;
using viam::common::v1::Transform;
using viam::robot::v1::FrameSystemConfig;
using viam::robot::v1::RobotService;
using viam::robot::v1::Status;
using Viam::SDK::Options;
using Viam::SDK::ViamChannel;

/// gRPC client for a robot. This class should be used for all interactions with
/// a robot.
// TODO(RSDK-1742) replace all `ResourceName` references in API with `Name`
class RobotClient {
   public:
    ~RobotClient();
    void refresh();
    void close();
    /// Create a robot client that is connected to the robot at the provided
    /// address. Uses the underlying rust-utils library and webRTC to dial the
    /// address.
    ///
    /// Args:
    /// 	address: Address of the robot (IP address, URI, URL, etc.)
    /// 	options: Options for connecting and refreshing
    static std::shared_ptr<RobotClient> at_address(std::string address, Options options);
    /// Create a robot client that is connected to the robot at the provided
    /// local socket. Creates a direct connection to the robot using the `unix://`
    /// scheme. Only useful for connecting to robots across Unix sockets.
    ///
    /// Args:
    /// 	address: Local socket of the robot (a .sock file, etc.)
    /// 	options: Options for connecting and refreshing
    static std::shared_ptr<RobotClient> at_local_socket(std::string address, Options options);
    static std::shared_ptr<RobotClient> with_channel(ViamChannel channel, Options options);
    RobotClient(ViamChannel channel);
    std::vector<ResourceName>* resource_names();
    std::unique_ptr<RobotService::Stub> stub_;
    /// Gets a `shared_ptr` to a Resource Base.
    ///
    /// Args:
    /// 	name: the name of the resource
    /// 	type: the resource type (defaults to a generic `ResourceBase`)
    ///
    /// Raises:
    /// 	Throws an error if the requested resource doesn't exist or is of the wrong type.
    ///
    /// This method should not be called directly except in specific cases. The
    /// type-annotated `resource_by_name<T>(name)` overload is the preferred method
    /// for obtaining resources.
    ///
    /// Because the return type here is a `ResourceBase`, the user will need to manually
    /// cast to the desired type.
    std::shared_ptr<ResourceBase> resource_by_name(ResourceName name,
                                                   ResourceType type = {"resource"});
    template <typename T>
    std::shared_ptr<T> resource_by_name(const std::string& name) {
        ResourceName r;
        Subtype subtype = T::subtype();
        *r.mutable_namespace_() = subtype.type_namespace();
        *r.mutable_type() = subtype.resource_type();
        *r.mutable_subtype() = subtype.resource_subtype();
        *r.mutable_name() = std::move(name);

        auto resource = this->resource_by_name(std::move(r), subtype.resource_type());
        return std::dynamic_pointer_cast<T>(resource);
    }
    std::vector<FrameSystemConfig> get_frame_system_config(
        std::vector<Transform> additional_transforms = std::vector<Transform>());
    std::vector<viam::robot::v1::Operation> get_operations();
    std::vector<Status> get_status(
        std::vector<ResourceName> components = std::vector<ResourceName>());
    std::vector<viam::robot::v1::Discovery> discover_components(
        std::vector<viam::robot::v1::DiscoveryQuery> queries);

    std::vector<std::shared_ptr<std::thread>> threads;

    viam::common::v1::PoseInFrame transform_pose(
        viam::common::v1::PoseInFrame query,
        std::string destination,
        std::vector<Transform> additional_transforms = std::vector<Transform>());
    void block_for_operation(std::string id);
    void stop_all();
    void stop_all(std::unordered_map<ResourceName,
                                     std::unordered_map<std::string, std::shared_ptr<ProtoType>>,
                                     ResourceNameHasher,
                                     ResourceNameEqual> extra);
    void cancel_operation(std::string id);

   private:
    std::atomic<bool> should_refresh;
    unsigned int refresh_interval;
    // (RSDK-919): make use of should_close_channel
    bool should_close_channel;
    std::shared_ptr<Channel> channel;
    ViamChannel viam_channel;
    std::mutex lock;
    std::vector<ResourceName> resource_names_;
    ResourceManager resource_manager;
    void refresh_every();
};
