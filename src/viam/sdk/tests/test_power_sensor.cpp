#define BOOST_TEST_MODULE test module test_powersensor
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.grpc.pb.h>
#include <viam/api/component/powersensor/v1/powersensor.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/power_sensor/client.hpp>
#include <viam/sdk/components/power_sensor/power_sensor.hpp>
#include <viam/sdk/components/power_sensor/server.hpp>
#include <viam/sdk/tests/mocks/mock_power_sensor.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {

using namespace powersensor;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_powersensor)

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
    PowerSensorServer powersensor_server;
    std::shared_ptr<MockPowerSensor> mock = MockPowerSensor::get_mock_powersensor();
    powersensor_server.resource_manager()->add(std::string("mock_powersensor"), mock);

    grpc::ServerBuilder builder;
    builder.RegisterService(&powersensor_server);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();

    grpc::ChannelArguments args;
    auto grpc_channel = server->InProcessChannel(args);
    PowerSensorClient client("mock_powersensor", grpc_channel);
    // Run the passed test on the created stack
    std::forward<Lambda>(func)(client, mock);
    // shutdown afterwards
    server->Shutdown();
}

BOOST_AUTO_TEST_CASE(test_get_voltage) {
    server_to_mock_pipeline([](PowerSensor& client, std::shared_ptr<MockPowerSensor> mock) -> void {
        mock->peek_voltage = {10.0, false};
        BOOST_CHECK(client.get_voltage() == mock->peek_voltage);
    });
}

BOOST_AUTO_TEST_CASE(test_get_current) {
    server_to_mock_pipeline([](PowerSensor& client, std::shared_ptr<MockPowerSensor> mock) -> void {
        mock->peek_current = {10.0, false};
        BOOST_CHECK(client.get_current() == mock->peek_current);
    });
}

BOOST_AUTO_TEST_CASE(test_get_power) {
    server_to_mock_pipeline([](PowerSensor& client, std::shared_ptr<MockPowerSensor> mock) -> void {
        mock->peek_power = 10.0;
        BOOST_CHECK(client.get_power() == mock->peek_power);
    });
}

BOOST_AUTO_TEST_CASE(test_get_readings) {
    server_to_mock_pipeline([](PowerSensor& client, std::shared_ptr<MockPowerSensor> mock) -> void {
        AttributeMap expected = fake_map();

        AttributeMap readings = client.get_readings();

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(readings->at(std::string("test")));
        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    server_to_mock_pipeline([](PowerSensor& client, std::shared_ptr<MockPowerSensor> mock) -> void {
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
