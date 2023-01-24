#pragma once

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "servo.hpp"
#include "component/servo/v1/servo.grpc.pb.h"

using grpc::Channel;
// using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
// using grpc::CompletionQueue;
// using grpc::Status;
// using viam::component::servo::v1::ResourceName;
// using helloworld::HelloReply;
// using helloworld::HelloRequest;
using viam::component::servo::v1::ServoService;

class ServoClient: public Servo {
    public:
        explicit ServoClient(std::string name_, std::shared_ptr<Channel> channel);
        void move(int angle);
        int get_position();
        // void stop() = 0;
        bool is_moving();

        static std::shared_ptr<ServoClient> create(std::string name_, std::shared_ptr<Channel> channel);

    private:
        std::unique_ptr<ServoService::Stub> stub_;
};
