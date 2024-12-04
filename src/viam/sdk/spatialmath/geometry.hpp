#pragma once

#include <array>
#include <string>
#include <tuple>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/common/proto_convert_vector.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>

VIAM_SDK_API_FWD_NAMESPACE_BEGIN(common)

class RectangularPrism;
class Sphere;
class Capsule;

class Geometry;
class GetGeometriesResponse;

class GeoPoint;
class GeoGeometry;

VIAM_SDK_API_FWD_NAMESPACE_END

namespace viam {
namespace sdk {

// TODO(RSDK-4553): add thorough documentation to this whole file.

enum class GeometryType { box, sphere, capsule, point };

struct box {
    double x;
    double y;
    double z;
    friend bool operator==(const box& lhs, const box& rhs);
};
struct sphere {
    double radius;
    friend bool operator==(const sphere& lhs, const sphere& rhs);
};
struct capsule {
    double radius;
    double length;
    friend bool operator==(const capsule& lhs, const capsule& rhs);
};

typedef boost::variant<struct box, struct sphere, struct capsule> geometry_specifics;

class GeometryConfig {
   public:
    GeometryConfig() = default;

    GeometryConfig(pose, geometry_specifics, Orientation, std::string label);
    GeometryConfig(pose, geometry_specifics, std::string label);

    double get_theta() const;
    const pose& get_pose() const;
    const geometry_specifics& get_geometry_specifics() const;
    GeometryType get_geometry_type() const;
    const Orientation& get_orientation() const;
    const std::string& get_label() const;

    friend bool operator==(const GeometryConfig& lhs, const GeometryConfig& rhs);

   private:
    pose pose_;
    geometry_specifics geometry_specifics_;
    // TODO: if and when RDK makes more explicit use of ox/oy/oz, we should
    // do the same here
    Orientation orientation_;
    std::string label_;
};

struct geo_point {
    // TODO it really bugs me that this is not in lat-long but this would break existing
    // aggregate initializers
    double longitude, latitude;

    friend bool operator==(const geo_point& lhs, const geo_point& rhs);
    friend std::ostream& operator<<(std::ostream& os, const geo_point& v);
};

struct geo_geometry {
    geo_point location;
    std::vector<GeometryConfig> geometries;

    friend bool operator==(const geo_geometry& lhs, const geo_geometry& rhs);
};

namespace proto_convert_details {

template <>
struct to_proto<box> {
    void operator()(const box&, common::v1::RectangularPrism*) const;
};

template <>
struct from_proto<common::v1::RectangularPrism> {
    box operator()(const common::v1::RectangularPrism*) const;
};

template <>
struct to_proto<sphere> {
    void operator()(const sphere&, common::v1::Sphere*) const;
};

template <>
struct from_proto<common::v1::Sphere> {
    sphere operator()(const common::v1::Sphere*) const;
};

template <>
struct to_proto<capsule> {
    void operator()(const capsule&, common::v1::Capsule*) const;
};

template <>
struct from_proto<common::v1::Capsule> {
    capsule operator()(const common::v1::Capsule*) const;
};

template <>
struct to_proto<GeometryConfig> {
    void operator()(const GeometryConfig&, common::v1::Geometry*) const;
};

template <>
struct from_proto<common::v1::Geometry> {
    GeometryConfig operator()(const common::v1::Geometry*) const;
};

template <>
struct to_proto<geo_point> {
    void operator()(const geo_point&, common::v1::GeoPoint*) const;
};

template <>
struct from_proto<common::v1::GeoPoint> {
    geo_point operator()(const common::v1::GeoPoint*) const;
};

template <>
struct to_proto<geo_geometry> {
    void operator()(const geo_geometry&, common::v1::GeoGeometry*) const;
};

template <>
struct from_proto<common::v1::GeoGeometry> {
    geo_geometry operator()(const common::v1::GeoGeometry*) const;
};

template <>
struct from_proto<common::v1::GetGeometriesResponse> {
    std::vector<GeometryConfig> operator()(const common::v1::GetGeometriesResponse*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
