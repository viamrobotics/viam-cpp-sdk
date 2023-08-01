#include "common/v1/common.pb.h"
#include "viam/sdk/common/pose_in_frame.hpp"
#include <viam/sdk/common/world_state.hpp>

namespace viam {
namespace sdk {

WorldState WorldState::from_proto(const common::v1::WorldState& ws) {
    const auto& proto_obstacles = ws.obstacles();
    std::vector<geometries_in_frame> obstacles;
    for (const auto& po : proto_obstacles) {
        geometries_in_frame obstacle;
        for (const auto& geo : po.geometries()) {
            obstacle.geometries.push_back(GeometryConfig::from_proto(geo));
        }
        obstacles.push_back(obstacle);
    }

    const auto& proto_transforms = ws.transforms();
    std::vector<transform> transforms;
    for (const auto& pt : proto_transforms) {
        transform t = WorldState::transform::from_proto(pt);
        transforms.push_back(std::move(t));
    }

    return {obstacles, transforms};
}

WorldState::transform WorldState::transform::from_proto(const common::v1::Transform& proto) {
    WorldState::transform transform;
    transform.reference_frame = proto.reference_frame();
    transform.pose_in_observer_frame = PoseInFrame::from_proto(proto.pose_in_observer_frame());
    if (proto.has_physical_object()) {
        transform.physical_object =
            std::make_shared<GeometryConfig>(GeometryConfig::from_proto(proto.physical_object()));
    }

    return transform;
}

common::v1::Transform WorldState::transform::to_proto() const {
    common::v1::Transform proto;
    *proto.mutable_reference_frame() = reference_frame;
    *proto.mutable_pose_in_observer_frame() = pose_in_observer_frame.to_proto();
    if (physical_object) {
        *proto.mutable_physical_object() = physical_object->to_proto();
    }

    return proto;
}

}  // namespace sdk
}  // namespace viam
