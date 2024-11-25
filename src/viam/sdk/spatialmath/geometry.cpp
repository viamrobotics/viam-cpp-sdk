#include <viam/sdk/spatialmath/geometry.hpp>

#include <string>
#include <tuple>

#include <boost/variant/get.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace sdk {

viam::common::v1::Sphere GeometryConfig::sphere_proto() const {
    try {
        viam::common::v1::Sphere sphere;
        const auto sphere_specifics = boost::get<struct sphere>(geometry_specifics_);
        sphere.set_radius_mm(sphere_specifics.radius);
        return sphere;
    } catch (...) {
        throw Exception(
            "Couldn't convert geometry config to sphere proto; sphere specifics not found");
    }
}

viam::common::v1::RectangularPrism GeometryConfig::box_proto() const {
    try {
        const auto box_specifics = boost::get<struct box>(geometry_specifics_);
        viam::common::v1::RectangularPrism box;
        viam::common::v1::Vector3 vec3;
        vec3.set_x(box_specifics.x);
        vec3.set_y(box_specifics.y);
        vec3.set_z(box_specifics.z);
        *box.mutable_dims_mm() = vec3;
        return box;
    } catch (...) {
        throw Exception("Couldn't convert geometry config to box proto; box specifics not found");
    }
}

viam::common::v1::Capsule GeometryConfig::capsule_proto() const {
    try {
        const auto capsule_specifics = boost::get<struct capsule>(geometry_specifics_);
        viam::common::v1::Capsule capsule;
        capsule.set_radius_mm(capsule_specifics.radius);
        capsule.set_length_mm(capsule_specifics.length);
        return capsule;
    } catch (...) {
        throw Exception(
            "Couldn't convert geometry config to capsule proto; capsule specifics not found");
    }
}

GeometryConfig::GeometryConfig() : geometry_type_(GeometryType::box) {}

GeometryConfig GeometryConfig::from_proto(const viam::common::v1::Geometry& proto) {
    GeometryConfig cfg;
    const auto& pose = proto.center();
    cfg.pose_ = v2::from_proto(pose);
    cfg.label_ = proto.label();

    switch (proto.geometry_type_case()) {
        case viam::common::v1::Geometry::GeometryTypeCase::kBox: {
            cfg.geometry_type_ = GeometryType::box;
            struct box box;
            box.x = proto.box().dims_mm().x();
            box.y = proto.box().dims_mm().y();
            box.z = proto.box().dims_mm().z();
            cfg.set_geometry_specifics(box);
            return cfg;
        }
        case viam::common::v1::Geometry::GeometryTypeCase::kSphere: {
            auto r = proto.sphere().radius_mm();
            if (r == 0) {
                cfg.geometry_type_ = GeometryType::point;
            } else {
                cfg.geometry_type_ = GeometryType::sphere;
            }
            struct sphere sphere({r});
            cfg.set_geometry_specifics(sphere);
            return cfg;
        }
        case viam::common::v1::Geometry::GeometryTypeCase::kCapsule: {
            cfg.geometry_type_ = GeometryType::capsule;
            struct capsule capsule;
            capsule.radius = proto.capsule().radius_mm();
            capsule.length = proto.capsule().length_mm();
            cfg.set_geometry_specifics(capsule);
            return cfg;
        }
        case viam::common::v1::Geometry::GeometryTypeCase::GEOMETRY_TYPE_NOT_SET:
        default: {
            throw Exception(ErrorCondition::k_not_supported, "Geometry type is not supported");
        }
    }
}
std::vector<GeometryConfig> GeometryConfig::from_proto(
    const viam::common::v1::GetGeometriesResponse& proto) {
    std::vector<GeometryConfig> response;
    for (const auto& geometry : proto.geometries()) {
        response.push_back(from_proto(geometry));
    }
    return response;
}

viam::common::v1::Geometry GeometryConfig::to_proto() const {
    viam::common::v1::Geometry geometry_;
    *geometry_.mutable_label() = label_;
    *geometry_.mutable_center() = v2::to_proto(pose_);
    switch (geometry_type_) {
        case GeometryType::box: {
            *geometry_.mutable_box() = box_proto();
            return geometry_;
        }
        case GeometryType::sphere: {
            *geometry_.mutable_sphere() = sphere_proto();
            return geometry_;
        }
        case point: {
            viam::common::v1::Sphere sphere;
            sphere.set_radius_mm(0);
            *geometry_.mutable_sphere() = sphere;
            return geometry_;
        }
        case capsule: {
            *geometry_.mutable_capsule() = capsule_proto();
            return geometry_;
        }
        case unknown:
        default: {
            if (pose_.coordinates.x == 0 && pose_.coordinates.y == 0 && pose_.coordinates.z == 0) {
                *geometry_.mutable_box() = box_proto();
            } else {
                *geometry_.mutable_sphere() = sphere_proto();
            }
            return geometry_;
        }
    }
}

