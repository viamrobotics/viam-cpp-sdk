#include "service/motion/v1/motion.pb.h"
#include <viam/sdk/services/motion/motion.hpp>

#include <common/v1/common.pb.h>

namespace viam {
namespace sdk {

PoseInFrame Motion::get_pose(const Name& name) {
    viam::common::v1::PoseInFrame pif;
    service::motion::v1::GetPoseRequest req;
    auto foo = req.mutable_supplemental_transforms();
    pif.mutable_pose();
    pif.mutable
}

}  // namespace sdk
}  // namespace viam
