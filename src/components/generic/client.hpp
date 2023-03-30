#pragma once

#include <grpcpp/channel.h>

#include <component/generic/v1/generic.grpc.pb.h>

#include <components/generic/generic.hpp>

class GenericClient : public Generic {
   public:
    AttributeMap do_command(AttributeMap command) override;
    GenericClient(std::string name, std::shared_ptr<grpc::Channel> channel_);

   protected:
    GenericClient(std::string name);
    std::unique_ptr<viam::component::generic::v1::GenericService::StubInterface> stub_;
    std::string name_;

   private:
    std::shared_ptr<grpc::Channel> channel_;
};
