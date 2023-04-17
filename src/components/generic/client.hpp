/// @file components/generic/client.hpp
///
/// @brief Implements a gRPC client for the `Generic` component.
#pragma once

#include <grpcpp/channel.h>

#include <component/generic/v1/generic.grpc.pb.h>

#include <components/generic/generic.hpp>
#include <robot/client.hpp>

namespace viam {
namespace cppsdk {

/// @class GenericClient
/// @brief gRPC client implementation of a `Generic` component.
/// @ingroup Generic
class GenericClient : public Generic {
   public:
    AttributeMap do_command(AttributeMap command) override;
    GenericClient(std::string name, std::shared_ptr<grpc::Channel> channel)
        : Generic(std::move(name)),
          stub_(viam::component::generic::v1::GenericService::NewStub(channel)),
          channel_(std::move(channel)){};

   protected:
    // This constructor leaves the `channel_` as a nullptr. This is useful for testing
    // purposes, but renders it unusable for production use. Care should be taken to
    // avoid use of this constructor outside of tests.
    GenericClient(std::string name,
                  std::unique_ptr<viam::component::generic::v1::GenericService::StubInterface> stub)
        : Generic(std::move(name)), stub_(std::move(stub)){};

   private:
    std::unique_ptr<viam::component::generic::v1::GenericService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace cppsdk
}  // namespace viam
