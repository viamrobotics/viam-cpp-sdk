#include <viam/sdk/components/private/power_sensor_client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/power_sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

using namespace viam::component::powersensor::v1;

namespace viam {
namespace sdk {
namespace impl {

PowerSensorClient::PowerSensorClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : PowerSensor(std::move(name)),
      stub_(PowerSensorService::NewStub(channel)),
      channel_(std::move(channel)){};

PowerSensor::voltage PowerSensorClient::get_voltage(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetVoltage)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

PowerSensor::current PowerSensorClient::get_current(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetCurrent)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

double PowerSensorClient::get_power(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPower)
        .with(extra)
        .invoke([](auto& response) { return response.watts(); });
}

ProtoStruct PowerSensorClient::get_readings(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetReadings)
        .with(extra)
        .invoke([](auto& response) {
            ProtoStruct result;
            for (const auto& r : response.readings()) {
                result.emplace(r.first, ProtoValue::from_proto(r.second));
            }
            return result;
        });
}

ProtoStruct PowerSensorClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
