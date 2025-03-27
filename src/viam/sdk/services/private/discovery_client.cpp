#include <viam/api/service/discovery/v1/discovery.grpc.pb.h>
#include <viam/api/service/discovery/v1/discovery.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/discovery.hpp>
#include <viam/sdk/services/private/discovery_client.hpp>

namespace viam {
namespace sdk {
namespace impl {

DiscoveryClient::DiscoveryClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Discovery(std::move(name)),
      stub_(viam::service::discovery::v1::DiscoveryService::NewStub(channel)),
      channel_(std::move(channel)) {}

std::vector<ResourceConfig> DiscoveryClient::discover_resources(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::DiscoverResources)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

ProtoStruct DiscoveryClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
