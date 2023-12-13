#include <viam/sdk/components/sensor/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/sensor/v1/sensor.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/sensor/sensor.hpp>
#include <viam/sdk/config/resource.hpp>

namespace viam {
namespace sdk {

SensorClient::SensorClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Sensor(std::move(name)),
      stub_(viam::component::sensor::v1::SensorService::NewStub(channel)),
      channel_(std::move(channel)){};

using namespace viam::common::v1;

AttributeMap SensorClient::get_readings(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetReadings)
        .with(extra)
        .invoke([](auto& response) {
            AttributeMap result =
                std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
            for (const auto& r : response.readings()) {
                result->emplace(std::move(r.first), std::make_shared<ProtoType>(r.second));
            }
            return result;
        });
}

AttributeMap SensorClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

std::vector<GeometryConfig> SensorClient::get_geometries(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return GeometryConfig::from_proto(response); });
}

}  // namespace sdk
}  // namespace viam
