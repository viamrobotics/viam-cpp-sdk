#include <viam/sdk/components/motor/motor.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/motor/v1/motor.grpc.pb.h>
#include <viam/api/component/motor/v1/motor.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

Motor::position Motor::from_proto(const viam::component::motor::v1::GetPositionResponse& proto) {
    return proto.position();
}
API Motor::api() const {
    return API::get<Motor>();
}

API API::traits<Motor>::api() {
    return {kRDK, kComponent, "motor"};
}

Motor::power_status Motor::from_proto(const viam::component::motor::v1::IsPoweredResponse& proto) {
    Motor::power_status power_status;
    power_status.is_on = proto.is_on();

    power_status.power_pct = proto.power_pct();
    return power_status;
}

Motor::properties Motor::from_proto(
    const viam::component::motor::v1::GetPropertiesResponse& proto) {
    Motor::properties properties;
    properties.position_reporting = proto.position_reporting();
    return properties;
}

viam::component::motor::v1::GetPositionResponse Motor::to_proto(position position) {
    viam::component::motor::v1::GetPositionResponse proto;
    proto.set_position(position);
    return proto;
}

viam::component::motor::v1::IsPoweredResponse Motor::to_proto(power_status power_status) {
    viam::component::motor::v1::IsPoweredResponse proto;
    proto.set_is_on(power_status.is_on);

    proto.set_power_pct(power_status.power_pct);
    return proto;
}

viam::component::motor::v1::GetPropertiesResponse Motor::to_proto(properties properties) {
    viam::component::motor::v1::GetPropertiesResponse proto;
    proto.set_position_reporting(properties.position_reporting);
    return proto;
}

Motor::Motor(std::string name) : Component(std::move(name)){};

bool operator==(const Motor::power_status& lhs, const Motor::power_status& rhs) {
    return (lhs.is_on == rhs.is_on && lhs.power_pct == rhs.power_pct);
}

bool operator==(const Motor::properties& lhs, const Motor::properties& rhs) {
    return (lhs.position_reporting == rhs.position_reporting);
}

}  // namespace sdk
}  // namespace viam
