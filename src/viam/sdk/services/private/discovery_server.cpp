#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/private/discovery_client.hpp>
#include <viam/sdk/services/private/discovery_server.hpp>

namespace viam {
namespace sdk {
namespace impl {

using namespace service::discovery::v1;

::grpc::Status DiscoveryServer::DiscoverResources(
    ::grpc::ServerContext*, const ::viam::service::discovery::v1::DiscoverResourcesRequest* request,
    ::viam::service::discovery::v1::DiscoverResourcesResponse* response) noexcept {
    return make_service_helper<Discovery>("DiscoveryServer::DiscoverResources", this,
                                          request)([&](auto& helper, auto& discovery) {
        const std::vector<ResourceConfig> resources =
            discovery->discover_resources(helper.getExtra());
        for (const auto& resource : resources) {
            *response->mutable_discoveries()->Add() = to_proto(resource);
        }
    });
}

::grpc::Status DiscoveryServer::DoCommand(
    ::grpc::ServerContext*, const ::viam::common::v1::DoCommandRequest* request,
    ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Discovery>("DiscoveryServer::DoCommand", this,
                                          request)([&](auto&, auto& discovery) {
        const ProtoStruct result = discovery->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
