#pragma once

#include <common/v1/common.pb.h>

#include <spatialmath/orientation.hpp>
#include <string>
#include <tuple>

enum GeometryType {
    box,
    sphere,
    point,
    unknown,
};

class GeometryConfig {
    GeometryType geometry_type;
    double x;
    double y;
    double z;
    double r;
    // TODO: if and when RDK makes more explicit use of ox/oy/oz, we should
    // do the same here
    double o_x;
    double o_y;
    double o_z;
    OrientationConfig orientation_config;
    std::string label;

   public:
    viam::common::v1::Geometry to_proto();
    viam::common::v1::RectangularPrism box_proto();
    viam::common::v1::Sphere sphere_proto();
    viam::common::v1::Pose pose_proto();
    static GeometryConfig from_proto(viam::common::v1::Geometry proto);
};
