/// @file services/private/discovery_client.hpp
///
/// @brief Implements a gRPC client for the `Discovery` service
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/service/discovery/v1/discovery.grpc.pb.h>

#include <viam/sdk/services/discovery.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class DiscoveryClient
/// @brief gRPC client implementation of a `Discovery` service.
/// @ingroup Discovery
class DiscoveryClient : public Discovery {
   public:
    using interface_type = Discovery;
    DiscoveryClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    std::vector<ResourceConfig> discover_resources(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;

    // Using declarations to introduce convenience overloads of interface which do not need to be
    // passed the ProtoStruct parameter.
    using Discovery::discover_resources;

   private:
    using StubType = viam::service::discovery::v1::DiscoveryService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
