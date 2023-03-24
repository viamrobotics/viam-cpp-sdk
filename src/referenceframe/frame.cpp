#include <app/v1/robot.pb.h>
#include <common/v1/common.pb.h>

#include <referenceframe/frame.hpp>
#include <spatialmath/geometry.hpp>
#include <spatialmath/orientation.hpp>
#include <spatialmath/orientation_types.hpp>
#include <string>

viam::app::v1::Frame LinkConfig::to_proto() {
  viam::app::v1::Frame frame;

  *frame.mutable_parent() = parent;
  *frame.mutable_geometry() = geometry.to_proto();
  *frame.mutable_orientation() = orientation.to_proto();
  *frame.mutable_translation() = translation.to_proto();
  return frame;
};

LinkConfig LinkConfig::from_proto(viam::app::v1::Frame proto) {
  LinkConfig lc;

  lc.parent = proto.parent();
  lc.translation.x = proto.translation().x();
  lc.translation.y = proto.translation().y();
  lc.translation.z = proto.translation().z();

  if (proto.has_orientation()) {
    lc.orientation = OrientationConfig::from_proto(proto.orientation());
  }

  if (proto.has_geometry()) {
    lc.geometry = GeometryConfig::from_proto(proto.geometry());
  }

  return lc;
};
