#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "servo.hpp"
#include "client.hpp"
#include "component/servo/v1/servo.pb.h"
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
using viam::component::servo::v1::MoveRequest;
using viam::component::servo::v1::MoveResponse;

ServoClient::ServoClient(std::string name_, std::shared_ptr<Channel> channel)
    : stub_(ServoService::NewStub(channel)) {
        name = name_;
    };

void ServoClient::move(int angle) {
    MoveRequest req;
    req.set_name(name);
    MoveResponse resp;
    grpc::ClientContext context;

    grpc::Status status = stub_->Move(&context, req, &resp);
};

int ServoClient::get_position() { return 0; };
// void stop() {};
bool ServoClient::is_moving() { return false; };

std::shared_ptr<ServoClient> ServoClient::create(std::string name_, std::shared_ptr<Channel> channel) {
    return std::make_shared<ServoClient>(name_, channel);
}

