#include <viam/sdk/spatialmath/geometry.hpp>

#include <string>
#include <tuple>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace sdk {

viam::common::v1::Sphere GeometryConfig::sphere_proto() const {
    viam::common::v1::Sphere sphere;
    sphere.set_radius_mm(r_);
    return sphere;
}

viam::common::v1::RectangularPrism GeometryConfig::box_proto() const {
    viam::common::v1::RectangularPrism box;
    viam::common::v1::Vector3 vec3;
    vec3.set_x(x_);
    vec3.set_y(y_);
    vec3.set_z(z_);
    *box.mutable_dims_mm() = vec3;
    return box;
}

viam::common::v1::Pose GeometryConfig::pose_proto() const {
    viam::common::v1::Pose pose;
    pose.set_x(x_);
    pose.set_y(y_);
    pose.set_z(z_);
    pose.set_o_x(o_x_);
    pose.set_o_y(o_y_);
    pose.set_o_z(o_z_);
    pose.set_theta(0);
    return pose;
}

GeometryConfig GeometryConfig::from_proto(const viam::common::v1::Geometry &proto) {
    GeometryConfig cfg;
    switch (proto.geometry_type_case()) {
        case viam::common::v1::Geometry::GeometryTypeCase::kBox: {
            cfg.geometry_type_ = box;
            cfg.x_ = proto.box().dims_mm().x();
            cfg.y_ = proto.box().dims_mm().y();
            cfg.z_ = proto.box().dims_mm().z();
            return cfg;
        }
        case viam::common::v1::Geometry::GeometryTypeCase::kSphere: {
            cfg.r_ = proto.sphere().radius_mm();
            if (cfg.r_ == 0) {
                cfg.geometry_type_ = point;
            } else {
                cfg.geometry_type_ = sphere;
            }
            return cfg;
        }
        case viam::common::v1::Geometry::GeometryTypeCase::GEOMETRY_TYPE_NOT_SET:
        default: {
            throw "Geometry type is not supported";
        }
    }
}
std::vector<GeometryConfig> GeometryConfig::from_proto(const viam::common::v1::GetGeometriesResponse &proto){
    std::vector<GeometryConfig> response;
    for(const viam::common::v1::Geometry &geometry : proto.geometries()){
        response.push_back(from_proto(geometry));
    }
    return response;
}

viam::common::v1::Geometry GeometryConfig::to_proto() const {
    viam::common::v1::Geometry geometry_;
    *geometry_.mutable_label() = label_;
    switch (geometry_type_) {
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
        case unknown:
        default: {
            if (x_ == 0 && y_ == 0 && z_ == 0) {
                *geometry_.mutable_box() = box_proto();
            } else {
                *geometry_.mutable_sphere() = sphere_proto();
            }
            return geometry_;
        }
    }
}

}  // namespace sdk
}  // namespace viam
