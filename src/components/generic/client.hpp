#pragma once

#include <grpcpp/channel.h>

#include <component/generic/v1/generic.grpc.pb.h>

#include <components/generic/generic.hpp>
#include <robot/client.hpp>

class GenericClient : public Generic {
   public:
    AttributeMap do_command(AttributeMap command) override;
    GenericClient(std::string name, std::shared_ptr<grpc::Channel> channel)
        : Generic(std::move(name)),
          stub_(viam::component::generic::v1::GenericService::NewStub(channel)),
          channel_(std::move(channel)){};

   protected:
    GenericClient(std::string name,
                  std::unique_ptr<viam::component::generic::v1::GenericService::StubInterface> stub)
        : Generic(std::move(name)), stub_(std::move(stub)){};

   private:
    std::unique_ptr<viam::component::generic::v1::GenericService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

