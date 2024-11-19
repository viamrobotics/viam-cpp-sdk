#include <viam/sdk/components/private/power_sensor_server.hpp>

#include <viam/sdk/common/service_helper.hpp>
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
    : ResourceServer(std::move(manager)){};

::grpc::Status PowerSensorServer::GetVoltage(::grpc::ServerContext*,
                                             const GetVoltageRequest* request,
                                             GetVoltageResponse* response) noexcept {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetVoltage", this, request)([&](auto& helper, auto& powersensor) {
        const PowerSensor::voltage result = powersensor->get_voltage(helper.getExtra());
        *response = to_proto(result);
    });
}

::grpc::Status PowerSensorServer::GetCurrent(::grpc::ServerContext*,
                                             const GetCurrentRequest* request,
                                             GetCurrentResponse* response) noexcept {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetCurrent", this, request)([&](auto& helper, auto& powersensor) {
        const PowerSensor::current result = powersensor->get_current(helper.getExtra());
        *response = to_proto(result);
    });
}

::grpc::Status PowerSensorServer::GetPower(::grpc::ServerContext*,
                                           const GetPowerRequest* request,
                                           GetPowerResponse* response) noexcept {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetPower", this, request)([&](auto& helper, auto& powersensor) {
        const double watts = powersensor->get_power(helper.getExtra());
        response->set_watts(watts);
    });
}

::grpc::Status PowerSensorServer::GetReadings(
    ::grpc::ServerContext*,
    const viam::common::v1::GetReadingsRequest* request,
    viam::common::v1::GetReadingsResponse* response) noexcept {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetReadings", this, request)([&](auto& helper, auto& powersensor) {
        const ProtoStruct result = powersensor->get_readings(helper.getExtra());
        for (const auto& r : result) {
            response->mutable_readings()->insert({r.first, to_proto(r.second)});
        }
    });
}

::grpc::Status PowerSensorServer::DoCommand(
    grpc::ServerContext*,
    const viam::common::v1::DoCommandRequest* request,
    viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::DoCommand", this, request)([&](auto&, auto& powersensor) {
        const ProtoStruct result = powersensor->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
