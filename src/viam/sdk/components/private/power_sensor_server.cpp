#include <viam/sdk/components/private/power_sensor_server.hpp>

#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/power_sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

using namespace viam::component::powersensor::v1;

namespace viam {
namespace sdk {
namespace impl {

GetVoltageResponse to_proto(const PowerSensor::voltage& v) {
    GetVoltageResponse proto;
    proto.set_volts(v.volts);
    proto.set_is_ac(v.is_ac);
    return proto;
}

GetCurrentResponse to_proto(const PowerSensor::current& c) {
    GetCurrentResponse proto;
    proto.set_amperes(c.amperes);
    proto.set_is_ac(c.is_ac);
    return proto;
}

PowerSensorServer::PowerSensorServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status PowerSensorServer::GetVoltage(::grpc::ServerContext* context,
                                             const GetVoltageRequest* request,
                                             GetVoltageResponse* response) noexcept {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetVoltage", this, context, request)([&](auto& helper, auto& powersensor) {
        const PowerSensor::voltage result = powersensor->get_voltage(helper.getExtra());
        *response = to_proto(result);
    });
}

::grpc::Status PowerSensorServer::GetCurrent(::grpc::ServerContext* context,
                                             const GetCurrentRequest* request,
                                             GetCurrentResponse* response) noexcept {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetCurrent", this, context, request)([&](auto& helper, auto& powersensor) {
        const PowerSensor::current result = powersensor->get_current(helper.getExtra());
        *response = to_proto(result);
    });
}

::grpc::Status PowerSensorServer::GetPower(::grpc::ServerContext* context,
                                           const GetPowerRequest* request,
                                           GetPowerResponse* response) noexcept {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetPower", this, context, request)([&](auto& helper, auto& powersensor) {
        const double watts = powersensor->get_power(helper.getExtra());
        response->set_watts(watts);
    });
}

::grpc::Status PowerSensorServer::GetReadings(
    ::grpc::ServerContext* context,
    const viam::common::v1::GetReadingsRequest* request,
    viam::common::v1::GetReadingsResponse* response) noexcept {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetReadings", this, context, request)([&](auto& helper, auto& powersensor) {
        *(response->mutable_readings()) =
            to_proto(powersensor->get_readings(helper.getExtra())).fields();
    });
}

::grpc::Status PowerSensorServer::DoCommand(
    grpc::ServerContext* context,
    const viam::common::v1::DoCommandRequest* request,
    viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::DoCommand", this, context, request)([&](auto&, auto& powersensor) {
        const ProtoStruct result = powersensor->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
