#define BOOST_TEST_MODULE test module test_board

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/board/v1/board.grpc.pb.h>
#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/board/board.hpp>
#include <viam/sdk/components/board/client.hpp>
#include <viam/sdk/components/board/server.hpp>
#include <viam/sdk/tests/mocks/mock_board.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace board;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_board)
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
    BoardServer board_server;
    std::shared_ptr<MockBoard> mock = std::make_shared<MockBoard>("mock_board");
    board_server.resource_manager()->add(std::string("mock_board"), mock);

    grpc::ServerBuilder builder;
    builder.RegisterService(&board_server);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();

    grpc::ChannelArguments args;
    auto grpc_channel = server->InProcessChannel(args);
    BoardClient client("mock_board", grpc_channel);
    // Run the passed test on the created stack
    std::forward<Lambda>(func)(client, mock);
    server->Shutdown();
}

BOOST_AUTO_TEST_CASE(test_status) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_CASE(test_set_gpio) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_CASE(test_get_gpio) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_CASE(test_get_pwm) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_CASE(test_set_pwm) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_CASE(test_get_pwm_frequency) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_CASE(test_set_pwm_frequency) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_CASE(test_read_analog) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_CASE(test_read_digital_interrupt) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_CASE(test_set_power_mode) {
    server_to_mock_pipeline([](Board& client, std::shared_ptr<MockBoard> mock) -> void {
        // TODO impl
        BOOST_CHECK(false);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
