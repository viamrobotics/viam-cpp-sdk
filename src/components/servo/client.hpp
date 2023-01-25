#pragma once

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "servo.hpp"
#include "component/servo/v1/servo.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using viam::component::servo::v1::ServoService;

class ServoClient: public Servo {
    public:
        explicit ServoClient(std::string name_, std::shared_ptr<Channel> channel);
        static std::shared_ptr<ServoClient> from_robot(std::shared_ptr<RobotClient> robot, std::string name);
        void move(int angle);
        int get_position();
        // void stop() = 0;
        bool is_moving();

        static std::shared_ptr<ServoClient> create(std::string name_, std::shared_ptr<Channel> channel);

    private:
        std::unique_ptr<ServoService::Stub> stub_;
};
