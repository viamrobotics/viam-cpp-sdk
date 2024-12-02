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
    /// @enum Mode
    /// @brief Enum affecting how this nav service's goal is specified.
    /// @ingroup Navigation
    enum class Mode : uint8_t {
        k_unspecified,
        k_manual,
        k_waypoint,
        k_explore,
    };

    /// @enum MapType
    /// @brief Enum affecting the format of the map used with this nav service.
    /// @ingroup Navigation
    enum class MapType : uint8_t {
        k_unspecified,
        k_none,
        k_gps,
    };

    /// @struct LocationResponse
    /// @brief Location and direction of the base configured
    /// @ingroup Navigation
    struct LocationResponse {
        geo_point location;
        double compass_heading;

        bool operator==(const LocationResponse& rhs) const {
            return compass_heading == rhs.compass_heading && location == rhs.location;
        }
    };

    /// @struct Properties
    /// @brief A set of attributes for this nav service.
    /// @ingroup Navigation
    struct Properties {
        MapType map_type;
    };

    /// @struct Waypoint
    /// @brief A location with an `id` handle that can be used to uniquely identify and remove it.
    /// @ingroup Navigation
    struct Waypoint {
        std::string id;
        geo_point location;
    };

    /// @struct Path
    /// @brief A user-provided destination and a set of geopoints to get there.
    /// @ingroup Navigation
    struct Path {
        std::string destination_waypoint_id;
        std::vector<geo_point> geopoints;

        bool operator==(const Path& rhs) const {
            return destination_waypoint_id == rhs.destination_waypoint_id &&
                   geopoints == rhs.geopoints;
        }
    };

    API api() const override;

    /// @brief Get the current mode.
    /// @param extra Any additional arguments to the method.
    /// @return Current mode.
    virtual Mode get_mode(const ProtoStruct& extra) = 0;

    /// @brief Set the current mode.
    /// @param mode Desired mode.
    /// @param extra Any additional arguments to the method.
    virtual void set_mode(const Mode mode, const ProtoStruct& extra) = 0;

    /// @brief Get the current location.
    /// @param extra Any additional arguments to the method.
    /// @return Current location.
    virtual LocationResponse get_location(const ProtoStruct& extra) = 0;

    /// @brief Get the waypoints this nav service knows about.
    /// @param extra Any additional arguments to the method.
    /// @return List of waypoints.
    virtual std::vector<Waypoint> get_waypoints(const ProtoStruct& extra) = 0;

    /// @brief Add a waypoint.
    /// @param location Coordinate of the new waypoint.
    virtual void add_waypoint(const geo_point& location, const ProtoStruct& extra) = 0;

    /// @brief Remove a waypoint by ID.
    /// @param id The string ID of the waypoint to remove.
    /// @param extra Any additional arguments to the method.
    virtual void remove_waypoint(const std::string id, const ProtoStruct& extra) = 0;

    /// @brief Get the obstacles this nav service knows about.
    /// @param extra Any additional arguments to the method.
    /// @return List of shapes.
    virtual std::vector<geo_geometry> get_obstacles(const ProtoStruct& extra) = 0;

    /// @brief Get the paths this nav service knows about.
    /// @param extra Any additional arguments to the method.
    /// @return List of paths.
    virtual std::vector<Path> get_paths(const ProtoStruct& extra) = 0;

    /// @brief Get this nav service's properties.
    /// @return Properties.
    virtual Properties get_properties() = 0;

    /// @brief Do an arbitrary command.
    /// @param command Freeform fields that are service-specific.
    /// @return Freeform result of the command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    // overloads without `extra` param:

    inline Mode get_mode() {
        return get_mode({});
    }

    inline void set_mode(const Mode mode) {
        set_mode(mode, {});
    }

    inline LocationResponse get_location() {
        return get_location({});
    }

    inline std::vector<Waypoint> get_waypoints() {
        return get_waypoints({});
    }

    inline void add_waypoint(const geo_point& location) {
        add_waypoint(location, {});
    }

    inline void remove_waypoint(const std::string id) {
        remove_waypoint(id, {});
    }

    inline std::vector<geo_geometry> get_obstacles() {
        return get_obstacles({});
    }

    inline std::vector<Path> get_paths() {
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
