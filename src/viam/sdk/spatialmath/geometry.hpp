#pragma once

#include <string>
#include <tuple>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace sdk {

struct Coordinates {
    double x_;
    double y_;
    double z_;
};
struct Pose {
    double o_x_;
    double o_y_;
    double o_z_;
};
enum GeometryType {
    box,
    sphere,
    point,
    unknown,
};

class GeometryConfig {
   public:
    viam::common::v1::Geometry to_proto() const;
    viam::common::v1::RectangularPrism box_proto() const;
    viam::common::v1::Sphere sphere_proto() const;
    viam::common::v1::Pose pose_proto() const;
    static GeometryConfig from_proto(const viam::common::v1::Geometry& proto);
    static std::vector<GeometryConfig> from_proto(
        const viam::common::v1::GetGeometriesResponse& proto);
    void set_coordinates(const Coordinates& coordinates);
    void set_pose(const Pose& pose);
    void set_radius(double r);
    void set_geometry_type(GeometryType type);
    void set_orientation_config(OrientationConfig config);
    void set_label(std::string label);
    double get_radius() const;
    Coordinates get_coordinates() const;
    Pose get_pose() const;
    GeometryType get_geometry_type() const;
    OrientationConfig get_orientation_config() const;
    std::string get_label() const;
    friend bool operator==(const GeometryConfig& lhs, const GeometryConfig& rhs);

   private:
    Coordinates coordinates_;
    GeometryType geometry_type_;
    Pose pose_;
    double r_;
    // TODO: if and when RDK makes more explicit use of ox/oy/oz, we should
    // do the same here
    OrientationConfig orientation_config_;
    std::string label_;
};

}  // namespace sdk
}  // namespace viam
