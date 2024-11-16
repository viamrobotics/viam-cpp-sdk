#define BOOST_TEST_MODULE test module test_navigation

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/services/navigation.hpp>
#include <viam/sdk/tests/mocks/mock_navigation.hpp>

namespace viam {
namespace sdktests {
namespace navigation {

using namespace viam::sdk;

BOOST_AUTO_TEST_CASE(nav_mode) {
    // test get_mode and set_mode
}

BOOST_AUTO_TEST_CASE(nav_get_location) {
}

BOOST_AUTO_TEST_CASE(nav_waypoints) {
    // get, add, remove
}

BOOST_AUTO_TEST_CASE(nav_obstacles) {
}

BOOST_AUTO_TEST_CASE(nav_paths) {
}

}  // namespace navigation
}  // namespace sdktests
}  // namespace viam
