#include <viam/sdk/components/generic/client.hpp>

#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

GenericClient::GenericClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Generic(std::move(name)),
      stub_(viam::component::generic::v1::GenericService::NewStub(channel)),
      channel_(std::move(channel)){};

AttributeMap GenericClient::do_command(AttributeMap command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
};
std::vector<GeometryConfig> GenericClient::get_geometries() {
    return make_client_helper(this, *stub_, &StubType::GetGeometries).invoke([](auto& response) {
        return GeometryConfig::from_proto(response);
    });
};

}  // namespace sdk
}  // namespace viam
