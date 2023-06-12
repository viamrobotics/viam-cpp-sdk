#pragma once

#include <string>
#include <tuple>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace sdk {

enum GeometryType {
    box,
    sphere,
    point,
    unknown,
};

class GeometryConfig {
   public:
    struct coordinates {
        double x, y, z;
    };
    struct pose {
        double o_x, o_y, o_z;
    };
    viam::common::v1::Geometry to_proto() const;
    viam::common::v1::RectangularPrism box_proto() const;
    viam::common::v1::Sphere sphere_proto() const;
    viam::common::v1::Pose pose_proto() const;
    static GeometryConfig from_proto(const viam::common::v1::Geometry& proto);
    static std::vector<GeometryConfig> from_proto(
        const viam::common::v1::GetGeometriesResponse& proto);
    void set_coordinates(const coordinates& coordinates);
    void set_pose(const pose& pose);
    void set_radius(double r);
    void set_geometry_type(GeometryType type);
    void set_orientation_config(OrientationConfig config);
    void set_label(std::string label);
    double get_radius() const;
    coordinates get_coordinates() const;
    pose get_pose() const;
    GeometryType get_geometry_type() const;
    OrientationConfig get_orientation_config() const;
    std::string get_label() const;
    friend bool operator==(const GeometryConfig& lhs, const GeometryConfig& rhs);

   private:
    coordinates coordinates_;
    GeometryType geometry_type_;
    pose pose_;
    double r_;
    // TODO: if and when RDK makes more explicit use of ox/oy/oz, we should
    // do the same here
    OrientationConfig orientation_config_;
    std::string label_;
};

}  // namespace sdk
}  // namespace viam
