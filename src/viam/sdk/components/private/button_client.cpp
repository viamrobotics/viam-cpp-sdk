#include <viam/sdk/components/private/button_client.hpp>

#include <grpcpp/channel.h>

#include <viam/api/component/button/v1/button.grpc.pb.h>
#include <viam/api/component/button/v1/button.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_value.hpp>

namespace viam {
namespace sdk {
namespace impl {

ButtonClient::ButtonClient(std::string name, const ViamChannel& channel)
    : Button(std::move(name)),
      stub_(viam::component::button::v1::ButtonService::NewStub(channel.channel())),
      channel_(&channel) {}

void ButtonClient::push(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Push).with(extra).invoke();
}

ProtoStruct ButtonClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
