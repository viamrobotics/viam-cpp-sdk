#define BOOST_TEST_MODULE test module test_navigation

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/services/navigation.hpp>
#include <viam/sdk/tests/mocks/mock_navigation.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::Navigation::Mode)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::Navigation::Path)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::Navigation::LocationResponse)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<std::string>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::Navigation::Path>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::geo_point>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::Navigation::MapType)

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

BOOST_AUTO_TEST_CASE(nav_get_location) {
    auto mock = std::make_shared<MockNav>("mock_nav");
    client_to_mock_pipeline<Navigation>(mock, [&](Navigation& client) {
        auto loc = client.get_location();
        BOOST_CHECK_EQUAL(loc, Navigation::LocationResponse{});
        mock->loc.compass_heading = 1;
        loc = client.get_location();
        BOOST_CHECK_CLOSE(loc.compass_heading, 1., 1e-4);
    });
}

// weak backport of std::transform.
template <typename T, typename U>
std::vector<T> mapOver(const std::vector<U>& source, T (*f)(const U&)) {
    std::vector<T> ret;
    for (const auto& x : source) {
        ret.push_back(f(x));
    }
    return ret;
}

BOOST_AUTO_TEST_CASE(nav_waypoints) {
    // get, add, remove
    auto mock = std::make_shared<MockNav>("mock_nav");
    client_to_mock_pipeline<Navigation>(mock, [&](Navigation& client) {
        // confirm empty
        auto waypoints = client.get_waypoints();
        BOOST_CHECK_EQUAL(waypoints->size(), 0);

        // add 3 and confirm size
        for (int i = 0; i < 3; i++) {
            client.add_waypoint(geo_point{0, double(i)});
        }
        waypoints = client.get_waypoints();
        BOOST_CHECK_EQUAL(waypoints->size(), 3);

        // remove 1, check size and IDs
        client.remove_waypoint((*waypoints)[1].id);
        const std::vector<std::string> expected_ids = {(*waypoints)[0].id, (*waypoints)[2].id};
        waypoints = client.get_waypoints();
        BOOST_CHECK_EQUAL(waypoints->size(), 2);
        const auto actual_ids = mapOver(
            *waypoints,
            (std::string(*)(const Navigation::Waypoint&))[](const Navigation::Waypoint& wp) {
                return wp.id;
            });
        BOOST_CHECK_EQUAL(expected_ids, actual_ids);
    });
}

BOOST_AUTO_TEST_CASE(nav_obstacles) {
    auto mock = std::make_shared<MockNav>("mock_nav");
    client_to_mock_pipeline<Navigation>(mock, [&](Navigation& client) {
        // empty case
        auto obstacles = client.get_obstacles();
        BOOST_CHECK_EQUAL(obstacles->size(), 0);

        // one element
        mock->obstacles.push_back({});
        obstacles = client.get_obstacles();
        BOOST_CHECK_EQUAL(obstacles->size(), 1);

        // one element with one sub-element
        mock->obstacles.back().geometries.push_back({});
        obstacles = client.get_obstacles();
        BOOST_CHECK_EQUAL(obstacles->size(), 1);
        BOOST_CHECK_EQUAL(obstacles->back().geometries.size(), 1);
    });
}

BOOST_AUTO_TEST_CASE(nav_paths) {
    auto mock = std::make_shared<MockNav>("mock_nav");
    client_to_mock_pipeline<Navigation>(mock, [&](Navigation& client) {
        // empty
        auto paths = client.get_paths();
        BOOST_CHECK_EQUAL(paths->size(), 0);

        // 1 element with 1 sub-element
        mock->paths.push_back({"2", {{}}});
        paths = client.get_paths();
        std::vector<Navigation::Path> expected = {Navigation::Path{"2", {{}}}};
        BOOST_CHECK_EQUAL(*paths, expected);
    });
}

BOOST_AUTO_TEST_CASE(props) {
    auto mock = std::make_shared<MockNav>("mock_nav");
    client_to_mock_pipeline<Navigation>(mock, [&](Navigation& client) {
        const auto props = client.get_properties();
        BOOST_CHECK_EQUAL(props.map_type, Navigation::MapType::k_unspecified);
    });
}

BOOST_AUTO_TEST_CASE(do_command) {
    auto mock = std::make_shared<MockNav>("mock_nav");
    client_to_mock_pipeline<Navigation>(mock, [&](Navigation& client) {
        const auto ret = client.do_command({});
        BOOST_CHECK_EQUAL(ret.size(), 0);
    });
}

}  // namespace navigation
}  // namespace sdktests
}  // namespace viam
