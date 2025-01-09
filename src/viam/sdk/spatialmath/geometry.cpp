#include <viam/sdk/spatialmath/geometry.hpp>

#include <string>
#include <tuple>

#include <boost/variant/get.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace sdk {

GeometryConfig::GeometryConfig(pose p, geometry_specifics gs, Orientation o, std::string label)
    : pose_(std::move(p)),
      geometry_specifics_(std::move(gs)),
      orientation_(std::move(o)),
      label_(std::move(label)) {}

GeometryConfig::GeometryConfig(pose p, geometry_specifics gs, std::string label)
    : GeometryConfig(std::move(p), std::move(gs), {}, std::move(label)) {}

const geometry_specifics& GeometryConfig::get_geometry_specifics() const {
    return geometry_specifics_;
}

GeometryType GeometryConfig::get_geometry_type() const {
    struct Visitor {
        auto operator()(const box&) const {
            return GeometryType::box;
        }
        auto operator()(const sphere& s) const {
            return s.radius == 0.0 ? GeometryType::point : GeometryType::sphere;
        }
        auto operator()(const capsule&) const {
            return GeometryType::capsule;
        }
    };

    return boost::apply_visitor(Visitor{}, geometry_specifics_);
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
    return std::tie(
               lhs.pose_.coordinates, lhs.pose_.orientation, lhs.label_, lhs.geometry_specifics_) ==
           std::tie(
               rhs.pose_.coordinates, rhs.pose_.orientation, rhs.label_, rhs.geometry_specifics_);
}

bool operator==(const geo_point& lhs, const geo_point& rhs) {
    return std::tie(lhs.latitude, lhs.longitude) == std::tie(rhs.latitude, rhs.longitude);
}

bool operator==(const geo_geometry& lhs, const geo_geometry& rhs) {
    return std::tie(lhs.location, lhs.geometries) == std::tie(rhs.location, rhs.geometries);
}

namespace proto_convert_details {

void to_proto_impl<box>::operator()(const box& self, common::v1::RectangularPrism* proto) const {
    *(proto->mutable_dims_mm()) = v2::to_proto(Vector3{self.x, self.y, self.z});
}

box from_proto_impl<common::v1::RectangularPrism>::operator()(
    const common::v1::RectangularPrism* proto) const {
    const auto& dims = proto->dims_mm();
    return {dims.x(), dims.y(), dims.z()};
}

void to_proto_impl<sphere>::operator()(const sphere& self, common::v1::Sphere* proto) const {
    proto->set_radius_mm(self.radius);
}

sphere from_proto_impl<common::v1::Sphere>::operator()(const common::v1::Sphere* proto) const {
    return {proto->radius_mm()};
}

void to_proto_impl<capsule>::operator()(const capsule& self, common::v1::Capsule* proto) const {
    proto->set_radius_mm(self.radius);
    proto->set_length_mm(self.length);
}

capsule from_proto_impl<common::v1::Capsule>::operator()(const common::v1::Capsule* proto) const {
    return {proto->radius_mm(), proto->length_mm()};
}

void to_proto_impl<GeometryConfig>::operator()(const GeometryConfig& self,
                                               common::v1::Geometry* proto) const {
    struct Visitor {
        common::v1::Geometry& geometry;

        void operator()(const box& b) {
            *geometry.mutable_box() = v2::to_proto(b);
        }

        void operator()(const sphere& s) {
            *geometry.mutable_sphere() = v2::to_proto(s);
        }

        void operator()(const capsule& c) {
            *geometry.mutable_capsule() = v2::to_proto(c);
        }
    };

    boost::apply_visitor(Visitor{*proto}, self.get_geometry_specifics());

    *(proto->mutable_label()) = self.get_label();
    *(proto->mutable_center()) = v2::to_proto(self.get_pose());
}

GeometryConfig from_proto_impl<common::v1::Geometry>::operator()(
    const common::v1::Geometry* proto) const {
    auto get_specifics = [proto]() -> geometry_specifics {
        switch (proto->geometry_type_case()) {
            case viam::common::v1::Geometry::GeometryTypeCase::kBox:
                return v2::from_proto(proto->box());
            case viam::common::v1::Geometry::GeometryTypeCase::kSphere:
                return v2::from_proto(proto->sphere());
            case viam::common::v1::Geometry::GeometryTypeCase::kCapsule:
                return v2::from_proto(proto->capsule());
            case viam::common::v1::Geometry::GeometryTypeCase::
                GEOMETRY_TYPE_NOT_SET:  // fallthrough
            default:
                throw Exception(ErrorCondition::k_not_supported, "Geometry type is not supported");
        }
    };

    return GeometryConfig(v2::from_proto(proto->center()), get_specifics(), proto->label());
}

void to_proto_impl<geo_point>::operator()(const geo_point& self,
                                          common::v1::GeoPoint* proto) const {
    proto->set_latitude(self.latitude);
    proto->set_longitude(self.longitude);
}

geo_point from_proto_impl<common::v1::GeoPoint>::operator()(
    const common::v1::GeoPoint* proto) const {
    geo_point result;
    result.latitude = proto->latitude();
    result.longitude = proto->longitude();

    return result;
}

void to_proto_impl<geo_geometry>::operator()(const geo_geometry& self,
                                             common::v1::GeoGeometry* proto) const {
    *(proto->mutable_location()) = v2::to_proto(self.location);
    *(proto->mutable_geometries()) = impl::to_repeated_field(self.geometries);
}

geo_geometry from_proto_impl<common::v1::GeoGeometry>::operator()(
    const common::v1::GeoGeometry* proto) const {
    return {v2::from_proto(proto->location()), impl::from_repeated_field(proto->geometries())};
}

std::vector<GeometryConfig> from_proto_impl<common::v1::GetGeometriesResponse>::operator()(
    const common::v1::GetGeometriesResponse* proto) const {
    return impl::from_repeated_field(proto->geometries());
}

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
