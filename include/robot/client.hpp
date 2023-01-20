#ifndef ROBOTCLIENT_H
#define ROBOTCLIENT_H

#include <common/utils.h>
#include <components/resource_manager.h>
#include <grpcpp/channel.h>
#include <registry/registry.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>
#include <rpc/dial.h>

#include <string>

using grpc::Channel;
using viam::common::v1::ResourceName;
using viam::common::v1::Transform;
using viam::robot::v1::FrameSystemConfig;
using viam::robot::v1::RobotService;
using viam::robot::v1::Status;
using Viam::SDK::Options;
using Viam::SDK::ViamChannel;

class RobotClient {
   public:
    void refresh();
    void close();
    static std::shared_ptr<RobotClient> at_address(std::string address, Options options);
    static std::shared_ptr<RobotClient> with_channel(ViamChannel channel, Options options);
    RobotClient(ViamChannel channel);
    std::vector<ResourceName>* resource_names();
    std::unique_ptr<RobotService::Stub> stub_;
    ComponentBase get_component(ResourceName name);
    std::vector<FrameSystemConfig> get_frame_system_config(
        std::vector<Transform> additional_transforms = std::vector<Transform>());
    std::vector<Status> get_status(
        std::vector<ResourceName> components = std::vector<ResourceName>());

   private:
    void stop_all();
    void stop_all(std::unordered_map<ResourceName,
                                     std::unordered_map<std::string, ProtoType>,
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
