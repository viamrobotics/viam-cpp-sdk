#include <viam/sdk/services/private/generic_client.hpp>

#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/service/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

GenericServiceClient::GenericServiceClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : GenericService(std::move(name)),
      stub_(viam::service::generic::v1::GenericService::NewStub(channel)),
      channel_(std::move(channel)){};

AttributeMap GenericServiceClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace sdk
}  // namespace viam
