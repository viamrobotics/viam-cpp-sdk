#pragma once

#include <viam/sdk/spatialmath/orientation.hpp>

#include <array>
#include <string>
#include <tuple>

#include <viam/api/common/v1/common.pb.h>

namespace viam {
namespace sdk {

enum GeometryType {
    box,
    sphere,
    capsule,
    point,
    unknown,
};

struct coordinates {
    double x, y, z;
};

struct pose_orientation {
    double o_x, o_y, o_z;
};

struct pose {
    struct coordinates coordinates;
    pose_orientation orientation;
    double theta;

    static pose from_proto(const viam::common::v1::Pose& proto);
    viam::common::v1::Pose to_proto() const;
};

// CR erodkin: this really needs good documentation.
class GeometryConfig {
   public:
    struct box {
        double x;
        double y;
        double z;
    };
    struct sphere {
        double radius;
    };
    struct capsule {
        double radius;
        double length;
    };
    typedef boost::variant<struct box, struct sphere, struct capsule, boost::blank>
        geometry_specifics;
    viam::common::v1::Geometry to_proto() const;
    viam::common::v1::RectangularPrism box_proto() const;
    viam::common::v1::Sphere sphere_proto() const;
    viam::common::v1::Capsule capsule_proto() const;
    viam::common::v1::Pose pose_proto() const;
    static GeometryConfig from_proto(const viam::common::v1::Geometry& proto);
    static std::vector<GeometryConfig> from_proto(
        const viam::common::v1::GetGeometriesResponse& proto);
    void set_coordinates(coordinates coordinates);
    void set_pose(pose pose);
    void set_pose_orientation(pose_orientation orientation);
    void set_theta(double theta);
    void set_geometry_specifics(geometry_specifics gs);
    void set_geometry_type(GeometryType type);
    void set_orientation_config(OrientationConfig config);
    void set_label(std::string label);
    double get_theta() const;
    coordinates get_coordinates() const;
    pose get_pose() const;
    geometry_specifics get_geometry_specifics() const;
    GeometryType get_geometry_type() const;
    OrientationConfig get_orientation_config() const;
    std::string get_label() const;
    friend bool operator==(const GeometryConfig& lhs, const GeometryConfig& rhs);

   private:
    GeometryType geometry_type_;
    pose pose_;
    geometry_specifics geometry_specifics_;
    // CR erodkin: flyby adding theta
    // TODO: if and when RDK makes more explicit use of ox/oy/oz, we should
    // do the same here
    OrientationConfig orientation_config_;
    std::string label_;
};

// CR erodkin: do we need this at all? same with geo_obstacle.
struct geo_point {
    double longitude, latitude;

    common::v1::GeoPoint to_proto() const;
    static geo_point from_proto(const common::v1::GeoPoint& proto);
};

struct geo_obstacle {
    geo_point location;
    std::vector<GeometryConfig> geometries;

    common::v1::GeoObstacle to_proto() const;
    static geo_obstacle from_proto(const common::v1::GeoObstacle& proto);
};

}  // namespace sdk
}  // namespace viam
