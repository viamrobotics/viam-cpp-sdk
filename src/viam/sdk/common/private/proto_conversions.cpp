#include "viam/sdk/common/utils.hpp"
#include <viam/sdk/common/private/proto_conversions.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>
#include <boost/variant/get.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/proto_value.hpp>

namespace viam {
namespace sdk {
namespace impl {

viam::common::v1::Vector3 to_proto(const Vector3& v3) {
    viam::common::v1::Vector3 result;
    result.set_x(v3.x());
    result.set_y(v3.y());
    result.set_z(v3.z());
    return result;
};

Vector3 from_proto(const viam::common::v1::Vector3& vec) {
    return {vec.x(), vec.y(), vec.z()};
}

response_metadata from_proto(const viam::common::v1::ResponseMetadata& proto) {
    response_metadata metadata;
    metadata.captured_at = timestamp_to_time_pt(proto.captured_at());
    return metadata;
}

viam::common::v1::ResponseMetadata to_proto(const response_metadata& metadata) {
    viam::common::v1::ResponseMetadata proto;
    google::protobuf::Timestamp ts = time_pt_to_timestamp(metadata.captured_at);
    *proto.mutable_captured_at() = std::move(ts);
    return proto;
}

std::chrono::microseconds from_proto(const google::protobuf::Duration& proto) {
    namespace sc = std::chrono;
    const sc::seconds seconds_part{proto.seconds()};
    const sc::nanoseconds nanos_part{proto.nanos()};

    const sc::microseconds from_seconds = sc::duration_cast<sc::microseconds>(seconds_part);
    sc::microseconds from_nanos = sc::duration_cast<sc::microseconds>(nanos_part);

    if ((nanos_part.count() < 0) && (from_nanos > nanos_part)) {
        from_nanos -= sc::microseconds(1);
    } else if ((nanos_part.count() > 0) && (from_nanos < nanos_part)) {
        from_nanos += sc::microseconds(1);
    }
    return from_seconds + from_nanos;
}

google::protobuf::Duration to_proto(const std::chrono::microseconds& duration) {
    namespace sc = std::chrono;

    const sc::seconds seconds = sc::duration_cast<sc::seconds>(duration);
    const sc::nanoseconds nanos = duration - seconds;

    google::protobuf::Duration proto;
    proto.set_nanos(static_cast<int32_t>(nanos.count()));
    proto.set_seconds(seconds.count());
    return proto;
}

viam::module::v1::HandlerMap to_proto(const HandlerMap_& hm) {
    viam::module::v1::HandlerMap proto;
    for (const auto& h : hm.handles()) {
        viam::module::v1::HandlerDefinition hd;
        for (const auto& model : h.second) {
            const std::string m = model.to_string();
            *hd.mutable_models()->Add() = m;
        }
        viam::robot::v1::ResourceRPCSubtype rpc_subtype;
        const Name name(h.first.api(), "", "");
        const viam::common::v1::ResourceName resource_name = to_proto(name);
        *rpc_subtype.mutable_subtype() = resource_name;
        *rpc_subtype.mutable_proto_service() = h.first.proto_service_name();
        *hd.mutable_subtype() = rpc_subtype;
        *proto.add_handlers() = hd;
    }

    return proto;
};

// NOLINTNEXTLINE(readability-const-return-type)
HandlerMap_ from_proto(const viam::module::v1::HandlerMap& proto) {
    HandlerMap_ hm;

    const google::protobuf::RepeatedPtrField<viam::module::v1::HandlerDefinition>& handlers =
        proto.handlers();

    for (const auto& handler : handlers) {
        const viam::common::v1::ResourceName name = handler.subtype().subtype();
        const API api(name.namespace_(), name.type(), name.subtype());
        const google::protobuf::DescriptorPool* pool =
            google::protobuf::DescriptorPool::generated_pool();
        const google::protobuf::ServiceDescriptor* sd = pool->FindServiceByName(name.type());
        const RPCSubtype handle(api, *sd);
        for (const auto& mod : handler.models()) {
            try {
                const Model model = Model::from_str(mod);
                hm.add_model(model, handle);
            } catch (std::string error) {  // NOLINT
                BOOST_LOG_TRIVIAL(error) << "Error processing model " + mod;
            }
        }
    }

    return hm;
}

viam::app::v1::ComponentConfig to_proto(const ResourceConfig& cfg) {
    viam::app::v1::ComponentConfig proto_cfg;
    const google::protobuf::RepeatedPtrField<viam::app::v1::ResourceLevelServiceConfig>
        service_configs;

    for (const auto& svc_cfg : cfg.service_config()) {
        viam::app::v1::ResourceLevelServiceConfig cfg;
        *cfg.mutable_type() = svc_cfg.type;
        *cfg.mutable_attributes() = map_to_struct(svc_cfg.attributes);
        *proto_cfg.mutable_service_configs()->Add() = cfg;
    }

    *proto_cfg.mutable_name() = cfg.name();
    *proto_cfg.mutable_namespace_() = cfg.namespace_();
    *proto_cfg.mutable_type() = cfg.type();
    *proto_cfg.mutable_api() = cfg.api().to_string();
    *proto_cfg.mutable_model() = cfg.model().to_string();
    *proto_cfg.mutable_attributes() = map_to_struct(cfg.attributes());
    for (const auto& dep : cfg.depends_on()) {
        *proto_cfg.mutable_depends_on()->Add() = dep;
    }
    *proto_cfg.mutable_frame() = to_proto(cfg.frame());

    return proto_cfg;
}

ResourceConfig from_proto(const viam::app::v1::ComponentConfig& proto_cfg) {
    LinkConfig frame;
    if (proto_cfg.has_frame()) {
        frame = from_proto(proto_cfg.frame());
    }

    return ResourceConfig(proto_cfg.type(),
                          proto_cfg.name(),
                          proto_cfg.namespace_(),
                          struct_to_map(proto_cfg.attributes()),
                          proto_cfg.api(),
                          Model::from_str(proto_cfg.model()),
                          frame);
};

WorldState::geometries_in_frame from_proto(const common::v1::GeometriesInFrame& proto) {
    WorldState::geometries_in_frame gif;
    for (const auto& geo : proto.geometries()) {
        gif.geometries.push_back(from_proto(geo));
    }
    gif.reference_frame = proto.reference_frame();

    return gif;
}

common::v1::GeometriesInFrame to_proto(const WorldState::geometries_in_frame& gif) {
    common::v1::GeometriesInFrame proto;

    *proto.mutable_reference_frame() = gif.reference_frame;
    for (const auto& geometry : gif.geometries) {
        *proto.mutable_geometries()->Add() = to_proto(geometry);
    }

    return proto;
}

WorldState::transform from_proto(const common::v1::Transform& proto) {
    WorldState::transform transform;
    transform.reference_frame = proto.reference_frame();
    transform.pose_in_observer_frame = from_proto(proto.pose_in_observer_frame());
    if (proto.has_physical_object()) {
        transform.physical_object =
            std::make_shared<GeometryConfig>(from_proto(proto.physical_object()));
    }

    return transform;
}

common::v1::Transform to_proto(const WorldState::transform& transform) {
    common::v1::Transform proto;
    *proto.mutable_reference_frame() = transform.reference_frame;
    *proto.mutable_pose_in_observer_frame() = to_proto(transform.pose_in_observer_frame);
    if (transform.physical_object) {
        *proto.mutable_physical_object() = to_proto(*transform.physical_object);
    }

    return proto;
}

WorldState from_proto(const common::v1::WorldState& ws) {
    const auto& proto_obstacles = ws.obstacles();
    std::vector<WorldState::geometries_in_frame> obstacles;
    for (const auto& po : proto_obstacles) {
        obstacles.push_back(from_proto(po));
    }

    const auto& proto_transforms = ws.transforms();
    std::vector<WorldState::transform> transforms;
    for (const auto& pt : proto_transforms) {
        transforms.push_back(from_proto(pt));
    }

    return {obstacles, transforms};
}

common::v1::WorldState to_proto(const WorldState& ws) {
    common::v1::WorldState proto_ws;
    for (const auto& obstacle : ws.obstacles()) {
        *proto_ws.mutable_obstacles()->Add() = to_proto(obstacle);
    }
    for (const auto& transform : ws.transforms()) {
        *proto_ws.mutable_transforms()->Add() = to_proto(transform);
    }

    return proto_ws;
}

viam::common::v1::Pose to_proto(const pose& pose) {
    viam::common::v1::Pose proto;
    proto.set_x(pose.coordinates.x);
    proto.set_y(pose.coordinates.y);
    proto.set_z(pose.coordinates.z);
    proto.set_o_x(pose.orientation.o_x);
    proto.set_o_y(pose.orientation.o_y);
    proto.set_o_z(pose.orientation.o_z);
    proto.set_theta(pose.theta);

    return proto;
}

pose from_proto(const viam::common::v1::Pose& proto) {
    struct pose pose;
    pose.coordinates.x = proto.x();
    pose.coordinates.y = proto.y();
    pose.coordinates.z = proto.z();
    pose.orientation.o_x = proto.o_x();
    pose.orientation.o_y = proto.o_y();
    pose.orientation.o_z = proto.o_z();
    pose.theta = proto.theta();

    return pose;
}

common::v1::PoseInFrame to_proto(const pose_in_frame& pif) {
    common::v1::PoseInFrame proto_pif;
    *proto_pif.mutable_reference_frame() = pif.reference_frame;
    common::v1::Pose proto_pose;
    proto_pose.set_x(pif.pose.coordinates.x);
    proto_pose.set_y(pif.pose.coordinates.y);
    proto_pose.set_z(pif.pose.coordinates.z);
    proto_pose.set_o_x(pif.pose.orientation.o_x);
    proto_pose.set_o_y(pif.pose.orientation.o_y);
    proto_pose.set_o_z(pif.pose.orientation.o_z);
    proto_pose.set_theta(pif.pose.theta);
    *proto_pif.mutable_pose() = std::move(proto_pose);
    return proto_pif;
};

pose_in_frame from_proto(const common::v1::PoseInFrame& proto) {
    pose_in_frame pif;
    pif.reference_frame = proto.reference_frame();
    const auto& proto_pose = proto.pose();
    pif.pose.orientation.o_x = proto_pose.o_x();
    pif.pose.orientation.o_y = proto_pose.o_y();
    pif.pose.orientation.o_z = proto_pose.o_z();
    pif.pose.coordinates.x = proto_pose.x();
    pif.pose.coordinates.y = proto_pose.y();
    pif.pose.coordinates.z = proto_pose.z();
    pif.pose.theta = proto_pose.theta();

    return pif;
}

viam::common::v1::Sphere sphere_proto(const GeometryConfig& cfg) {
    try {
        viam::common::v1::Sphere sphere;
        const auto sphere_specifics = boost::get<struct sphere>(cfg.get_geometry_specifics());
        sphere.set_radius_mm(sphere_specifics.radius);
        return sphere;
    } catch (...) {
        throw Exception(
            "Couldn't convert geometry config to sphere proto; sphere specifics not found");
    }
}

viam::common::v1::RectangularPrism box_proto(const GeometryConfig& cfg) {
    try {
        const auto box_specifics = boost::get<struct box>(cfg.get_geometry_specifics());
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

viam::common::v1::Capsule capsule_proto(const GeometryConfig& cfg) {
    try {
        const auto capsule_specifics = boost::get<struct capsule>(cfg.get_geometry_specifics());
        viam::common::v1::Capsule capsule;
        capsule.set_radius_mm(capsule_specifics.radius);
        capsule.set_length_mm(capsule_specifics.length);
        return capsule;
    } catch (...) {
        throw Exception(
            "Couldn't convert geometry config to capsule proto; capsule specifics not found");
    }
}

GeometryConfig from_proto(const viam::common::v1::Geometry& proto) {
    GeometryConfig cfg;
    const auto& pose = proto.center();
    cfg.set_pose(::viam::sdk::impl::from_proto(pose));
    cfg.set_label(proto.label());

    switch (proto.geometry_type_case()) {
        case viam::common::v1::Geometry::GeometryTypeCase::kBox: {
            cfg.set_geometry_type(GeometryType::box);
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
                cfg.set_geometry_type(GeometryType::point);
            } else {
                cfg.set_geometry_type(GeometryType::sphere);
            }
            struct sphere sphere({r});
            cfg.set_geometry_specifics(sphere);
            return cfg;
        }
        case viam::common::v1::Geometry::GeometryTypeCase::kCapsule: {
            cfg.set_geometry_type(GeometryType::capsule);
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
std::vector<GeometryConfig> from_proto(const viam::common::v1::GetGeometriesResponse& proto) {
    std::vector<GeometryConfig> response;
    for (const auto& geometry : proto.geometries()) {
        response.push_back(from_proto(geometry));
    }
    return response;
}

viam::common::v1::Geometry to_proto(const GeometryConfig& cfg) {
    viam::common::v1::Geometry geometry_;
    *geometry_.mutable_label() = cfg.get_label();
    *geometry_.mutable_center() = to_proto(cfg.get_pose());
    switch (cfg.get_geometry_type()) {
        case GeometryType::box: {
            *geometry_.mutable_box() = box_proto(cfg);
            return geometry_;
        }
        case GeometryType::sphere: {
            *geometry_.mutable_sphere() = sphere_proto(cfg);
            return geometry_;
        }
        case point: {
            viam::common::v1::Sphere sphere;
            sphere.set_radius_mm(0);
            *geometry_.mutable_sphere() = sphere;
            return geometry_;
        }
        case capsule: {
            *geometry_.mutable_capsule() = capsule_proto(cfg);
            return geometry_;
        }
        case unknown:
        default: {
            auto pose = cfg.get_pose();
            if (pose.coordinates.x == 0 && pose.coordinates.y == 0 && pose.coordinates.z == 0) {
                *geometry_.mutable_box() = box_proto(cfg);
            } else {
                *geometry_.mutable_sphere() = sphere_proto(cfg);
            }
            return geometry_;
        }
    }
}

common::v1::GeoPoint to_proto(const geo_point& gp) {
    common::v1::GeoPoint proto;
    proto.set_latitude(gp.latitude);
    proto.set_longitude(gp.longitude);

    return proto;
}

geo_point from_proto(const common::v1::GeoPoint& proto) {
    struct geo_point geo_point;
    geo_point.latitude = proto.latitude();
    geo_point.longitude = proto.longitude();

    return geo_point;
}

common::v1::GeoGeometry to_proto(const geo_geometry& gg) {
    common::v1::GeoGeometry proto;
    *proto.mutable_location() = to_proto(gg.location);

    for (const auto& geometry : gg.geometries) {
        *proto.mutable_geometries()->Add() = to_proto(geometry);
    }

    return proto;
}

geo_geometry from_proto(const common::v1::GeoGeometry& proto) {
    struct geo_geometry geo_geometry;

    geo_geometry.location = from_proto(proto.location());
    for (const auto& proto_geometry : proto.geometries()) {
        auto geometry = from_proto(proto_geometry);
        geo_geometry.geometries.push_back(std::move(geometry));
    }

    return geo_geometry;
}

OrientationConfig from_proto(const viam::app::v1::Orientation& proto) {
    namespace p = viam::app::v1;
    switch (proto.type_case()) {
        case p::Orientation::TypeCase::kAxisAngles: {
            auto type = AxisAngles;
            axis_angles aa;
            aa.x = proto.axis_angles().x();
            aa.y = proto.axis_angles().y();
            aa.z = proto.axis_angles().z();
            aa.theta = proto.axis_angles().theta();
            return {type, {}, std::move(aa)};
        }
        case p::Orientation::TypeCase::kEulerAngles: {
            auto type = EulerAngles;
            euler_angles ea;
            ea.yaw = proto.euler_angles().yaw();
            ea.pitch = proto.euler_angles().pitch();
            ea.roll = proto.euler_angles().roll();
            return {type, {}, std::move(ea)};
        }
        case p::Orientation::TypeCase::kQuaternion: {
            auto type = Quaternion;
            quaternion quat;
            quat.w = proto.quaternion().w();
            quat.x = proto.quaternion().x();
            quat.y = proto.quaternion().y();
            quat.z = proto.quaternion().z();
            return {type, {}, std::move(quat)};
        }
        case p::Orientation::TypeCase::kVectorDegrees: {
            auto type = OrientationVectorDegrees;
            orientation_vector_degrees ovd;
            ovd.x = proto.vector_degrees().x();
            ovd.y = proto.vector_degrees().y();
            ovd.z = proto.vector_degrees().z();
            ovd.theta = proto.vector_degrees().theta();
            return {type, {}, std::move(ovd)};
        }
        case p::Orientation::TypeCase::kVectorRadians: {
            auto type = OrientationVector;
            orientation_vector ov;
            ov.x = proto.vector_radians().x();
            ov.y = proto.vector_radians().y();
            ov.z = proto.vector_radians().z();
            ov.theta = proto.vector_radians().theta();
            return {type, {}, std::move(ov)};
        }
        case p::Orientation::TypeCase::kNoOrientation: {
            // if type is NoOrientation, we put a sentinel
            // orientation that indicates no rotation
            auto type = Quaternion;
            quaternion quat;
            quat.w = 1;
            quat.x = 0;
            quat.y = 0;
            quat.z = 0;
            return {type, {}, std::move(quat)};
        }
        case p::Orientation::TypeCase::TYPE_NOT_SET:
        default: {
            throw Exception(ErrorCondition::k_not_supported, "orientation type not known");
        }
    }
    return OrientationConfig{};
}

viam::app::v1::Orientation to_proto(const OrientationConfig& cfg) {
    namespace p = viam::app::v1;
    p::Orientation orientation;
    switch (cfg.get_type()) {
        case AxisAngles: {
            p::Orientation_AxisAngles aa;
            const axis_angles a = boost::get<axis_angles>(cfg.get_orientation());
            aa.set_x(a.x);
            aa.set_y(a.y);
            aa.set_z(a.z);
            aa.set_theta(a.theta);
            *orientation.mutable_axis_angles() = std::move(aa);
            return orientation;
        };
        case OrientationVector: {
            p::Orientation_OrientationVectorRadians ovec;
            const orientation_vector ov = boost::get<orientation_vector>(cfg.get_orientation());
            ovec.set_x(ov.x);
            ovec.set_y(ov.y);
            ovec.set_z(ov.z);
            ovec.set_theta(ov.theta);
            *orientation.mutable_vector_radians() = std::move(ovec);
            return orientation;
        };
        case OrientationVectorDegrees: {
            p::Orientation_OrientationVectorDegrees ovec;
            const orientation_vector_degrees ovd =
                boost::get<orientation_vector_degrees>(cfg.get_orientation());
            ovec.set_x(ovd.x);
            ovec.set_y(ovd.y);
            ovec.set_z(ovd.z);
            ovec.set_theta(ovd.theta);
            *orientation.mutable_vector_degrees() = std::move(ovec);
            return orientation;
        };
        case EulerAngles: {
            p::Orientation_EulerAngles euler;
            const euler_angles ea = boost::get<euler_angles>(cfg.get_orientation());
            euler.set_pitch(ea.pitch);
            euler.set_roll(ea.roll);
            euler.set_yaw(ea.yaw);
            *orientation.mutable_euler_angles() = std::move(euler);
            return orientation;
        };
        case Quaternion: {
            p::Orientation_Quaternion quat;
            const quaternion q = boost::get<quaternion>(cfg.get_orientation());
            quat.set_w(q.w);
            quat.set_x(q.x);
            quat.set_y(q.y);
            quat.set_z(q.z);
            *orientation.mutable_quaternion() = std::move(quat);
            return orientation;
        };
        default: {
            throw Exception(ErrorCondition::k_not_supported, "orientation type not known");
        }
    }
}

viam::app::v1::Frame to_proto(const LinkConfig& cfg) {
    viam::app::v1::Frame frame;

    *frame.mutable_parent() = cfg.get_parent();
    *frame.mutable_geometry() = to_proto(cfg.get_geometry_config());
    *frame.mutable_orientation() = to_proto(cfg.get_orientation_config());
    *frame.mutable_translation() = to_proto(cfg.get_translation());
    return frame;
};

LinkConfig from_proto(const viam::app::v1::Frame& proto) {
    translation ts;
    ts.x = proto.translation().x();
    ts.y = proto.translation().y();
    ts.z = proto.translation().z();

    OrientationConfig ocfg;
    if (proto.has_orientation()) {
        ocfg = impl::from_proto(proto.orientation());
    }

    GeometryConfig gcfg;
    if (proto.has_geometry()) {
        gcfg = from_proto(proto.geometry());
    }

    return {std::move(ts), std::move(ocfg), std::move(gcfg), proto.parent()};
};

viam::app::v1::Translation to_proto(const translation& ts) {
    viam::app::v1::Translation t;
    t.set_x(ts.x);
    t.set_y(ts.y);
    t.set_z(ts.z);
    return t;
}

viam::common::v1::ResourceName to_proto(const Name& name) {
    viam::common::v1::ResourceName rn;
    *rn.mutable_namespace_() = name.api().type_namespace();
    if (name.remote_name().empty()) {
        *rn.mutable_name() = name.name();
    } else {
        *rn.mutable_name() = name.remote_name() + ":" + name.name();
    }
    *rn.mutable_type() = name.api().resource_type();
    *rn.mutable_subtype() = name.api().resource_subtype();
    return rn;
}

Name from_proto(const viam::common::v1::ResourceName& proto) {
    const API api(proto.namespace_(), proto.type(), proto.subtype());
    auto name_parts = long_name_to_remote_and_short(proto.name());

    return Name(
        {proto.namespace_(), proto.type(), proto.subtype()}, name_parts.first, name_parts.second);
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
