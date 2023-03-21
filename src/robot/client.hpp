#pragma once

#include <common/v1/common.pb.h>
#include <grpcpp/channel.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>

#include <common/utils.hpp>
#include <registry/registry.hpp>
#include <resource/resource_manager.hpp>
#include <rpc/dial.hpp>
#include <string>
#include <thread>

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
    /// address.
    ///
    /// Args:
    /// 	address: Address of the robot (IP address, URI, URL, etc.)
    /// 	options: Options for connecting and refreshing
    static std::shared_ptr<RobotClient> at_address(std::string address, Options options);
    static std::shared_ptr<RobotClient> at_local_socket(std::string address, Options options);
    static std::shared_ptr<RobotClient> with_channel(ViamChannel channel, Options options);
    RobotClient(ViamChannel channel);
    std::vector<ResourceName>* resource_names();
    std::unique_ptr<RobotService::Stub> stub_;
    std::shared_ptr<ResourceBase> resource_by_name(ResourceName name);
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
                                     std::unordered_map<std::string, ProtoType*>,
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

