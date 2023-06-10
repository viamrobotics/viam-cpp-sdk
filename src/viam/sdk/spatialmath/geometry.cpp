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
    vec3.set_x(coordinates_.x_);
    vec3.set_y(coordinates_.y_);
    vec3.set_z(coordinates_.z_);
    *box.mutable_dims_mm() = vec3;
    return box;
}

viam::common::v1::Pose GeometryConfig::pose_proto() const {
    viam::common::v1::Pose pose;
    pose.set_x(coordinates_.x_);
    pose.set_y(coordinates_.y_);
    pose.set_z(coordinates_.z_);
    pose.set_o_x(pose_.o_x_);
    pose.set_o_y(pose_.o_y_);
    pose.set_o_z(pose_.o_z_);
    pose.set_theta(0);
    return pose;
}

GeometryConfig GeometryConfig::from_proto(const viam::common::v1::Geometry& proto) {
    GeometryConfig cfg;
    const auto& pose = proto.center();
    cfg.coordinates_.x_ = pose.x();
    cfg.coordinates_.y_ = pose.y();
    cfg.coordinates_.z_ = pose.z();
    cfg.pose_.o_x_ = pose.o_x();
    cfg.pose_.o_y_ = pose.o_y();
    cfg.pose_.o_z_ = pose.o_z();
    cfg.label_ = proto.label();

    switch (proto.geometry_type_case()) {
        case viam::common::v1::Geometry::GeometryTypeCase::kBox: {
            cfg.geometry_type_ = box;
            cfg.coordinates_.x_ = proto.box().dims_mm().x();
            cfg.coordinates_.y_ = proto.box().dims_mm().y();
            cfg.coordinates_.z_ = proto.box().dims_mm().z();
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
            if (coordinates_.x_ == 0 && coordinates_.y_ == 0 && coordinates_.z_ == 0) {
                *geometry_.mutable_box() = box_proto();
            } else {
                *geometry_.mutable_sphere() = sphere_proto();
            }
            return geometry_;
        }
    }
}
void GeometryConfig::set_coordinates(const Coordinates& coordinates) {
    coordinates_ = coordinates;
}
void GeometryConfig::set_pose(const Pose& pose) {
    pose_ = pose;
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
double GeometryConfig::get_radius() const {
    return r_;
}

GeometryType GeometryConfig::get_geometry_type() const {
    return geometry_type_;
}
Coordinates GeometryConfig::get_coordinates() const {
    return coordinates_;
}
Pose GeometryConfig::get_pose() const {
    return pose_;
}
OrientationConfig GeometryConfig::get_orientation_config() const {
    return orientation_config_;
}

std::string GeometryConfig::get_label() const {
    return label_;
}

bool operator==(const GeometryConfig& lhs, const GeometryConfig& rhs) {
    // Only spheres contain a radius variable that is passed "round-trip" to and from proto
    if (lhs.geometry_type_ == GeometryType::sphere && lhs.r_ != rhs.r_) {
        return false;
    }
    return lhs.coordinates_.x_ == rhs.coordinates_.x_ &&
           lhs.coordinates_.y_ == rhs.coordinates_.y_ &&
           lhs.coordinates_.z_ == rhs.coordinates_.z_ && lhs.pose_.o_x_ == rhs.pose_.o_x_ &&
           lhs.pose_.o_y_ == rhs.pose_.o_y_ && lhs.pose_.o_z_ == rhs.pose_.o_z_ &&
           lhs.label_ == rhs.label_ && lhs.geometry_type_ == rhs.geometry_type_;
}

}  // namespace sdk
}  // namespace viam
