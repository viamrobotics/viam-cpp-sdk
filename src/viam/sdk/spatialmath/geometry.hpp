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
    static GeometryConfig from_proto(viam::common::v1::Geometry proto);
    google::protobuf::RepeatedField<viam::common::v1::Geometry> to_proto(const std::vector<viam::common::v1::Geometry>& geometries);

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
