#include <viam/sdk/components/private/sensor_client.hpp>

#include <grpcpp/channel.h>

#include <memory>
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

SensorClient::SensorClient(std::string name, const ViamChannel& channel)
    : Sensor(std::move(name)),
      stub_(viam::component::sensor::v1::SensorService::NewStub(channel.channel())),
      channel_(&channel) {}

using namespace viam::common::v1;

ProtoStruct SensorClient::get_readings(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetReadings)
        .with(extra)
        .invoke([](auto& response) {
            ProtoStruct result;
            for (const auto& r : response.readings()) {
                result.emplace(r.first, from_proto(r.second));
            }
            return result;
        });
}

ProtoStruct SensorClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

std::vector<GeometryConfig> SensorClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
