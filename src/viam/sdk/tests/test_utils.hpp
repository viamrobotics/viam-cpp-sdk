#pragma once

#include <unordered_map>

#include <grpcpp/grpcpp.h>

#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

AttributeMap fake_map();
std::vector<GeometryConfig> fake_geometries();

// This function sets up the following architecture:
// -- ResourceClient
//        /\
//
//        | (grpc InProcessChannel)
//
//        \/
// -- ResourceServer
//        /\
//
//        | (function calls)
//
//        \/
// -- MockResource
//
// This is as close to a real client->server->resource setup as we can get
// without starting another process.
//
// The passed in test_case function will have access to the created ResourceClient.
template <typename ClientType, typename ServerType, typename F>
void client_to_mock_pipeline(std::shared_ptr<Resource> mock, F&& test_case) {
    // Create a viam RPC server. Create a resource-specific server with that
    // viam RPC server. Add the mock resource to the resource-specific server,
    // and start the RPC server.
    std::shared_ptr<sdk::Server> server = std::make_shared<sdk::Server>();
    ServerType resource_server(server);
    resource_server.resource_manager()->add(mock->name(), mock);
    server->start();

    // Create a resource-specific client to the mock over an established
    // in-process gRPC channel.
    grpc::ChannelArguments args;
    std::shared_ptr<grpc::Channel> grpc_channel = server->grpc_in_process_channel(args);
    ClientType client(mock->name(), grpc_channel);

    // Run the passed-in test case on the created stack and give access to the
    // created resource-specific client.
    std::forward<F>(test_case)(client);

    // Shutdown viam RPC server afterward.
    server->shutdown();
}

}  // namespace sdktests
}  // namespace viam
