/// @file services/navigation.hpp
///
/// @brief Defines a `Navigation` service.
#pragma once

#include <string>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/service.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

class Navigation : public Service {
   public:
    enum class Mode : uint8_t {
        k_unspecified,
        k_manual,
        k_waypoint,
        k_explore,
    };

    enum class MapType : uint8_t {
        k_unspecified,
        k_none,
        k_gps,
    };

    struct LocationResponse {
        geo_point location;
        double compass_heading;
    };

    struct Properties {
        MapType map_type;
    };

    struct Waypoint {
        std::string id;
        geo_point location;
    };

    struct Path {
        std::string destination_waypoint_id;
        std::vector<geo_point> geopoints;
    };

    API api() const override;

    virtual Mode get_mode(const ProtoStruct& extra) = 0;
    virtual void set_mode(const Mode mode, const ProtoStruct& extra) = 0;
    virtual LocationResponse get_location(const ProtoStruct& extra) = 0;
    virtual std::unique_ptr<std::vector<Waypoint>> get_waypoints(const ProtoStruct& extra) = 0;
    virtual void add_waypoint(const geo_point& location,
                              const ProtoStruct& extra) = 0;
    virtual void remove_waypoint(const std::string id,
                                 const ProtoStruct& extra) = 0;
    virtual std::unique_ptr<std::vector<geo_geometry>> get_obstacles(const ProtoStruct& extra) = 0;
    virtual std::unique_ptr<std::vector<Path>> get_paths(const ProtoStruct& extra) = 0;
    virtual Properties get_properties() = 0;
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    // overloads without `extra` param.
    inline Mode get_mode() {
        return get_mode({});
    }
    inline void set_mode(const Mode mode) {
        set_mode(mode, {});
    }
    inline LocationResponse get_location() {
        return get_location({});
    }
    inline std::unique_ptr<std::vector<Waypoint>> get_waypoints() {
        return get_waypoints({});
    }
    inline void add_waypoint(const geo_point& location) {
        add_waypoint(location, {});
    }
    inline void remove_waypoint(const std::string id) {
        remove_waypoint(id, {});
    }
    inline std::unique_ptr<std::vector<geo_geometry>> get_obstacles() {
        return get_obstacles({});
    }
    inline std::unique_ptr<std::vector<Path>> get_paths() {
        return get_paths({});
    }

   protected:
    explicit Navigation(std::string name);
};

template <>
struct API::traits<Navigation> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
