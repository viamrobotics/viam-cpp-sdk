#include <viam/sdk/components/private/motor_client.hpp>

#include <memory>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/motor/v1/motor.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/private/proto_conversions.hpp>
#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

Motor::position from_proto(const viam::component::motor::v1::GetPositionResponse& proto) {
    return proto.position();
}

Motor::power_status from_proto(const viam::component::motor::v1::IsPoweredResponse& proto) {
    Motor::power_status power_status;
    power_status.is_on = proto.is_on();

    power_status.power_pct = proto.power_pct();
    return power_status;
}

Motor::properties from_proto(const viam::component::motor::v1::GetPropertiesResponse& proto) {
    Motor::properties properties;
    properties.position_reporting = proto.position_reporting();
    return properties;
}

MotorClient::MotorClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Motor(std::move(name)),
      stub_(viam::component::motor::v1::MotorService::NewStub(channel)),
      channel_(std::move(channel)) {};

void MotorClient::set_power(double power_pct, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::SetPower)
        .with(extra, [&](auto& request) { request.set_power_pct(power_pct); })
        .invoke();
}

void MotorClient::go_for(double rpm, double revolutions, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GoFor)
        .with(extra,
              [&](auto& request) {
                  request.set_rpm(rpm);
                  request.set_revolutions(revolutions);
              })
        .invoke();
}

void MotorClient::go_to(double rpm, double position_revolutions, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GoTo)
        .with(extra,
              [&](auto& request) {
                  request.set_rpm(rpm);
                  request.set_position_revolutions(position_revolutions);
              })
        .invoke();
}

void MotorClient::set_rpm(double rpm, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::SetRPM)
        .with(extra, [&](auto& request) { request.set_rpm(rpm); })
        .invoke();
}

void MotorClient::reset_zero_position(double offset, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::ResetZeroPosition)
        .with(extra, [&](auto& request) { request.set_offset(offset); })
        .invoke();
}

Motor::position MotorClient::get_position(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPosition)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

Motor::properties MotorClient::get_properties(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

void MotorClient::stop(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Stop).with(extra).invoke();
}

Motor::power_status MotorClient::get_power_status(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::IsPowered)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

std::vector<GeometryConfig> MotorClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

bool MotorClient::is_moving() {
    return make_client_helper(this, *stub_, &StubType::IsMoving).invoke([](auto& response) {
        return response.is_moving();
    });
}

ProtoStruct MotorClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
