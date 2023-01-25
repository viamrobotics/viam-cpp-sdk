#ifndef ROBOTCLIENT_H
#define ROBOTCLIENT_H

#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>

#include <string>

#include "../common/utils.hpp"
#include "../components/resource_manager.hpp"
#include "../registry/registry.hpp"
#include "../rpc/dial.hpp"
#include "grpcpp/channel.h"

using grpc::Channel;
using viam::common::v1::ResourceName;
using viam::common::v1::Transform;
using viam::common::v1::PoseInFrame;
using viam::robot::v1::FrameSystemConfig;
using viam::robot::v1::RobotService;
using viam::robot::v1::Status;
using viam::robot::v1::Operation;
using viam::robot::v1::Discovery;
using viam::robot::v1::DiscoveryQuery;

class RobotClient {
   public:
    void refresh();
    void close();
    static std::shared_ptr<RobotClient> at_address(std::string address, Options options);
    static std::shared_ptr<RobotClient> with_channel(ViamChannel channel, Options options);
    RobotClient(ViamChannel channel);
    std::vector<ResourceName>* resource_names();
    std::unique_ptr<RobotService::Stub> stub_;
    std::shared_ptr<ComponentBase> get_component(ResourceName name);
    std::vector<FrameSystemConfig> get_frame_system_config(
        std::vector<Transform> additional_transforms = std::vector<Transform>());
    std::vector<Status> get_status(
        std::vector<ResourceName> components = std::vector<ResourceName>());

    std::vector<Operation> get_operations();
    void cancel_operation(std::string id);
    void block_for_operation(std::string id);


    PoseInFrame transform_pose(
        PoseInFrame query,
        std::string destination,
        std::vector<Transform> additional_transforms = std::vector<Transform>());
    std::vector<Discovery> discover_components(std::vector<DiscoveryQuery> queries);

   private:
    void stop_all();
    void stop_all(std::unordered_map<ResourceName,
                                     std::unordered_map<std::string, ProtoType*>,
                                     ResourceNameHasher,
                                     ResourceNameEqual> extra);
    std::atomic<bool> should_refresh;
    unsigned int refresh_interval;
    bool should_close_channel;
    std::shared_ptr<Channel> channel;
    ViamChannel viam_channel;
    std::mutex lock;
    std::vector<ResourceName> resource_names_;
    ResourceManager resource_manager;
    void refresh_every();
};

#endif
