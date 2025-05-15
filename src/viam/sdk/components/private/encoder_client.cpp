#include <viam/sdk/components/private/encoder_client.hpp>

#include <grpcpp/channel.h>

#include <memory>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/encoder.hpp>
#include <viam/sdk/components/private/encoder.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

using sdk::from_proto;
using sdk::to_proto;

viam::component::encoder::v1::GetPositionResponse to_proto(const Encoder::position& position) {
    viam::component::encoder::v1::GetPositionResponse proto;
    proto.set_value(position.value);

    proto.set_position_type(to_proto(position.type));
    return proto;
}

Encoder::position from_proto(const viam::component::encoder::v1::GetPositionResponse& proto) {
    Encoder::position position;
    position.value = proto.value();

    position.type = from_proto(proto.position_type());
    return position;
}

Encoder::properties from_proto(const viam::component::encoder::v1::GetPropertiesResponse& proto) {
    Encoder::properties properties;
    properties.ticks_count_supported = proto.ticks_count_supported();

    properties.angle_degrees_supported = proto.angle_degrees_supported();
    return properties;
}

EncoderClient::EncoderClient(std::string name, ViamChannel& channel)
    : Encoder(std::move(name)),
      stub_(viam::component::encoder::v1::EncoderService::NewStub(channel.channel())),
      channel_(&channel) {}

Encoder::position EncoderClient::get_position(const ProtoStruct& extra,
                                              position_type position_type) {
    return make_client_helper(this, *stub_, &StubType::GetPosition)
        .with(extra, [&](auto& request) { request.set_position_type(to_proto(position_type)); })
        .invoke([](auto& response) { return from_proto(response); });
}

void EncoderClient::reset_position(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::ResetPosition).with(extra).invoke();
}

Encoder::properties EncoderClient::get_properties(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

std::vector<GeometryConfig> EncoderClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
};

ProtoStruct EncoderClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
