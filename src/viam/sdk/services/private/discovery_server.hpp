/// @file services/private/discovery_server.hpp
///
/// @brief Implements a gRPC server for the `Discovery` service
#pragma once

#include <viam/api/service/discovery/v1/discovery.grpc.pb.h>
#include <viam/api/service/discovery/v1/discovery.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/services/discovery.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class DiscoveryServer
/// @brief gRPC server implementation of a `Discovery` service.
/// @ingroup Discovery
class DiscoveryServer : public ResourceServer,
                        public viam::service::discovery::v1::DiscoveryService::Service {
   public:
    using interface_type = Discovery;
    using service_type = service::discovery::v1::DiscoveryService;
    explicit DiscoveryServer(std::shared_ptr<ResourceManager> manager)
        : ResourceServer(std::move(manager)) {}

    ::grpc::Status DiscoverResources(
        ::grpc::ServerContext* context,
        const ::viam::service::discovery::v1::DiscoverResourcesRequest* request,
        ::viam::service::discovery::v1::DiscoverResourcesResponse* response) noexcept override;
    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
