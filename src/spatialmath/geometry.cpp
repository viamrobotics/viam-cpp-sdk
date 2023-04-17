#include <spatialmath/geometry.hpp>

#include <string>
#include <tuple>

#include <common/v1/common.pb.h>

#include <spatialmath/orientation.hpp>

namespace viam {
namespace cppsdk {

viam::common::v1::Sphere GeometryConfig::sphere_proto() {
    viam::common::v1::Sphere sphere;
    sphere.set_radius_mm(r);
    return sphere;
}

viam::common::v1::RectangularPrism GeometryConfig::box_proto() {
    viam::common::v1::RectangularPrism box;
    viam::common::v1::Vector3 vec3;
    vec3.set_x(x);
    vec3.set_y(y);
    vec3.set_z(z);
    *box.mutable_dims_mm() = vec3;
    return box;
}

viam::common::v1::Pose GeometryConfig::pose_proto() {
    viam::common::v1::Pose pose;
    pose.set_x(x);
    pose.set_y(y);
    pose.set_z(z);
    pose.set_o_x(0);
    pose.set_o_y(0);
    pose.set_o_z(1);
    pose.set_theta(0);
    return pose;
}

GeometryConfig GeometryConfig::from_proto(viam::common::v1::Geometry proto) {
    GeometryConfig cfg;
    switch (proto.geometry_type_case()) {
        case viam::common::v1::Geometry::GeometryTypeCase::kBox: {
            cfg.geometry_type = box;
            cfg.x = proto.box().dims_mm().x();
            cfg.y = proto.box().dims_mm().y();
            cfg.z = proto.box().dims_mm().z();
            return cfg;
        }
        case viam::common::v1::Geometry::GeometryTypeCase::kSphere: {
            cfg.r = proto.sphere().radius_mm();
            if (cfg.r == 0) {
                cfg.geometry_type = point;
            } else {
                cfg.geometry_type = sphere;
            }
            return cfg;
        }
        case viam::common::v1::Geometry::GeometryTypeCase::GEOMETRY_TYPE_NOT_SET:
        default: {
            throw "Geometry type is not supported";
        }
    }
}

viam::common::v1::Geometry GeometryConfig::to_proto() {
    viam::common::v1::Geometry geometry_;
    *geometry_.mutable_label() = label;
    switch (geometry_type) {
        case box: {
            *geometry_.mutable_box() = box_proto();
            return geometry_;
        }
        case sphere: {
            *geometry_.mutable_sphere() = sphere_proto();
            return geometry_;
        }
        case point: {
            *geometry_.mutable_center() = pose_proto();
            viam::common::v1::Sphere sphere;
            sphere.set_radius_mm(0);
            *geometry_.mutable_sphere() = sphere;

            return geometry_;
        }
        case unknown: {
            if (x == 0 && y == 0 && z == 0) {
                *geometry_.mutable_box() = box_proto();
            } else {
                *geometry_.mutable_sphere() = sphere_proto();
            }
            return geometry_;
        }
    }
};

}  // namespace cppsdk
}  // namespace viam
