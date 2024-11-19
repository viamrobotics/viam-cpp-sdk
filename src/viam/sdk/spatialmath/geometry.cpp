#include <viam/sdk/spatialmath/geometry.hpp>

#include <string>

#include <boost/variant/get.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace sdk {

GeometryConfig::GeometryConfig() : geometry_type_(GeometryType::box) {}

void GeometryConfig::set_coordinates(coordinates coordinates) {
    pose_.coordinates = std::move(coordinates);
}
void GeometryConfig::set_pose(pose pose) {
    pose_ = std::move(pose);
}
void GeometryConfig::set_geometry_specifics(geometry_specifics gs) {
    geometry_specifics_ = std::move(gs);
}
void GeometryConfig::set_pose_orientation(pose_orientation orientation) {
    pose_.orientation = std::move(orientation);
}
void GeometryConfig::set_theta(double theta) {
    pose_.theta = std::move(theta);
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
geometry_specifics GeometryConfig::get_geometry_specifics() const {
    return geometry_specifics_;
}
double GeometryConfig::get_theta() const {
    return pose_.theta;
}
GeometryType GeometryConfig::get_geometry_type() const {
    return geometry_type_;
}
coordinates GeometryConfig::get_coordinates() const {
    return pose_.coordinates;
}
pose GeometryConfig::get_pose() const {
    return pose_;
}
OrientationConfig GeometryConfig::get_orientation_config() const {
    return orientation_config_;
}

std::string GeometryConfig::get_label() const {
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
    const auto& lhs_coordinates = lhs.pose_.coordinates;
    const auto& rhs_coordinates = rhs.pose_.coordinates;
    const auto& lhs_orientation = lhs.pose_.orientation;
    const auto& rhs_orientation = rhs.pose_.orientation;
    return lhs_coordinates.x == rhs_coordinates.x && lhs_coordinates.y == rhs_coordinates.y &&
           lhs_coordinates.z == rhs_coordinates.z && lhs_orientation.o_x == rhs_orientation.o_x &&
           lhs_orientation.o_y == rhs_orientation.o_y &&
           lhs_orientation.o_z == rhs_orientation.o_z && lhs.label_ == rhs.label_ &&
           lhs.geometry_type_ == rhs.geometry_type_ &&
           lhs.geometry_specifics_ == rhs.geometry_specifics_;
}

bool operator==(const geo_point& lhs, const geo_point& rhs) {
    return lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude;
}

bool operator==(const geo_geometry& lhs, const geo_geometry& rhs) {
    return lhs.location == rhs.location && lhs.geometries == rhs.geometries;
}

}  // namespace sdk
}  // namespace viam
