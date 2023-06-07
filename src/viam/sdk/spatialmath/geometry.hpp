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
    viam::common::v1::Geometry to_proto() const;
    viam::common::v1::RectangularPrism box_proto() const;
    viam::common::v1::Sphere sphere_proto() const;
    viam::common::v1::Pose pose_proto() const;
    static GeometryConfig from_proto(const viam::common::v1::Geometry &proto);
    static std::vector<GeometryConfig> from_proto(const viam::common::v1::GetGeometriesResponse &proto);
    void set_coordinates(double x, double y, double z) {
        x_ = x;
        y_ = y;
        z_ = z;
    }
    void set_pose(double x, double y, double z) {
        o_x_ = x;
        o_y_ = y;
        o_z_ = z;
    }
    void set_radius(double r) {
        r_ = r;
    }
    void set_geometry_type(GeometryType type) {
        geometry_type_ = type;
    }
    void set_orientation_config(OrientationConfig config) {
        orientation_config_ = config;
    }
    void set_label(std::string label) {
        label_ = label;
    }
    std::vector<double> get_coordinates(){
        return {x_, y_, z_};
    }
    std::vector<double> get_pose(){
        return {o_x_, o_y_, o_z_};
    }
    double get_radius(){
        return r_;
    }
    GeometryType get_geometry_type(){
        return geometry_type_;
    }
    OrientationConfig get_orientation_config(){
        return orientation_config_;
    }
    std::string get_label(){
        return label_;
    }

   private:
    GeometryType geometry_type_;
    double x_;
    double y_;
    double z_;
    double r_;
    // TODO: if and when RDK makes more explicit use of ox/oy/oz, we should
    // do the same here
    double o_x_;
    double o_y_;
    double o_z_;
    OrientationConfig orientation_config_;
    std::string label_;
};

}  // namespace sdk
}  // namespace viam
