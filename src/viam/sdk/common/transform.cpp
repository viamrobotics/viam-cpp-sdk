#include <viam/sdk/common/transform.hpp>

#include <common/v1/common.pb.h>

namespace viam {
namespace sdk {

viam::common::v1::Transform transform::to_proto() const {
    common::v1::Transform t;
    *t.mutable_reference_frame() = reference_frame;
    *t.mutable_pose_in_observer_frame() = pif.to_proto();
    *t.mutable_physical_object() = object.to_proto();
    return t;
}

transform transform::from_proto(const common::v1::Transform& proto) {
    return transform(proto.reference_frame(),
                     pose_in_frame::from_proto(proto.pose_in_observer_frame()),
                     GeometryConfig::from_proto(proto.physical_object()));
}

}  // namespace sdk
}  // namespace viam