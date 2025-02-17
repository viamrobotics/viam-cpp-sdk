#include <viam/sdk/services/private/discovery_client.hpp>

#include <viam/sdk/common/service_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

DiscoveryServer::DiscoveryServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status DiscoveryServer::DiscoverResources(
    ::grpc::ServerContext*,
    const ::viam::service::discovery::v1::DiscoverResourcesRequest* request,
    ::viam::service::discovery::v1::DiscoverResourcesResponse* response) noexcept {
    return make_service_helper<Discovery>(
        "DiscoveryServer::DiscoverResources", this, request)([&](auto& helper, auto& discovery) {
        const std::vector<ResourceConfig> resources =
            discovery->discover_resources(helper.getExtra());
        for (const auto& resource : resources) {
            *response->mutable_resources()->Add() = to_proto(resource);
        }
    });
}

::grpc::Status DiscoveryServer::DoCommand(
    ::grpc::ServerContext*,
    const ::viam::service::discovery::v1::DoCommandRequest* request,
    ::viam::service::discovery::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Discovery>(
        "DiscoveryServer::DoCommand", this, request)([&](auto& helper, auto& discovery) {
        const ProtoStruct result = discovery->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
