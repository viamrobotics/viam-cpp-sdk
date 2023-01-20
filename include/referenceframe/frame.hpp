#ifndef FRAME_H
#define FRAME_H

#include <spatialmath/geometry.h>
#include <spatialmath/orientation.h>

#include <string>

#include "app/v1/robot.pb.h"
class LinkConfig {
       public:
	std::string id;
	std::tuple<double, double, double> translation;
	OrientationConfig orientation;
	std::string parent;

	viam::app::v1::Frame to_proto();
	static LinkConfig from_proto(viam::app::v1::Frame proto);
};

#endif

