#include <viam/sdk/components/private/sensor_client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/sensor/v1/sensor.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/config/resource.hpp>

namespace viam {
namespace sdk {
namespace impl {

SensorClient::SensorClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Sensor(std::move(name)),
      stub_(viam::component::sensor::v1::SensorService::NewStub(channel)),
      channel_(std::move(channel)){};

using namespace viam::common::v1;

ProtoStruct SensorClient::get_readings(const ProtoStruct& extra) {
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

ProtoStruct SensorClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

std::vector<GeometryConfig> SensorClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return GeometryConfig::from_proto(response); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
