#define BOOST_TEST_MODULE test module test_arm
#include <viam/sdk/components/arm.hpp>

#include <boost/optional/optional_io.hpp>
#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/tests/mocks/mock_arm.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<double>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<std::vector<double>>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::KinematicsData)

namespace viam {
namespace sdktests {

using namespace arm;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_arm)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockArm arm("mock_arm");
    auto api = arm.api();
    auto static_api = API::get<Arm>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "arm");
}

BOOST_AUTO_TEST_CASE(test_positions) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        pose p{{2, 3, 4}, {5, 6, 7}, 8};
        client.move_to_position(p);
        BOOST_CHECK_EQUAL(mock->current_location, p);
        BOOST_CHECK_EQUAL(mock->get_end_position({}), p);
    });
}

BOOST_AUTO_TEST_CASE(joint_positions) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        std::vector<double> positions{{1.0, 2.0, 3.0, 4.0}};
        client.move_to_joint_positions(positions, {});
        BOOST_CHECK_EQUAL(client.get_joint_positions(), positions);
        BOOST_CHECK_EQUAL(mock->joint_positions, positions);
    });
}

BOOST_AUTO_TEST_CASE(thru_joint_positions) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        std::vector<std::vector<double>> positions{{1.0, 2.0}, {3.0}};
        client.move_through_joint_positions(positions, {1.0, 2.0}, {});
        BOOST_CHECK_EQUAL(mock->move_thru_positions, positions);
        BOOST_CHECK_EQUAL(mock->move_opts.max_vel_degs_per_sec, 1.0);
        BOOST_CHECK_EQUAL(mock->move_opts.max_acc_degs_per_sec2, 2.0);
        BOOST_CHECK_GT(mock->viam_client_metadata.size(), 0);
    });
}

BOOST_AUTO_TEST_CASE(test_stop) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        mock->peek_stop_called = false;
        client.stop();
        BOOST_CHECK(mock->peek_stop_called);
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_CASE(test_get_kinematics) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) {
        const auto& kinematics = client.get_kinematics();
        BOOST_CHECK_EQUAL(kinematics, fake_kinematics());
    });
}

BOOST_AUTO_TEST_CASE(test_is_moving) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) { BOOST_CHECK(!client.is_moving()); });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_CASE(test_get_3d_models) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) {
        const auto& models = client.get_3d_models();
        const auto& expected = fake_3d_models();

        // Compare keysets directly
        std::set<std::string> model_keys;
        std::set<std::string> expected_keys;
        for (const auto& [key, _] : models) {
            model_keys.insert(key);
        }
        for (const auto& [key, _] : expected) {
            expected_keys.insert(key);
        }
        BOOST_CHECK_EQUAL_COLLECTIONS(
            model_keys.begin(), model_keys.end(), expected_keys.begin(), expected_keys.end());
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
