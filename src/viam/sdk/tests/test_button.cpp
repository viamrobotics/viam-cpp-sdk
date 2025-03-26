#define BOOST_TEST_MODULE test module test_button

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/components/button.hpp>
#include <viam/sdk/tests/mocks/mock_button.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace button;
using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_button)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockButton button("mock_button");
    auto api = button.api();
    auto static_api = API::get<Button>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "button");
}

BOOST_AUTO_TEST_CASE(test_push) {
    std::shared_ptr<MockButton> mock = MockButton::get_mock_button();
    client_to_mock_pipeline<Button>(mock, [&](Button& client) {
        client.push();
        BOOST_CHECK(mock->peek_push_called);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockButton> mock = MockButton::get_mock_button();
    client_to_mock_pipeline<Button>(mock, [](Button& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
