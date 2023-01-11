#include <spatialmath/geometry.h>
#include <spatialmath/orientation.h>

#include <string>

#include "app/v1/robot.pb.h"
#include "common/v1/common.pb.h"
class LinkConfig {
	std::string id;
	std::tuple<double, double, double> translation;
	OrientationConfig orientation;
	std::string parent;
	viam::app::v1::Frame to_proto();
};

viam::app::v1::Frame LinkConfig::to_proto() {
	// CR erodkin: fix this. geometry, orientation, translation all need to
	// be fleshed out
	viam::app::v1::Frame frame;
	viam::common::v1::Geometry geometry;
	viam::app::v1::Orientation orientation_;
	viam::app::v1::Translation translation_;

	*frame.mutable_parent() = parent;
	*frame.mutable_geometry() = geometry;
	*frame.mutable_orientation() = orientation_;
	*frame.mutable_translation() = translation_;
};

