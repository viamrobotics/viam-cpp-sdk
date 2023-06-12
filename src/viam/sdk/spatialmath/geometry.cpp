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
    vec3.set_x(coordinates_.x);
    vec3.set_y(coordinates_.y);
    vec3.set_z(coordinates_.z);
    *box.mutable_dims_mm() = vec3;
    return box;
}

viam::common::v1::Pose GeometryConfig::pose_proto() const {
    viam::common::v1::Pose pose;
    pose.set_x(coordinates_.x);
    pose.set_y(coordinates_.y);
    pose.set_z(coordinates_.z);
    pose.set_o_x(pose_.o_x);
    pose.set_o_y(pose_.o_y);
    pose.set_o_z(pose_.o_z);
    pose.set_theta(0);
    return pose;
}

GeometryConfig GeometryConfig::from_proto(const viam::common::v1::Geometry& proto) {
    GeometryConfig cfg;
    const auto& pose = proto.center();
    cfg.coordinates_.x = pose.x();
    cfg.coordinates_.y = pose.y();
    cfg.coordinates_.z = pose.z();
    cfg.pose_.o_x = pose.o_x();
    cfg.pose_.o_y = pose.o_y();
    cfg.pose_.o_z = pose.o_z();
    cfg.label_ = proto.label();

    switch (proto.geometry_type_case()) {
        case viam::common::v1::Geometry::GeometryTypeCase::kBox: {
            cfg.geometry_type_ = box;
            cfg.coordinates_.x = proto.box().dims_mm().x();
            cfg.coordinates_.y = proto.box().dims_mm().y();
            cfg.coordinates_.z = proto.box().dims_mm().z();
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
            if (coordinates_.x == 0 && coordinates_.y == 0 && coordinates_.z == 0) {
                *geometry_.mutable_box() = box_proto();
            } else {
                *geometry_.mutable_sphere() = sphere_proto();
            }
            return geometry_;
        }
    }
}
void GeometryConfig::set_coordinates(coordinates coordinates) {
    coordinates_ = std::move(coordinates);
}
void GeometryConfig::set_pose(pose pose) {
    pose_ = std::move(pose);
}
void GeometryConfig::set_radius(double r) {
    r_ = std::move(r);
}
void GeometryConfig::set_geometry_type(GeometryType type) {
    geometry_type_ = std::move(type);
}
void GeometryConfig::set_orientation_config(OrientationConfig config) {
    orientation_config_ = std::move(config);
}
void GeometryConfig::set_label(std::string label) {
    label_ = std::move(label);
}
double GeometryConfig::get_radius() const {
    return r_;
}

GeometryType GeometryConfig::get_geometry_type() const {
    return geometry_type_;
}
GeometryConfig::coordinates GeometryConfig::get_coordinates() const {
    return coordinates_;
}
GeometryConfig::pose GeometryConfig::get_pose() const {
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
    return lhs.coordinates_.x == rhs.coordinates_.x && lhs.coordinates_.y == rhs.coordinates_.y &&
           lhs.coordinates_.z == rhs.coordinates_.z && lhs.pose_.o_x == rhs.pose_.o_x &&
           lhs.pose_.o_y == rhs.pose_.o_y && lhs.pose_.o_z == rhs.pose_.o_z &&
           lhs.label_ == rhs.label_ && lhs.geometry_type_ == rhs.geometry_type_;
}

}  // namespace sdk
}  // namespace viam
