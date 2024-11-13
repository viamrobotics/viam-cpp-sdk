/// @file services/navigation.hpp
///
/// @brief Defines a `Navigation` service.
#pragma once

#include <string>

#include <viam/api/service/navigation/v1/navigation.pb.h>

// #include <viam/sdk/common/pose.hpp>
// #include <viam/sdk/common/proto_value.hpp>
// #include <viam/sdk/common/utils.hpp>
// #include <viam/sdk/common/world_state.hpp>
// #include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/services/service.hpp>
// #include <viam/sdk/spatialmath/geometry.hpp>

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
       public:
        GeoPoint location;
        double compass_heading;
    }

    struct Waypoint {
       public:
        std::string id;
        GeoPoint location;
    }

    struct Path {
        std::string destination_waypoint_id;
        std::vector<GeoPoint> geopoints;
    }

    API
    api() const override;

    Mode get_mode(const std::string name, const ProtoStruct& extra);
    void set_mode(const std::string name, const Mode mode, const ProtoStruct& extra);
    LocationResponse get_location(const std::string name, const ProtoStruct& extra);
    std::vector<Waypoint> get_waypoints(const std::string name, const ProtoStruct& extra);
    void add_waypoint(const std::string name, const GeoPoint& location, const ProtoStruct& extra);
    void remove_waypoint(const std::string name, const std::string id, const ProtoStruct& extra);
    std::vector<GeoGeometry> get_obstacles(const std::string name, const ProtoStruct& extra);
    std::vector<Path> void get_paths(const std::string name, const ProtoStruct& extra);
    MapType void get_properties(const std::string);
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

   protected:
    explicit Navigation(std::string name);
};

template <>
struct API::traits<Navigation> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
