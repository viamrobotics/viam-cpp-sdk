#include <viam/sdk/services/private/generic_client.hpp>

#include <utility>

#include <grpcpp/channel.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/service/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

GenericServiceClient::GenericServiceClient(std::string name, const ViamChannel& channel)
    : GenericService(std::move(name)),
      stub_(viam::service::generic::v1::GenericService::NewStub(channel.channel())),
      channel_(&channel) {}

ProtoStruct GenericServiceClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
