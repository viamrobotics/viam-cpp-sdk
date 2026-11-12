#define BOOST_TEST_MODULE test module test_gripper

#include <boost/test/included/unit_test.hpp>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/components/gripper.hpp>
#include <viam/sdk/tests/mocks/mock_gripper.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::ProtoValue)

namespace viam {
namespace sdktests {

using namespace gripper;
using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_gripper)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockGripper gripper("mock_gripper");
    auto api = gripper.api();
    auto static_api = API::get<Gripper>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "gripper");
}

BOOST_AUTO_TEST_CASE(test_open) {
    std::shared_ptr<MockGripper> mock = MockGripper::get_mock_gripper();
    client_to_mock_pipeline<Gripper>(mock, [&](Gripper& client) {
        client.open();
        BOOST_CHECK(mock->peek_open_called);
    });
}

BOOST_AUTO_TEST_CASE(test_grab) {
    std::shared_ptr<MockGripper> mock = MockGripper::get_mock_gripper();
    client_to_mock_pipeline<Gripper>(mock, [](Gripper& client) { BOOST_CHECK(client.grab()); });
}

BOOST_AUTO_TEST_CASE(test_is_holding_something) {
    std::shared_ptr<MockGripper> mock = MockGripper::get_mock_gripper();
    client_to_mock_pipeline<Gripper>(mock, [](Gripper& client) {
        const ProtoStruct extra = {
            {"BEEP", "BOOP"},
        };
        Gripper::holding_status holding_status = client.is_holding_something(extra);
        BOOST_CHECK_EQUAL(holding_status.is_holding_something, false);
        BOOST_CHECK_EQUAL(holding_status.meta["BEEP"], ProtoValue("BOOP"));
    });
}

BOOST_AUTO_TEST_CASE(test_stop) {
    std::shared_ptr<MockGripper> mock = MockGripper::get_mock_gripper();
    client_to_mock_pipeline<Gripper>(mock, [&](Gripper& client) {
        mock->peek_stop_called = false;
        client.stop();
        BOOST_CHECK(mock->peek_stop_called);
    });
}

BOOST_AUTO_TEST_CASE(test_is_moving) {
    std::shared_ptr<MockGripper> mock = MockGripper::get_mock_gripper();
    client_to_mock_pipeline<Gripper>(mock,
                                     [](Gripper& client) { BOOST_CHECK(!client.is_moving()); });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockGripper> mock = MockGripper::get_mock_gripper();
    client_to_mock_pipeline<Gripper>(mock, [](Gripper& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockGripper> mock = MockGripper::get_mock_gripper();
    client_to_mock_pipeline<Gripper>(mock, [](Gripper& client) {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_CASE(test_get_kinematic_model) {
    std::shared_ptr<MockGripper> mock = MockGripper::get_mock_gripper();
    client_to_mock_pipeline<Gripper>(mock, [&](Gripper& client) {
        // the server names the model for the resource, whatever name it came with
        KinematicModel expected = fake_kinematic_model();
        expected.name = mock->name();
        BOOST_CHECK(client.get_kinematic_model() == expected);
        BOOST_CHECK(!mock->peek_kinematics_options.include_collision_meshes);
        BOOST_CHECK(!mock->peek_kinematics_options.include_visual_meshes);

        kinematics_options options;
        options.include_visual_meshes = true;
        BOOST_CHECK(client.get_kinematic_model(options) == expected);
        BOOST_CHECK(!mock->peek_kinematics_options.include_collision_meshes);
        BOOST_CHECK(mock->peek_kinematics_options.include_visual_meshes);
    });
}

// a gripper that does not override get_kinematic_model has none to serve, and viam-server only
// falls back to the gripper's geometries when the rpc comes back unimplemented.
BOOST_AUTO_TEST_CASE(test_get_kinematic_model_not_provided) {
    struct NoKinematicsGripper : MockGripper {
        using MockGripper::MockGripper;
        KinematicModel get_kinematic_model(const kinematics_options& options,
                                           const ProtoStruct& extra) override {
            return Gripper::get_kinematic_model(options, extra);
        }
    };
    auto mock = std::make_shared<NoKinematicsGripper>("mock_gripper");
    client_to_mock_pipeline<Gripper>(mock, [](Gripper& client) {
        try {
            client.get_kinematic_model();
            BOOST_FAIL("expected get_kinematic_model to throw");
        } catch (const GRPCException& xcp) {
            BOOST_CHECK_EQUAL(xcp.status()->error_code(), ::grpc::StatusCode::UNIMPLEMENTED);
        }
    });
}

BOOST_AUTO_TEST_CASE(test_get_status) {
    std::shared_ptr<MockGripper> mock = MockGripper::get_mock_gripper();
    client_to_mock_pipeline<Gripper>(mock, [](Gripper& client) {
        const ProtoStruct status = client.get_status();
        const ProtoStruct expected = fake_status();
        BOOST_CHECK(status.at("is_moving") == expected.at("is_moving"));
    });
}

BOOST_AUTO_TEST_SUITE_END()
}  // namespace sdktests
}  // namespace viam
