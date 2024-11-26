#include <viam/sdk/tests/test_utils.hpp>

#include <unordered_map>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/message_sizes.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

ProtoStruct fake_map() {
    return {{"test", "hello"}};
}

std::vector<GeometryConfig> fake_geometries() {
    pose p{1, 2, 3};
    return {GeometryConfig(p, sphere{1}, "sphere"),
            GeometryConfig(p, box{1, 2, 3}, "box"),
            GeometryConfig(p, sphere{0}, "point"),
            GeometryConfig(p, capsule{2, 4}, "capsule")};
}

TestServer::TestServer(std::shared_ptr<Server> sdk_server) : sdk_server_(std::move(sdk_server)) {}

TestServer::~TestServer() = default;

std::shared_ptr<grpc::Channel> TestServer::grpc_in_process_channel() {
    grpc::ChannelArguments args;
    args.SetMaxSendMessageSize(kMaxMessageSize);
    args.SetMaxReceiveMessageSize(kMaxMessageSize);
    return sdk_server_->server_->InProcessChannel(args);
}

}  // namespace sdktests
}  // namespace viam
