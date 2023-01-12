#include <app/v1/robot.pb.h>
#include <common/v1/common.pb.h>
#include <spatialmath/geometry.h>
#include <spatialmath/orientation.h>

#include <string>
class LinkConfig {
	std::string id;
	std::tuple<double, double, double> translation;
	OrientationConfig orientation;
	GeometryConfig geometry;
	std::string parent;
	viam::app::v1::Frame to_proto();
};

viam::app::v1::Frame LinkConfig::to_proto() {
	// CR erodkin: fix this. geometry, orientation, translation all need to
	// be fleshed out
	viam::app::v1::Frame frame;
	viam::app::v1::Orientation orientation_;
	viam::app::v1::Translation translation_;
	// orientation_.set

	*frame.mutable_parent() = parent;
	*frame.mutable_geometry() = geometry.to_proto();
	*frame.mutable_orientation() = orientation_;
	*frame.mutable_translation() = translation_;
};

