#include <app/v1/robot.pb.h>
#include <common/v1/common.pb.h>
#include <spatialmath/geometry.h>
#include <spatialmath/orientation.h>
#include <spatialmath/orientation_types.h>

#include <string>

class LinkConfig {
	std::string id;
	Viam::SDK::translation translation;
	OrientationConfig orientation;
	GeometryConfig geometry;
	std::string parent;
	viam::app::v1::Frame to_proto();
	static LinkConfig from_proto(viam::app::v1::Frame proto);
};

viam::app::v1::Frame LinkConfig::to_proto() {
	// CR erodkin: fix this. geometry, orientation, translation all need to
	// be fleshed out
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
		lc.orientation =
		    OrientationConfig::from_proto(proto.orientation());
	}

	if (proto.has_geometry()) {
		lc.geometry = GeometryConfig::from_proto(proto.geometry());
	}

	return lc;
};

