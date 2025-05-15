#include <viam/sdk/components/private/gripper_client.hpp>

#include <grpcpp/channel.h>

#include <viam/api/component/gripper/v1/gripper.grpc.pb.h>
#include <viam/api/component/gripper/v1/gripper.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {
namespace impl {

GripperClient::GripperClient(std::string name, ViamChannel& channel)
    : Gripper(std::move(name)),
      stub_(viam::component::gripper::v1::GripperService::NewStub(channel.channel())),
      channel_(&channel) {}

void GripperClient::open(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Open).with(extra).invoke();
}

bool GripperClient::grab(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Grab).with(extra).invoke([](auto& response) {
        return response.success();
    });
}

bool GripperClient::is_moving() {
    return make_client_helper(this, *stub_, &StubType::IsMoving).invoke([](auto& response) {
        return response.is_moving();
    });
}

void GripperClient::stop(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Stop).with(extra).invoke();
}

ProtoStruct GripperClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

std::vector<GeometryConfig> GripperClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
