#include <viam/sdk/services/framesystem.hpp>

#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

// NOLINTNEXTLINE(cert-err58-cpp)
const std::string FrameSystem::kPublicName = "$framesystem";

Name FrameSystem::public_name() {
    return {API::get<FrameSystem>(), "", kPublicName};
}

FrameSystem::FrameSystem(std::string name) : Service(std::move(name)) {}

FrameSystem::FrameSystem(const ResourceConfig& cfg) : Service(cfg) {}

API FrameSystem::api() const {
    return API::get<FrameSystem>();
}

API API::traits<FrameSystem>::api() {
    return {kRDKInternal, kService, "frame_system"};
}

bool operator==(const FrameSystem::frame_system_config& lhs,
                const FrameSystem::frame_system_config& rhs) {
    return lhs.frame == rhs.frame && lhs.model == rhs.model &&
           to_proto(lhs.kinematics).SerializeAsString() ==
               to_proto(rhs.kinematics).SerializeAsString();
}

namespace proto_convert_details {

FrameSystem::frame_system_config from_proto_impl<robot::v1::FrameSystemConfig>::operator()(
    const robot::v1::FrameSystemConfig* proto) const {
    FrameSystem::frame_system_config fsconfig;
    fsconfig.frame = from_proto(proto->frame());
    // The proto field is deprecated but still what older viam-servers publish.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    if (proto->has_kinematics()) {
        fsconfig.kinematics = from_proto(proto->kinematics());
    }
#pragma GCC diagnostic pop
    if (proto->has_model()) {
        fsconfig.model = from_proto(proto->model());
    }
    return fsconfig;
}

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
