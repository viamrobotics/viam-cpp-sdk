#pragma once

#include <unordered_map>

#include <grpcpp/grpcpp.h>

#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

AttributeMap fake_map();
std::vector<GeometryConfig> fake_geometries();

// TestServer is a wrapper around viam::sdk::Server that is a friend of the
// class and can therefore access its private fields.
class TestServer {
   public:
    TestServer(std::shared_ptr<Server> sdk_server);
    ~TestServer();

    std::shared_ptr<grpc::Channel> grpc_in_process_channel(const grpc::ChannelArguments& args);

   private:
    std::shared_ptr<Server> sdk_server_;
};

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
template <typename ClientType, typename F>
void client_to_mock_pipeline(std::shared_ptr<Resource> mock, F&& test_case) {
    // Create a ResourceManager. Add the mock resource to the ResourceManager.
    // Create a Server. Use the mock's API to create a resource-specific
    // server (like MotorServer) from the ResourceManager and Server. Start the
    // Server.
    auto rm = std::make_shared<ResourceManager>();
    rm->add(mock->name(), mock);
    auto server = std::make_shared<sdk::Server>();
    auto rs = sdk::Registry::lookup_resource(mock->api());
    // resource_server is unused; we call create_resource_server to call
    // register_service and associate the Server with the resource-specific
    // server (like MotorServer).
    auto resource_server = rs->create_resource_server(rm, *server);
    server->start();

    // Create a resource-specific client to the mock over an established
    // in-process gRPC channel.
    grpc::ChannelArguments args;
    auto test_server = TestServer(server);
    auto grpc_channel = test_server.grpc_in_process_channel(args);
    ClientType resource_client(mock->name(), grpc_channel);

    // Run the passed-in test case on the created stack and give access to the
    // created resource-specific client.
    std::forward<F>(test_case)(resource_client);

    // Shutdown Server afterward.
    server->shutdown();
}

}  // namespace sdktests
}  // namespace viam
