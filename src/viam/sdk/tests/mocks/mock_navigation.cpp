#include <viam/sdk/tests/mocks/mock_navigation.hpp>

#include <chrono>

#include <viam/sdk/common/private/utils.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/motion.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace navigation {

using namespace viam::sdk;

Navigation::Mode MockNav::get_mode(const ProtoStruct& extra) {
    return mode;
}

void MockNav::set_mode(const Mode mode, const ProtoStruct& extra) {
    this->mode = mode;
}

Navigation::LocationResponse MockNav::get_location(const ProtoStruct& extra) {
    return loc;
}

std::unique_ptr<std::vector<Navigation::Waypoint>> MockNav::get_waypoints(
    const ProtoStruct& extra) {
    return std::make_unique<std::vector<Waypoint>>(waypoints);
}

void MockNav::add_waypoint(const geo_point& location, const ProtoStruct& extra) {
    waypoints.push_back(Waypoint{std::to_string(next_waypoint_id++), location});
}

void MockNav::remove_waypoint(const std::string id, const ProtoStruct& extra) {
    for (auto it = waypoints.begin(); it != waypoints.end(); ++it) {
        if (it->id == id) {
            waypoints.erase(it);
            break;
        }
    }
}

std::unique_ptr<std::vector<geo_geometry>> MockNav::get_obstacles(const ProtoStruct& extra) {
    return std::make_unique<std::vector<geo_geometry>>(obstacles);
}

std::unique_ptr<std::vector<Navigation::Path>> MockNav::get_paths(const ProtoStruct& extra) {
    return std::make_unique<std::vector<Path>>(paths);
}

Navigation::Properties MockNav::get_properties() {
    return Properties{map_type};
}

ProtoStruct MockNav::do_command(const ProtoStruct& command) {
    return ProtoStruct{};
}

}  // namespace navigation
}  // namespace sdktests
}  // namespace viam
