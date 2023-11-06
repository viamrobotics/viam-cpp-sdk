#include <viam/sdk/components/sensor/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/sensor/v1/sensor.grpc.pb.h>

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
    GetReadingsResponse response;
    stub_wrapper<GetReadingsRequest>(this, response, {}, &SensorClient::Stub::GetReadings);

    AttributeMap result =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    for (const auto& r : response.readings()) {
        result->emplace(std::move(r.first), std::make_shared<ProtoType>(r.second));
    }
    return result;
}

AttributeMap SensorClient::do_command(const AttributeMap& command) {
    DoCommandRequest request;
    DoCommandResponse response;

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

std::vector<GeometryConfig> SensorClient::get_geometries(const AttributeMap& extra) {
    GetGeometriesResponse resp;
    stub_wrapper<GetGeometriesRequest>(this, resp, extra, &SensorClient::Stub::GetGeometries);
    return GeometryConfig::from_proto(resp);
}

}  // namespace sdk
}  // namespace viam
