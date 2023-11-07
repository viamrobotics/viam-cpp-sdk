#include <viam/sdk/components/power_sensor/server.hpp>

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
    const auto status = server_wrapper<PowerSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    PowerSensor::voltage result = status.powersensor->get_voltage(status.extra);
    *response = PowerSensor::to_proto(result);

    return ::grpc::Status();
}

::grpc::Status PowerSensorServer::GetCurrent(::grpc::ServerContext* context,
                                             const GetCurrentRequest* request,
                                             GetCurrentResponse* response) {
    const auto status = server_wrapper<PowerSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    PowerSensor::current result = status.powersensor->get_current(status.extra);
    *response = PowerSensor::to_proto(result);

    return ::grpc::Status();
}

::grpc::Status PowerSensorServer::GetPower(::grpc::ServerContext* context,
                                           const GetPowerRequest* request,
                                           GetPowerResponse* response) {
    const auto status = server_wrapper<PowerSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    double watts = status.powersensor->get_power(status.extra);
    response->set_watts(watts);

    return ::grpc::Status();
}

::grpc::Status PowerSensorServer::GetReadings(::grpc::ServerContext* context,
                                              const viam::common::v1::GetReadingsRequest* request,
                                              viam::common::v1::GetReadingsResponse* response) {
    const auto status = server_wrapper<PowerSensor>(request);
    if (!status.ok()) {
        return status.status;
    }

    const AttributeMap result = status.powersensor->get_readings(status.extra);
    for (const auto& r : *result) {
        response->mutable_readings()->insert({std::move(r.first), r.second->proto_value()});
    }
    return ::grpc::Status();
}

::grpc::Status PowerSensorServer::DoCommand(grpc::ServerContext* context,
                                            const viam::common::v1::DoCommandRequest* request,
                                            viam::common::v1::DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [PowerSensor::DoCommand] without a request");
    };

    auto rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<PowerSensor> sensor = std::dynamic_pointer_cast<PowerSensor>(rb);
    const AttributeMap result = sensor->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

void PowerSensorServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
