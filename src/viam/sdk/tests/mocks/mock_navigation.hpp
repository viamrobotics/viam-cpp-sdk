#pragma once

#include <viam/sdk/services/navigation.hpp>

namespace viam {
namespace sdktests {
namespace navigation {

using namespace sdk;

class MockNav : public sdk::Navigation {
   public:
    MockNav(std::string);
    Mode get_mode(const ProtoStruct& extra) override;
    void set_mode(const Mode mode, const ProtoStruct& extra) override;
    LocationResponse get_location(const ProtoStruct& extra) override;
    std::vector<Waypoint> get_waypoints(const ProtoStruct& extra) override;
    void add_waypoint(const geo_point& location, const ProtoStruct& extra) override;
    void remove_waypoint(const std::string id, const ProtoStruct& extra) override;
    std::vector<geo_geometry> get_obstacles(const ProtoStruct& extra) override;
    std::vector<Path> get_paths(const ProtoStruct& extra) override;
    Properties get_properties() override;
    ProtoStruct do_command(const ProtoStruct& command) override;

    Mode mode;
    LocationResponse loc = LocationResponse{};
    std::vector<Waypoint> waypoints;
    int next_waypoint_id = 0;
    std::vector<geo_geometry> obstacles;
    std::vector<Path> paths;
    MapType map_type = MapType::k_unspecified;
};

}  // namespace navigation
}  // namespace sdktests
}  // namespace viam
