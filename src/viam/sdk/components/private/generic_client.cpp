#include <viam/sdk/components/private/generic_client.hpp>

#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

GenericComponentClient::GenericComponentClient(std::string name,
                                               std::shared_ptr<grpc::Channel> channel)
    : GenericComponent(std::move(name)),
      stub_(viam::component::generic::v1::GenericService::NewStub(channel)),
      channel_(std::move(channel)) {}

ProtoStruct GenericComponentClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

std::vector<GeometryConfig> GenericComponentClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
