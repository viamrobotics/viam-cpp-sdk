#pragma once

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

    std::shared_ptr<grpc::Channel> grpc_in_process_channel();

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
template <typename ResourceType, typename F>
void client_to_mock_pipeline(std::shared_ptr<Resource> mock, F&& test_case) {
    auto server = std::make_shared<sdk::Server>();

    // normally the high level server service (either robot or module) handles adding managed
    // resources, but in this case we must do it ourselves.
    server->add_resource(mock);
    server->start();

    // Create a resource-specific client to the mock over an established
    // in-process gRPC channel.
    auto test_server = TestServer(server);
    auto grpc_channel = test_server.grpc_in_process_channel();

    auto resource_client = Registry::lookup_resource_client(API::get<ResourceType>())
                               ->create_rpc_client(mock->name(), std::move(grpc_channel));

    // Run the passed-in test case on the created stack and give access to the
    // created resource-specific client.
    std::forward<F>(test_case)(*std::dynamic_pointer_cast<ResourceType>(resource_client));

    // Shutdown Server afterward.
    server->shutdown();
}

}  // namespace sdktests
}  // namespace viam
