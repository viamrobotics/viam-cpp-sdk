#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/module/v1/module.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/common/world_state.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/handler_map.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {
namespace impl {

viam::common::v1::Vector3 to_proto(const Vector3& v3);
Vector3 from_proto(const viam::common::v1::Vector3& vec);

viam::common::v1::ResponseMetadata to_proto(const response_metadata& metadata);
response_metadata from_proto(const viam::common::v1::ResponseMetadata& proto);

google::protobuf::Duration to_proto(const std::chrono::microseconds& duration);
std::chrono::microseconds from_proto(const google::protobuf::Duration& proto);

viam::module::v1::HandlerMap to_proto(const HandlerMap_& hm);
HandlerMap_ from_proto(const viam::module::v1::HandlerMap& proto);

ResourceConfig from_proto(const viam::app::v1::ComponentConfig& proto_cfg);
viam::app::v1::ComponentConfig to_proto(const ResourceConfig& cfg);

WorldState::geometries_in_frame from_proto(const common::v1::GeometriesInFrame& proto);
common::v1::GeometriesInFrame to_proto(const WorldState::geometries_in_frame& gif);

common::v1::Transform to_proto(const WorldState::transform& transform);
WorldState::transform from_proto(const common::v1::Transform& proto);

common::v1::WorldState to_proto(const WorldState& ws);
WorldState from_proto(const common::v1::WorldState& ws);

pose from_proto(const viam::common::v1::Pose& proto);
viam::common::v1::Pose to_proto(const pose& pose);

pose_in_frame from_proto(const viam::common::v1::PoseInFrame& proto);
viam::common::v1::PoseInFrame to_proto(const pose_in_frame& pif);

GeometryConfig from_proto(const viam::common::v1::Geometry& proto);
std::vector<GeometryConfig> from_proto(const viam::common::v1::GetGeometriesResponse& proto);
viam::common::v1::Geometry to_proto(const GeometryConfig& cfg);
viam::common::v1::RectangularPrism box_proto(const GeometryConfig& cfg);
viam::common::v1::Sphere sphere_proto(const GeometryConfig& cfg);
viam::common::v1::Capsule capsule_proto(const GeometryConfig& cfg);

common::v1::GeoPoint to_proto(const geo_point& gp);
geo_point from_proto(const common::v1::GeoPoint& proto);

common::v1::GeoGeometry to_proto(const geo_geometry& gg);
geo_geometry from_proto(const common::v1::GeoGeometry& proto);

viam::app::v1::Orientation to_proto(const OrientationConfig& cfg);
OrientationConfig from_proto(const viam::app::v1::Orientation& proto);

viam::app::v1::Frame to_proto(const LinkConfig& cfg);
LinkConfig from_proto(const viam::app::v1::Frame& proto);

viam::app::v1::Translation to_proto(const translation& ts);

viam::common::v1::ResourceName to_proto(const Name& name);
Name from_proto(const viam::common::v1::ResourceName& proto);

}  // namespace impl
}  // namespace sdk
}  // namespace viam
