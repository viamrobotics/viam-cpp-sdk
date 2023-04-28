#include <viam/sdk/referenceframe/frame.hpp>

#include <string>

#include <viam/api/app/v1/robot.pb.h>
#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {

const viam::app::v1::Frame LinkConfig::to_proto() const {
    viam::app::v1::Frame frame;

    *frame.mutable_parent() = parent_;
    *frame.mutable_geometry() = geometry_.to_proto();
    *frame.mutable_orientation() = orientation_.to_proto();
    *frame.mutable_translation() = translation_.to_proto();
    return frame;
};

const LinkConfig LinkConfig::from_proto(viam::app::v1::Frame proto) {
    LinkConfig lc;

    lc.parent_ = proto.parent();
    lc.translation_.x = proto.translation().x();
    lc.translation_.y = proto.translation().y();
    lc.translation_.z = proto.translation().z();

    if (proto.has_orientation()) {
        lc.orientation_ = OrientationConfig::from_proto(proto.orientation());
    }

    if (proto.has_geometry()) {
        lc.geometry_ = GeometryConfig::from_proto(proto.geometry());
    }

    return lc;
};

}  // namespace sdk
}  // namespace viam
