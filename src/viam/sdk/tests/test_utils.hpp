#pragma once

#include <grpcpp/grpcpp.h>

#include <boost/test/unit_test.hpp>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdktests {

struct GlobalFixture {
    GlobalFixture() {
        (void)sdk::Instance::current(sdk::Instance::Creation::if_needed);
    }
};

BOOST_TEST_GLOBAL_FIXTURE(GlobalFixture);

// Buffer output filter to test console logging.
// In practice this is a pain and makes it hard to inspect Boost.Test output,
// so rather than using it as a test fixture we manually instantiate it and call release when done.
// https://stackoverflow.com/a/5405268
struct cout_redirect {
    cout_redirect() : old(std::cout.rdbuf(os.rdbuf())) {}

    void release() {
        std::cout.rdbuf(old);
    }

    ~cout_redirect() {
        release();
    }

    std::ostringstream os;

   private:
    std::streambuf* old;
};

using namespace viam::sdk;

ProtoStruct fake_map();
ProtoStruct fake_status();
std::vector<GeometryConfig> fake_geometries();
std::vector<ResourceConfig> fake_discovered_resources();

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
// -- (test_case closure)
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
// The test_case receives the raw in-process gRPC channel. It may build a typed
// resource client from it (see client_to_mock_pipeline) or a raw service stub
// to drive malformed requests a typed client would never emit -- the latter is
// how server-side validation and protocol enforcement get exercised, since a
// typed client masks them.
template <typename F>
void channel_to_mock_pipeline(std::shared_ptr<Resource> mock, F&& test_case) {
    auto server = std::make_shared<sdk::Server>();

    // Normally the high level server service (either robot or module) handles adding managed
    // resources, but in this case we must do it ourselves.
    server->add_resource(std::move(mock));
    server->start();

    auto test_server = TestServer(server);
    std::forward<F>(test_case)(test_server.grpc_in_process_channel());

    // Shutdown Server afterward.
    server->shutdown();
}

// Builds a typed resource client over the in-process channel and hands it to
// the test_case. The common case: exercise a component's client<->server
// behavior end to end.
template <typename ResourceType, typename F>
void client_to_mock_pipeline(std::shared_ptr<Resource> mock, F&& test_case) {
    channel_to_mock_pipeline(mock, [&](std::shared_ptr<grpc::Channel> grpc_channel) {
        auto channel = sdk::ViamChannel(std::move(grpc_channel));
        auto resource_client = sdk::Registry::get()
                                   .lookup_resource_client(API::get<ResourceType>())
                                   ->create_rpc_client(mock->name(), channel);
        std::forward<F>(test_case)(*std::dynamic_pointer_cast<ResourceType>(resource_client));
    });
}

}  // namespace sdktests
}  // namespace viam
