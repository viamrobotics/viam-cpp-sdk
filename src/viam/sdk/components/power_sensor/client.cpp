#include <viam/sdk/components/power_sensor/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/power_sensor/power_sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

using namespace viam::component::powersensor::v1;

namespace viam {
namespace sdk {

PowerSensorClient::PowerSensorClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : PowerSensor(std::move(name)),
      stub_(PowerSensorService::NewStub(channel)),
      channel_(std::move(channel)){};

PowerSensor::voltage PowerSensorClient::get_voltage(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &PowerSensorClient::Stub::GetVoltage)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

PowerSensor::current PowerSensorClient::get_current(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &PowerSensorClient::Stub::GetCurrent)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

double PowerSensorClient::get_power(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &PowerSensorClient::Stub::GetPower)
        .with(extra)
        .invoke([](auto& response) { return response.watts(); });
}

AttributeMap PowerSensorClient::get_readings(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &PowerSensorClient::Stub::GetReadings)
        .with(extra)
        .invoke([](auto& response) {
            AttributeMap result =
                std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
            for (const auto& r : response.readings()) {
                result->emplace(r.first, std::make_shared<ProtoType>(r.second));
            }
            return result;
        });
}

AttributeMap PowerSensorClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &PowerSensorClient::Stub::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace sdk
}  // namespace viam
