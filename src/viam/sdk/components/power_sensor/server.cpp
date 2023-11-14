#include <viam/sdk/components/power_sensor/server.hpp>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/power_sensor/power_sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

using namespace viam::component::powersensor::v1;

namespace viam {
namespace sdk {

PowerSensorServer::PowerSensorServer() : ResourceServer(std::make_shared<ResourceManager>()){};
PowerSensorServer::PowerSensorServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(manager){};

::grpc::Status PowerSensorServer::GetVoltage(::grpc::ServerContext* context,
                                             const GetVoltageRequest* request,
                                             GetVoltageResponse* response) {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetVoltage", this, request)([&](auto& helper, auto& powersensor) {
        const PowerSensor::voltage result = powersensor->get_voltage(helper.getExtra());
        *response = PowerSensor::to_proto(result);
    });
}

::grpc::Status PowerSensorServer::GetCurrent(::grpc::ServerContext* context,
                                             const GetCurrentRequest* request,
                                             GetCurrentResponse* response) {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetCurrent", this, request)([&](auto& helper, auto& powersensor) {
        const PowerSensor::current result = powersensor->get_current(helper.getExtra());
        *response = PowerSensor::to_proto(result);
    });
}

::grpc::Status PowerSensorServer::GetPower(::grpc::ServerContext* context,
                                           const GetPowerRequest* request,
                                           GetPowerResponse* response) {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetPower", this, request)([&](auto& helper, auto& powersensor) {
        const double watts = powersensor->get_power(helper.getExtra());
        response->set_watts(watts);
    });
}

::grpc::Status PowerSensorServer::GetReadings(::grpc::ServerContext* context,
                                              const viam::common::v1::GetReadingsRequest* request,
                                              viam::common::v1::GetReadingsResponse* response) {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::GetReadings", this, request)([&](auto& helper, auto& powersensor) {
        const AttributeMap result = powersensor->get_readings(helper.getExtra());
        for (const auto& r : *result) {
            response->mutable_readings()->insert({std::move(r.first), r.second->proto_value()});
        }
    });
}

::grpc::Status PowerSensorServer::DoCommand(grpc::ServerContext* context,
                                            const viam::common::v1::DoCommandRequest* request,
                                            viam::common::v1::DoCommandResponse* response) {
    return make_service_helper<PowerSensor>(
        "PowerSensorServer::DoCommand", this, request)([&](auto& helper, auto& powersensor) {
        const AttributeMap result = powersensor->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

void PowerSensorServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
