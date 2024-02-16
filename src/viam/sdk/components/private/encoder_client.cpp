#include <viam/sdk/components/encoder/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/encoder/encoder.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

EncoderClient::EncoderClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Encoder(std::move(name)),
      stub_(viam::component::encoder::v1::EncoderService::NewStub(channel)),
      channel_(std::move(channel)){};

Encoder::position EncoderClient::get_position(const AttributeMap& extra,
                                              position_type position_type) {
    return make_client_helper(this, *stub_, &StubType::GetPosition)
        .with(extra, [&](auto& request) { request.set_position_type(to_proto(position_type)); })
        .invoke([](auto& response) { return from_proto(response); });
}

void EncoderClient::reset_position(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::ResetPosition).with(extra).invoke();
}

Encoder::properties EncoderClient::get_properties(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

std::vector<GeometryConfig> EncoderClient::get_geometries(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return GeometryConfig::from_proto(response); });
};

AttributeMap EncoderClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace sdk
}  // namespace viam
