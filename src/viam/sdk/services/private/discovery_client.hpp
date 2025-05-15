/// @file services/private/discovery_client.hpp
///
/// @brief Implements a gRPC client for the `Discovery` service
#pragma once

#include <viam/api/service/discovery/v1/discovery.grpc.pb.h>

#include <viam/sdk/rpc/dial.hpp>
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
    DiscoveryClient(std::string name, ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    std::vector<ResourceConfig> discover_resources(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;

   private:
    using StubType = viam::service::discovery::v1::DiscoveryService::StubInterface;
    std::unique_ptr<StubType> stub_;
    ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
