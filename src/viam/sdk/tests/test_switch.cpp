#define BOOST_TEST_MODULE test module test_switch
#include <viam/sdk/components/switch.hpp>

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/tests/mocks/mock_switch.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::ProtoStruct)

namespace viam {
namespace sdktests {

using namespace switch_;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_switch)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockSwitch switch_("mock_switch");
    auto api = switch_.api();
    auto static_api = API::get<Switch>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "switch");
}

BOOST_AUTO_TEST_CASE(test_positions) {
    std::shared_ptr<MockSwitch> mock = MockSwitch::get_mock_switch();
    client_to_mock_pipeline<Switch>(mock, [&](Switch& client) {
        uint32_t position = 3;
        client.set_position(position);
        BOOST_CHECK_EQUAL(mock->current_position, position);
        BOOST_CHECK_EQUAL(client.get_position(), position);
    });
}

BOOST_AUTO_TEST_CASE(test_number_of_positions) {
    std::shared_ptr<MockSwitch> mock = MockSwitch::get_mock_switch();
    client_to_mock_pipeline<Switch>(mock, [&](Switch& client) {
        mock->number_of_positions = 5;
        BOOST_CHECK_EQUAL(client.get_number_of_positions(), 5);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockSwitch> mock = MockSwitch::get_mock_switch();
    client_to_mock_pipeline<Switch>(mock, [](Switch& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
