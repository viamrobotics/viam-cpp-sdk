#include <viam/sdk/components/motor/motor.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/motor/v1/motor.grpc.pb.h>
#include <viam/api/component/motor/v1/motor.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/motor/client.hpp>
#include <viam/sdk/components/motor/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

MotorSubtype::~MotorSubtype() = default;
std::shared_ptr<ResourceServerBase> MotorSubtype::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<MotorServer>(manager);
};

std::shared_ptr<ResourceBase> MotorSubtype::create_rpc_client(std::string name,
                                                              std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<MotorClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceSubtype> Motor::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::motor::v1::MotorService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the motor service");
    }
    return std::make_shared<MotorSubtype>(sd);
}

Subtype Motor::subtype() {
    return Subtype(RDK, COMPONENT, "motor");
}

Motor::position Motor::from_proto(viam::component::motor::v1::GetPositionResponse proto) {
    return proto.position();
}

Motor::power_status Motor::from_proto(viam::component::motor::v1::IsPoweredResponse proto) {
    Motor::power_status power_status;
    power_status.is_on = proto.is_on();

    power_status.power_pct = proto.power_pct();
    return power_status;
}

Motor::properties Motor::from_proto(viam::component::motor::v1::GetPropertiesResponse proto) {
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

bool operator==(const Motor::power_status& lhs, const Motor::power_status& rhs) {
    return (lhs.is_on == rhs.is_on && lhs.power_pct == rhs.power_pct);
}

bool operator==(const Motor::properties& lhs, const Motor::properties& rhs) {
    return (lhs.position_reporting == rhs.position_reporting);
}

namespace {
bool init() {
    Registry::register_subtype(Motor::subtype(), Motor::resource_subtype());
    return true;
};

bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
