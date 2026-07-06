#include <viam/sdk/components/arm.hpp>

#include <utility>
#include <vector>

#include <viam/api/component/arm/v1/arm.pb.h>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

API Arm::api() const {
    return API::get<Arm>();
}

API API::traits<Arm>::api() {
    return {kRDK, kComponent, "arm"};
}

Arm::Arm(std::string name) : Component(std::move(name)) {}

namespace proto_convert_details {

void to_proto_impl<Arm::trajectory_point>::operator()(
    const Arm::trajectory_point& self, viam::component::arm::v1::TrajectoryPoint* proto) const {
    *proto->mutable_time() = to_proto(self.time);
    *proto->mutable_positions()->mutable_values() = {self.positions.begin(), self.positions.end()};
    if (self.constraints) {
        auto* pb_kc = proto->mutable_constraints();
        *pb_kc->mutable_velocities()->mutable_values() = {self.constraints->velocities.begin(),
                                                          self.constraints->velocities.end()};
        if (self.constraints->accelerations) {
            *pb_kc->mutable_accelerations()->mutable_values() = {
                self.constraints->accelerations->begin(), self.constraints->accelerations->end()};
        }
    }
}

Arm::trajectory_point from_proto_impl<viam::component::arm::v1::TrajectoryPoint>::operator()(
    const viam::component::arm::v1::TrajectoryPoint* proto) const {
    Arm::trajectory_point result;
    result.time = from_proto(proto->time());
    result.positions.assign(proto->positions().values().begin(), proto->positions().values().end());
    if (proto->has_constraints()) {
        Arm::trajectory_point::kinematic_constraints kc;
        const auto& pb_kc = proto->constraints();
        kc.velocities.assign(pb_kc.velocities().values().begin(),
                             pb_kc.velocities().values().end());
        if (pb_kc.has_accelerations()) {
            std::vector<double> accs(pb_kc.accelerations().values().begin(),
                                     pb_kc.accelerations().values().end());
            kc.accelerations = std::move(accs);
        }
        result.constraints = std::move(kc);
    }
    return result;
}

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
