#include <viam/sdk/common/world_state.hpp>

#include <common/v1/common.pb.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>

namespace viam {
namespace sdk {

WorldState::WorldState(std::vector<geometries_in_frame> obstacles,
                       std::vector<transform> transforms)
    : obstacles_(std::move(obstacles)), transforms_(std::move(transforms)) {}

const std::vector<WorldState::geometries_in_frame>& WorldState::obstacles() const {
    return obstacles_;
}

const std::vector<WorldState::transform>& WorldState::transforms() const {
    return transforms_;
}

bool operator==(const WorldState::geometries_in_frame& lhs,
                const WorldState::geometries_in_frame& rhs) {
    return lhs.reference_frame == rhs.reference_frame && lhs.geometries == rhs.geometries;
}

bool operator==(const WorldState::transform& lhs, const WorldState::transform& rhs) {
    return std::tie(lhs.reference_frame, lhs.pose_in_observer_frame, lhs.physical_object) ==
           std::tie(rhs.reference_frame, rhs.pose_in_observer_frame, rhs.physical_object);
}

bool operator==(const WorldState& lhs, const WorldState& rhs) {
    return lhs.obstacles_ == rhs.obstacles_ && lhs.transforms_ == rhs.transforms_;
}

namespace proto_convert_details {

void to_proto<WorldState::geometries_in_frame>::operator()(
    const WorldState::geometries_in_frame& self, common::v1::GeometriesInFrame* proto) const {
    *(proto->mutable_geometries()) = impl::to_repeated_field(self.geometries);
    *(proto->mutable_reference_frame()) = self.reference_frame;
}

WorldState::geometries_in_frame from_proto<common::v1::GeometriesInFrame>::operator()(
    const common::v1::GeometriesInFrame* proto) const {
    return {impl::from_repeated_field(proto->geometries()), proto->reference_frame()};
}

void to_proto<WorldState::transform>::operator()(const WorldState::transform& self,
                                                 common::v1::Transform* proto) const {
    *(proto->mutable_reference_frame()) = self.reference_frame;
    *(proto->mutable_pose_in_observer_frame()) = v2::to_proto(self.pose_in_observer_frame);
    if (self.physical_object) {
        *(proto->mutable_physical_object()) = v2::to_proto(*self.physical_object);
    }
}

WorldState::transform from_proto<common::v1::Transform>::operator()(
    const common::v1::Transform* proto) const {
    WorldState::transform result{proto->reference_frame(),
                                 v2::from_proto(proto->pose_in_observer_frame())};
    if (proto->has_physical_object()) {
        result.physical_object = v2::from_proto(proto->physical_object());
    }

    return result;
}

void to_proto<WorldState>::operator()(const WorldState& self, common::v1::WorldState* proto) const {
    *(proto->mutable_obstacles()) = impl::to_repeated_field(self.obstacles());
    *(proto->mutable_transforms()) = impl::to_repeated_field(self.transforms());
}

WorldState from_proto<common::v1::WorldState>::operator()(
    const common::v1::WorldState* proto) const {
    return WorldState(impl::from_repeated_field(proto->obstacles()),
                      impl::from_repeated_field(proto->transforms()));
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
