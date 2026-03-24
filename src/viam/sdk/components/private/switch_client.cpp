#include <viam/sdk/components/private/switch_client.hpp>

#include <grpcpp/channel.h>

#include <viam/api/component/switch/v1/switch.grpc.pb.h>
#include <viam/api/component/switch/v1/switch.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {
namespace impl {

SwitchClient::SwitchClient(std::string name, const ViamChannel& channel)
    : Switch(std::move(name)),
      stub_(viam::component::switch_::v1::SwitchService::NewStub(channel.channel())),
      channel_(&channel) {}

void SwitchClient::set_position(uint32_t position, const ProtoStruct& extra) {
    make_client_helper(this, *stub_, &StubType::SetPosition)
        .with(extra, [&](auto& request) { request.set_position(position); })
        .invoke();
}

uint32_t SwitchClient::get_position(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPosition)
        .with(extra)
        .invoke([](auto& response) { return response.position(); });
}

Switch::position_info SwitchClient::get_number_of_positions(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetNumberOfPositions)
        .with(extra)
        .invoke([](auto& response) {
            const auto& labels = response.labels();
            if (!labels.empty() &&
                static_cast<uint32_t>(labels.size()) != response.number_of_positions()) {
                throw Exception(
                    "get_number_of_positions: labels size does not match number_of_positions");
            }
            return Switch::position_info{response.number_of_positions(),
                                         {labels.begin(), labels.end()}};
        });
}

ProtoStruct SwitchClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

ProtoStruct SwitchClient::get_status() {
    return make_client_helper(this, *stub_, &StubType::GetStatus).invoke([](auto& response) {
        return from_proto(response.result());
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
