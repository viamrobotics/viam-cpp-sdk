#include <viam/sdk/components/power_sensor.hpp>

#include <viam/api/component/powersensor/v1/powersensor.grpc.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

using namespace viam::component::powersensor::v1;

namespace viam {
namespace sdk {

API PowerSensor::api() const {
    return API::get<PowerSensor>();
}

API API::traits<PowerSensor>::api() {
    return {kRDK, kComponent, "power_sensor"};
}

PowerSensor::voltage PowerSensor::from_proto(const GetVoltageResponse& proto) {
    PowerSensor::voltage v;
    v.volts = proto.volts();
    v.is_ac = proto.is_ac();
    return v;
}

PowerSensor::current PowerSensor::from_proto(const GetCurrentResponse& proto) {
    PowerSensor::current c;
    c.amperes = proto.amperes();
    c.is_ac = proto.is_ac();
    return c;
}

GetVoltageResponse PowerSensor::to_proto(const voltage& v) {
    GetVoltageResponse proto;
    proto.set_volts(v.volts);
    proto.set_is_ac(v.is_ac);
    return proto;
}

GetCurrentResponse PowerSensor::to_proto(const current& c) {
    GetCurrentResponse proto;
    proto.set_amperes(c.amperes);
    proto.set_is_ac(c.is_ac);
    return proto;
}

bool operator==(const PowerSensor::voltage& lhs, const PowerSensor::voltage& rhs) {
    return (lhs.volts == rhs.volts && lhs.is_ac == rhs.is_ac);
}

bool operator==(const PowerSensor::current& lhs, const PowerSensor::current& rhs) {
    return (lhs.amperes == rhs.amperes && lhs.is_ac == rhs.is_ac);
}

}  // namespace sdk
}  // namespace viam