void GeometryConfig::set_pose(pose pose) {
    pose_ = std::move(pose);
}

void GeometryConfig::set_geometry_specifics(geometry_specifics gs) {
    geometry_specifics_ = std::move(gs);
}

void GeometryConfig::set_geometry_type(GeometryType type) {
    geometry_type_ = std::move(type);
}

void GeometryConfig::set_orientation(Orientation o) {
    orientation_ = std::move(o);
}

void GeometryConfig::set_label(std::string label) {
    label_ = std::move(label);
}

const geometry_specifics& GeometryConfig::get_geometry_specifics() const {
    return geometry_specifics_;
}

GeometryType GeometryConfig::get_geometry_type() const {
    return geometry_type_;
}

const pose& GeometryConfig::get_pose() const {
    return pose_;
}

const Orientation& GeometryConfig::get_orientation() const {
    return orientation_;
}

const std::string& GeometryConfig::get_label() const {
    return label_;
}

std::ostream& operator<<(std::ostream& os, const pose& v) {
    os << "{ coordinates: "
       << "{ x: " << v.coordinates.x << ", y: " << v.coordinates.y << ", z: " << v.coordinates.z
       << "},\norientation: "
       << "{ o_x: " << v.orientation.o_x << ", o_y: " << v.orientation.o_y
       << ", o_z: " << v.orientation.o_z << "},\ntheta: " << v.theta << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const geo_point& v) {
    os << "{ latitude: " << v.latitude << ", longitude: " << v.longitude << "}\n";
    return os;
}

bool operator==(const coordinates& lhs, const coordinates& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator==(const pose_orientation& lhs, const pose_orientation& rhs) {
    return lhs.o_x == rhs.o_x && lhs.o_y == rhs.o_y && lhs.o_z == rhs.o_z;
}

bool operator==(const pose& lhs, const pose& rhs) {
    return lhs.coordinates == rhs.coordinates && lhs.orientation == rhs.orientation &&
           lhs.theta == rhs.theta;
}

bool operator==(const struct box& lhs, const struct box& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator==(const struct sphere& lhs, const struct sphere& rhs) {
    return lhs.radius == rhs.radius;
}

bool operator==(const struct capsule& lhs, const struct capsule& rhs) {
    return lhs.radius == rhs.radius && lhs.length == rhs.length;
}

bool operator==(const GeometryConfig& lhs, const GeometryConfig& rhs) {
    return std::tie(lhs.pose_.coordinates,
                    lhs.pose_.orientation,
                    lhs.label_,
                    lhs.geometry_type_,
                    lhs.geometry_specifics_) == std::tie(rhs.pose_.coordinates,
                                                         rhs.pose_.orientation,
                                                         rhs.label_,
                                                         rhs.geometry_type_,
                                                         rhs.geometry_specifics_);
}

bool operator==(const geo_point& lhs, const geo_point& rhs) {
    return std::tie(lhs.latitude, lhs.longitude) == std::tie(rhs.latitude, rhs.longitude);
}

bool operator==(const geo_geometry& lhs, const geo_geometry& rhs) {
    return std::tie(lhs.location, lhs.geometries) == std::tie(rhs.location, rhs.geometries);
}

common::v1::GeoPoint geo_point::to_proto() const {
    common::v1::GeoPoint proto;
    proto.set_latitude(latitude);
    proto.set_longitude(longitude);

    return proto;
}

geo_point geo_point::from_proto(const common::v1::GeoPoint& proto) {
    struct geo_point geo_point;
    geo_point.latitude = proto.latitude();
    geo_point.longitude = proto.longitude();

    return geo_point;
}

common::v1::GeoGeometry geo_geometry::to_proto() const {
    common::v1::GeoGeometry proto;
    *proto.mutable_location() = location.to_proto();

    for (const auto& geometry : geometries) {
        *proto.mutable_geometries()->Add() = geometry.to_proto();
    }

    return proto;
}

geo_geometry geo_geometry::from_proto(const common::v1::GeoGeometry& proto) {
    struct geo_geometry geo_geometry;

    geo_geometry.location = geo_point::from_proto(proto.location());
    for (const auto& proto_geometry : proto.geometries()) {
        auto geometry = GeometryConfig::from_proto(proto_geometry);
        geo_geometry.geometries.push_back(std::move(geometry));
    }

    return geo_geometry;
}

}  // namespace sdk
}  // namespace viam
