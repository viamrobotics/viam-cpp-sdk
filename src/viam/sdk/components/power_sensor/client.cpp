#include <viam/sdk/components/power_sensor/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.grpc.pb.h>

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
    GetVoltageResponse resp;
    stub_wrapper<GetVoltageRequest>(this, resp, extra, &PowerSensorClient::Stub::GetVoltage);
    return from_proto(resp);
}

PowerSensor::current PowerSensorClient::get_current(const AttributeMap& extra) {
    GetCurrentResponse resp;
    stub_wrapper<GetCurrentRequest>(this, resp, extra, &PowerSensorClient::Stub::GetCurrent);
    return from_proto(resp);
}

double PowerSensorClient::get_power(const AttributeMap& extra) {
    GetPowerResponse resp;
    stub_wrapper<GetPowerRequest>(this, resp, extra, &PowerSensorClient::Stub::GetPower);
    return resp.watts();
}

AttributeMap PowerSensorClient::get_readings(const AttributeMap& extra) {
    viam::common::v1::GetReadingsResponse response;
    stub_wrapper<viam::common::v1::GetReadingsRequest>(
        this, response, {}, &PowerSensorClient::Stub::GetReadings);

    AttributeMap result =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    for (const auto& r : response.readings()) {
        result->emplace(r.first, std::make_shared<ProtoType>(r.second));
    }
    return result;
}

AttributeMap PowerSensorClient::do_command(const AttributeMap& command) {
    viam::common::v1::DoCommandRequest request;
    viam::common::v1::DoCommandResponse response;

    grpc::ClientContext ctx;

    const google::protobuf::Struct proto_command = map_to_struct(command);
    *request.mutable_command() = proto_command;
    *request.mutable_name() = this->name();

    const auto status = stub_->DoCommand(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return struct_to_map(response.result());
}

}  // namespace sdk
}  // namespace viam
