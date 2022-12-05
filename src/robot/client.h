#ifndef ROBOTCLIENT_H
#define ROBOTCLIENT_H

#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>

#include <string>

#include "../components/resource_manager.h"
#include "../registry/registry.h"
#include "../rpc/dial.h"
#include "grpcpp/channel.h"

using grpc::Channel;
using viam::common::v1::ResourceName;
using viam::robot::v1::RobotService;

class RobotClient {
       public:
	void refresh();
	void close();
	static std::shared_ptr<RobotClient> at_address(std::string address,
						       Options options);
	static std::shared_ptr<RobotClient> with_channel(ViamChannel channel,
							 Options options);
	RobotClient(ViamChannel channel);
	std::vector<ResourceName> *resource_names();
	std::unique_ptr<RobotService::Stub> stub_;
	ComponentBase get_component(ResourceName name);

       private:
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
