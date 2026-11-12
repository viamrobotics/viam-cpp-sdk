#define BOOST_TEST_MODULE test module test_gantry

#include <boost/test/included/unit_test.hpp>

#include <grpcpp/client_context.h>
#include <viam/api/component/gantry/v1/gantry.grpc.pb.h>

#include <viam/sdk/components/gantry.hpp>
#include <viam/sdk/tests/mocks/mock_gantry.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<double>)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::KinematicsData)
#pragma GCC diagnostic pop

namespace viam {
namespace sdktests {
using namespace viam::sdk;
using namespace gantry;

BOOST_AUTO_TEST_SUITE(test_gantry)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockGantry gantry("mock_gantry");
    auto api = gantry.api();
    auto static_api = API::get<Gantry>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "gantry");
}

BOOST_AUTO_TEST_CASE(test_positions) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [&](Gantry& client) {
        client.move_to_position({{1.0, 1.0}, {2.0, 2.0}, {3.0, 3.0}, {4.0, 4.0}});
        BOOST_CHECK_EQUAL(client.get_position(), (std::vector<double>({1.0, 2.0, 3.0, 4.0})));
    });
}

// The typed client always pairs each position with a speed, so drive the server with a raw stub
// to send what the RDK Go client sends from GoToInputs: positions and no speeds.
BOOST_AUTO_TEST_CASE(test_move_to_position_without_speeds) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    channel_to_mock_pipeline(mock, [&](std::shared_ptr<grpc::Channel> channel) {
        auto stub = ::viam::component::gantry::v1::GantryService::NewStub(channel);
        ::viam::component::gantry::v1::MoveToPositionRequest request;
        request.set_name(mock->name());
        request.add_positions_mm(10.0);
        request.add_positions_mm(20.0);
        ::viam::component::gantry::v1::MoveToPositionResponse response;
        grpc::ClientContext ctx;

        const grpc::Status status = stub->MoveToPosition(&ctx, request, &response);

        BOOST_CHECK(status.ok());
        BOOST_CHECK_EQUAL(mock->peek_positions, (std::vector<double>({10.0, 20.0})));
        BOOST_CHECK_EQUAL(mock->peek_speeds, (std::vector<double>({0.0, 0.0})));
    });
}

BOOST_AUTO_TEST_CASE(test_move_to_position_mismatched_speeds) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    channel_to_mock_pipeline(mock, [&](std::shared_ptr<grpc::Channel> channel) {
        auto stub = ::viam::component::gantry::v1::GantryService::NewStub(channel);
        ::viam::component::gantry::v1::MoveToPositionRequest request;
        request.set_name(mock->name());
        request.add_positions_mm(10.0);
        request.add_positions_mm(20.0);
        request.add_speeds_mm_per_sec(5.0);
        ::viam::component::gantry::v1::MoveToPositionResponse response;
        grpc::ClientContext ctx;

        const grpc::Status status = stub->MoveToPosition(&ctx, request, &response);

        BOOST_CHECK_EQUAL(status.error_code(), grpc::StatusCode::INVALID_ARGUMENT);
        BOOST_CHECK(mock->peek_positions.empty());
    });
}

BOOST_AUTO_TEST_CASE(test_home) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [&](Gantry& client) {
        BOOST_CHECK(client.home());
        BOOST_CHECK(mock->peek_home_called);
    });
}

BOOST_AUTO_TEST_CASE(test_get_lengths) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [](Gantry& client) {
        const auto& geometries = client.get_lengths();
        BOOST_CHECK_EQUAL(geometries, fake_lengths());
    });
}

BOOST_AUTO_TEST_CASE(test_is_moving) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [](Gantry& client) { BOOST_CHECK(!client.is_moving()); });
}

BOOST_AUTO_TEST_CASE(test_stop) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [&](Gantry& client) {
        mock->peek_stop_called = false;
        client.stop();
        BOOST_CHECK(mock->peek_stop_called);
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [](Gantry& client) {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_CASE(test_get_kinematic_model) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [&](Gantry& client) {
        // the server names the model for the resource, whatever name it came with
        KinematicModel expected = fake_kinematic_model();
        expected.name = mock->name();
        BOOST_CHECK(client.get_kinematic_model() == expected);
        BOOST_CHECK(!mock->peek_kinematics_options.include_collision_meshes);

        kinematics_options options;
        options.include_collision_meshes = true;
        BOOST_CHECK(client.get_kinematic_model(options) == expected);
        BOOST_CHECK(mock->peek_kinematics_options.include_collision_meshes);
        BOOST_CHECK(!mock->peek_kinematics_options.include_visual_meshes);
    });
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
BOOST_AUTO_TEST_CASE(test_get_kinematics) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [](Gantry& client) {
        const auto& kinematics = client.get_kinematics();
        BOOST_CHECK_EQUAL(kinematics, fake_kinematics());
    });
}
#pragma GCC diagnostic pop

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [](Gantry& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_CASE(test_get_status) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [](Gantry& client) {
        const ProtoStruct status = client.get_status();
        const ProtoStruct expected = fake_status();
        BOOST_CHECK(status.at("is_moving") == expected.at("is_moving"));
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
