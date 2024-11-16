#define BOOST_TEST_MODULE test module test_navigation

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/services/navigation.hpp>
#include <viam/sdk/tests/mocks/mock_navigation.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::Navigation::Mode)

namespace viam {
namespace sdktests {
namespace navigation {

using namespace viam::sdk;

BOOST_AUTO_TEST_CASE(nav_mode) {
    auto mock = std::make_shared<MockNav>("mock_nav");
    client_to_mock_pipeline<Navigation>(mock, [&](Navigation& client) {
        client.set_mode(Navigation::Mode::k_manual);
        BOOST_CHECK_EQUAL(client.get_mode(), Navigation::Mode::k_manual);
        client.set_mode(Navigation::Mode::k_explore);
        BOOST_CHECK_EQUAL(client.get_mode(), Navigation::Mode::k_explore);
    });
}

BOOST_AUTO_TEST_CASE(nav_get_location) {}

BOOST_AUTO_TEST_CASE(nav_waypoints) {
    // get, add, remove
}

BOOST_AUTO_TEST_CASE(nav_obstacles) {}

BOOST_AUTO_TEST_CASE(nav_paths) {}

}  // namespace navigation
}  // namespace sdktests
}  // namespace viam
