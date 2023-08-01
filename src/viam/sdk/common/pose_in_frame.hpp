#pragma once

// CR erodkin: fix
#include <common/v1/common.pb.h>

#include <viam/sdk/spatialmath/geometry.hpp>
namespace viam {
namespace sdk {

// CR erodkin: this should be a struct and probably we should move `pose` in here as well
class PoseInFrame {
   public:
    viam::common::v1::PoseInFrame to_proto();
    static PoseInFrame from_proto(const viam::common::v1::PoseInFrame& proto);

   private:
    std::string reference_frame_;
    pose pose_;
};

}  // namespace sdk
}  // namespace viam
