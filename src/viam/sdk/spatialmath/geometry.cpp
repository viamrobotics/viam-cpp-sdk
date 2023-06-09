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

GeometryConfig GeometryConfig::from_proto(const viam::common::v1::Geometry& proto) {
    GeometryConfig cfg;
    auto& pose = proto.center();
    cfg.x_ = pose.x();
    cfg.y_ = pose.y();
    cfg.z_ = pose.z();
    cfg.o_x_ = pose.o_x();
    cfg.o_y_ = pose.o_y();
    cfg.o_z_ = pose.o_z();
    cfg.label_ = proto.label();

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
std::vector<GeometryConfig> GeometryConfig::from_proto(
    const viam::common::v1::GetGeometriesResponse& proto) {
    std::vector<GeometryConfig> response;
    for (const auto& geometry : proto.geometries()) {
        response.push_back(from_proto(std::move(geometry)));
    }
    return response;
}

viam::common::v1::Geometry GeometryConfig::to_proto() const {
    viam::common::v1::Geometry geometry_;
    *geometry_.mutable_label() = label_;
    *geometry_.mutable_center() = pose_proto();
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
void GeometryConfig::set_coordinates(GeometryConfig::coordinates coord) {
    x_ = coord.x;
    y_ = coord.y;
    z_ = coord.z;
}

void GeometryConfig::set_pose(GeometryConfig::pose pos) {
    o_x_ = pos.x;
    o_y_ = pos.y;
    o_z_ = pos.z;
}
void GeometryConfig::set_radius(double r) {
    r_ = r;
}
void GeometryConfig::set_geometry_type(GeometryType type) {
    geometry_type_ = type;
}
void GeometryConfig::set_orientation_config(OrientationConfig config) {
    orientation_config_ = config;
}
void GeometryConfig::set_label(std::string label) {
    label_ = label;
}
GeometryConfig::coordinates GeometryConfig::get_coordinates() const {
    return {x_, y_, z_};
}

GeometryConfig::pose GeometryConfig::get_pose() const {
    return {o_x_, o_y_, o_z_};
}

double GeometryConfig::get_radius() const {
    return r_;
}

GeometryType GeometryConfig::get_geometry_type() const {
    return geometry_type_;
}

OrientationConfig GeometryConfig::get_orientation_config() const {
    return orientation_config_;
}

std::string GeometryConfig::get_label() const {
    return label_;
}

bool operator==(const GeometryConfig& lhs, const GeometryConfig& rhs) {
    if (lhs.geometry_type_ == GeometryType::sphere && lhs.r_ != rhs.r_) {
        return false;
    }
    return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ == rhs.z_ && lhs.o_x_ == rhs.o_x_ &&
           lhs.o_y_ == rhs.o_y_ && lhs.o_z_ == rhs.o_z_ && lhs.label_ == rhs.label_ &&
           lhs.geometry_type_ == rhs.geometry_type_;
}

}  // namespace sdk
}  // namespace viam
