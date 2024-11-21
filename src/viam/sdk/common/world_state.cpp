#include <viam/sdk/common/world_state.hpp>

#include <common/v1/common.pb.h>

#include <viam/sdk/common/pose.hpp>

namespace viam {
namespace sdk {

WorldState::geometries_in_frame WorldState::geometries_in_frame::from_proto(
    const common::v1::GeometriesInFrame& proto) {
    geometries_in_frame gif;
    for (const auto& geo : proto.geometries()) {
        gif.geometries.push_back(GeometryConfig::from_proto(geo));
    }
    gif.reference_frame = proto.reference_frame();

    return gif;
}

common::v1::GeometriesInFrame WorldState::geometries_in_frame::to_proto() const {
    common::v1::GeometriesInFrame proto;

    *proto.mutable_reference_frame() = reference_frame;
    for (const auto& geometry : geometries) {
        *proto.mutable_geometries()->Add() = geometry.to_proto();
    }

    return proto;
}

WorldState WorldState::from_proto(const common::v1::WorldState& ws) {
    const auto& proto_obstacles = ws.obstacles();
    std::vector<geometries_in_frame> obstacles;
    for (const auto& po : proto_obstacles) {
        obstacles.push_back(geometries_in_frame::from_proto(po));
    }

    const auto& proto_transforms = ws.transforms();
    std::vector<transform> transforms;
    for (const auto& pt : proto_transforms) {
        transforms.push_back(WorldState::transform::from_proto(pt));
    }

    return {obstacles, transforms};
}

common::v1::WorldState WorldState::to_proto() const {
    common::v1::WorldState proto_ws;
    for (const auto& obstacle : obstacles_) {
        *proto_ws.mutable_obstacles()->Add() = obstacle.to_proto();
    }
    for (const auto& transform : transforms_) {
        *proto_ws.mutable_transforms()->Add() = transform.to_proto();
    }

    return proto_ws;
}

WorldState::transform WorldState::transform::from_proto(const common::v1::Transform& proto) {
    WorldState::transform transform;
    transform.reference_frame = proto.reference_frame();
    transform.pose_in_observer_frame = v2::from_proto(proto.pose_in_observer_frame());
    if (proto.has_physical_object()) {
        transform.physical_object =
            std::make_shared<GeometryConfig>(GeometryConfig::from_proto(proto.physical_object()));
    }

    return transform;
}

common::v1::Transform WorldState::transform::to_proto() const {
    common::v1::Transform proto;
    *proto.mutable_reference_frame() = reference_frame;
    *proto.mutable_pose_in_observer_frame() = v2::to_proto(pose_in_observer_frame);
    if (physical_object) {
        *proto.mutable_physical_object() = physical_object->to_proto();
    }

    return proto;
}
bool operator==(const WorldState::geometries_in_frame& lhs,
                const WorldState::geometries_in_frame& rhs) {
    return lhs.reference_frame == rhs.reference_frame && lhs.geometries == rhs.geometries;
}
bool operator==(const WorldState::transform& lhs, const WorldState::transform& rhs) {
    return lhs.reference_frame == rhs.reference_frame &&
           (lhs.physical_object == rhs.physical_object ||
            *lhs.physical_object == *rhs.physical_object) &&
           lhs.pose_in_observer_frame == rhs.pose_in_observer_frame;
}

bool operator==(const WorldState& lhs, const WorldState& rhs) {
    return lhs.obstacles_ == rhs.obstacles_ && lhs.transforms_ == rhs.transforms_;
}

}  // namespace sdk
}  // namespace viam
