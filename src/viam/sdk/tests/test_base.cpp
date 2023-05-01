#define BOOST_TEST_MODULE test module test_base
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/base/v1/base.grpc.pb.h>
#include <viam/api/component/base/v1/base.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/base/base.hpp>
#include <viam/sdk/components/base/client.hpp>
#include <viam/sdk/components/base/server.hpp>
#include <viam/sdk/tests/mocks/mock_base.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace base;

using namespace viam::sdk;

using boost::qvm::operator-;
using boost::qvm::mag;

BOOST_AUTO_TEST_SUITE(test_base)

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
    BaseServer base_server;
    std::shared_ptr<MockBase> mock = MockBase::get_mock_base();
    base_server.resource_manager()->add(std::string("mock_base"), mock);

    grpc::ServerBuilder builder;
    builder.RegisterService(&base_server);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();

    grpc::ChannelArguments args;
    auto grpc_channel = server->InProcessChannel(args);
    BaseClient client("mock_base", grpc_channel);
    // Run the passed test on the created stack
    std::forward<Lambda>(func)(client, mock);
    // shutdown afterwards
    server->Shutdown();
}

BOOST_AUTO_TEST_CASE(test_move_straight) {
    server_to_mock_pipeline([](Base& client, std::shared_ptr<MockBase> mock) -> void {
        client.move_straight(32, 0.75);
        BOOST_CHECK_EQUAL(mock->peek_move_straight_distance_mm, 32);
        BOOST_CHECK_EQUAL(mock->peek_move_straight_mm_per_sec, 0.75);
    });
}

BOOST_AUTO_TEST_CASE(test_spin) {
    server_to_mock_pipeline([](Base& client, std::shared_ptr<MockBase> mock) -> void {
        client.spin(57.1, -21.1);
        BOOST_CHECK_EQUAL(mock->peek_spin_angle_deg, 57.1);
        BOOST_CHECK_EQUAL(mock->peek_spin_degs_per_sec, -21.1);
    });
}

BOOST_AUTO_TEST_CASE(test_set_power) {
    server_to_mock_pipeline([](Base& client, std::shared_ptr<MockBase> mock) -> void {
        Vector3 linear = {0.1, -0.1, 1.0};
        Vector3 angular = {0.5, -1.0, 1.0};
        client.set_power(linear, angular);

        BOOST_CHECK_SMALL(mag(mock->peek_set_power_linear - linear), 0.01);
        BOOST_CHECK_SMALL(mag(mock->peek_set_power_angular - angular), 0.01);
    });
}

BOOST_AUTO_TEST_CASE(test_set_velocity) {
    server_to_mock_pipeline([](Base& client, std::shared_ptr<MockBase> mock) -> void {
        Vector3 linear = {0.1, -0.1, 1.0};
        Vector3 angular = {0.5, -1.0, 1.0};
        client.set_velocity(linear, angular);

        BOOST_CHECK_SMALL(mag(mock->peek_set_velocity_linear - linear), 0.01);
        BOOST_CHECK_SMALL(mag(mock->peek_set_velocity_angular - angular), 0.01);
    });
}

BOOST_AUTO_TEST_CASE(test_stop) {
    server_to_mock_pipeline([](Base& client, std::shared_ptr<MockBase> mock) -> void {
        mock->peek_stop_called = false;
        client.stop();
        BOOST_CHECK(mock->peek_stop_called);
    });
}

BOOST_AUTO_TEST_CASE(test_is_moving) {
    server_to_mock_pipeline([](Base& client, std::shared_ptr<MockBase> mock) -> void {
        BOOST_CHECK(!client.is_moving());
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    server_to_mock_pipeline([](Base& client, std::shared_ptr<MockBase> mock) -> void {
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
