#include <viam/sdk/referenceframe/frame.hpp>

#include <string>

#include <viam/api/app/v1/robot.pb.h>
#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {

viam::app::v1::Frame LinkConfig::to_proto() const {
    viam::app::v1::Frame frame;

    *frame.mutable_parent() = parent_;
    *frame.mutable_geometry() = geometry_.to_proto();
    *frame.mutable_orientation() = v2::to_proto(orientation_);
    *frame.mutable_translation() = v2::to_proto(translation_);
    return frame;
};

LinkConfig LinkConfig::from_proto(const viam::app::v1::Frame& proto) {
    LinkConfig lc;

    lc.parent_ = proto.parent();
    lc.translation_.x = proto.translation().x();
    lc.translation_.y = proto.translation().y();
    lc.translation_.z = proto.translation().z();

    if (proto.has_orientation()) {
        lc.orientation_ = v2::from_proto(proto.orientation());
    }

    if (proto.has_geometry()) {
        lc.geometry_ = GeometryConfig::from_proto(proto.geometry());
    }

    return lc;
};

translation LinkConfig::get_translation() const {
    return translation_;
}

const Orientation& LinkConfig::get_orientation() const {
    return orientation_;
}

GeometryConfig LinkConfig::get_geometry_config() const {
    return geometry_;
}

std::string LinkConfig::get_parent() const {
    return parent_;
}

}  // namespace sdk
}  // namespace viam
