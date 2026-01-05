#define BOOST_TEST_MODULE test module test_gantry

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/components/gantry.hpp>
#include <viam/sdk/tests/mocks/mock_gantry.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<double>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::KinematicsData)

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

BOOST_AUTO_TEST_CASE(test_get_kinematics) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [](Gantry& client) {
        const auto& kinematics = client.get_kinematics();
        BOOST_CHECK_EQUAL(kinematics, fake_kinematics());
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockGantry> mock = MockGantry::get_mock_gantry();
    client_to_mock_pipeline<Gantry>(mock, [](Gantry& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
