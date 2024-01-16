#define BOOST_TEST_MODULE test module test_encoder

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>
#include <viam/api/component/encoder/v1/encoder.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/encoder/client.hpp>
#include <viam/sdk/components/encoder/encoder.hpp>
#include <viam/sdk/components/encoder/server.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/tests/mocks/mock_encoder.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {

using namespace encoder;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_encoder)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockEncoder encoder("mock_encoder");
    auto api = encoder.api();
    auto static_api = API::for_t<Encoder>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "encoder");
}

// This sets up the following architecture
// -- MockComponent
//        /\
//
//        | (function calls)
//
//        \/
// -- ComponentServer (Real)
//        /\
//
//        | (grpc InProcessChannel)
//
//        \/
// -- ComponentClient (Real)
//
// This is as close to a real setup as we can get
// without starting another process
//
// The passed in lambda function has access to the ComponentClient
//
template <typename Lambda>
void server_to_mock_pipeline(Lambda&& func) {
    EncoderServer encoder_server;
    std::shared_ptr<MockEncoder> mock = MockEncoder::get_mock_encoder();
    encoder_server.resource_manager()->add(std::string("mock_encoder"), mock);

    grpc::ServerBuilder builder;
    builder.RegisterService(&encoder_server);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();

    grpc::ChannelArguments args;
    auto grpc_channel = server->InProcessChannel(args);
    EncoderClient client("mock_encoder", grpc_channel);
    // Run the passed test on the created stack
    std::forward<Lambda>(func)(client, mock);
    server->Shutdown();
}

BOOST_AUTO_TEST_CASE(test_get_position) {
    server_to_mock_pipeline([](Encoder& client, std::shared_ptr<MockEncoder> mock) -> void {
        mock->peek_get_position_ret = Encoder::position{1.0, Encoder::position_type::angle_degrees};
        auto returned_position = client.get_position(Encoder::position_type::ticks_count);
        BOOST_CHECK(mock->peek_get_position_position_type == Encoder::position_type::ticks_count);
        BOOST_CHECK(returned_position == mock->peek_get_position_ret);
        // Check that an empty get_position() request has unspecified position_type
        client.get_position();
        BOOST_CHECK(mock->peek_get_position_position_type == Encoder::position_type::unspecified);
    });
}

BOOST_AUTO_TEST_CASE(test_reset_position) {
    server_to_mock_pipeline([](Encoder& client, std::shared_ptr<MockEncoder> mock) -> void {
        mock->peek_reset_position_called = false;
        client.reset_position();
        BOOST_CHECK(mock->peek_reset_position_called);
    });
}

BOOST_AUTO_TEST_CASE(test_get_properties) {
    server_to_mock_pipeline([](Encoder& client, std::shared_ptr<MockEncoder> mock) -> void {
        mock->peek_get_properties_ret = Encoder::properties{false, true};
        auto returned_properties = client.get_properties();
        BOOST_CHECK(returned_properties == mock->peek_get_properties_ret);
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    server_to_mock_pipeline([](Encoder& client, std::shared_ptr<MockEncoder> mock) -> void {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    server_to_mock_pipeline([](Encoder& client, std::shared_ptr<MockEncoder> mock) -> void {
        AttributeMap expected = fake_map();

        AttributeMap command = fake_map();
        AttributeMap result_map = client.do_command(command);

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(result_map->at(std::string("test")));
        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
