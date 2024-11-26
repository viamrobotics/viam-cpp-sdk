#include <viam/sdk/components/private/servo_client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/servo/v1/servo.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/components/servo.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

Servo::position from_proto(const viam::component::servo::v1::GetPositionResponse& proto) {
    return proto.position_deg();
}

ServoClient::ServoClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Servo(std::move(name)),
      stub_(viam::component::servo::v1::ServoService::NewStub(channel)),
      channel_(std::move(channel)) {}

void ServoClient::move(uint32_t angle_deg, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Move)
        .with(extra, [&](auto& request) { request.set_angle_deg(angle_deg); })
        .invoke();
}

Servo::position ServoClient::get_position(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPosition)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

void ServoClient::stop(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Stop).with(extra).invoke();
}

bool ServoClient::is_moving() {
    return make_client_helper(this, *stub_, &StubType::IsMoving).invoke([](auto& response) {
        return response.is_moving();
    });
}

std::vector<GeometryConfig> ServoClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return v2::from_proto(response); });
}

ProtoStruct ServoClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
