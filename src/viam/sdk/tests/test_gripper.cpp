#define BOOST_TEST_MODULE test module test_gripper

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/components/gripper.hpp>
#include <viam/sdk/tests/mocks/mock_gripper.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

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
        AttributeMap expected = fake_map();

        AttributeMap command = fake_map();
        AttributeMap result_map = client.do_command(command);

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(result_map->at(std::string("test")));
        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockGripper> mock = MockGripper::get_mock_gripper();
    client_to_mock_pipeline<Gripper>(mock, [](Gripper& client) {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_SUITE_END()
}  // namespace sdktests
}  // namespace viam
