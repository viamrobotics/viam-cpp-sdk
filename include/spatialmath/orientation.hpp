#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <app/v1/robot.pb.h>

#include <string>
#include <vector>

enum OrientationType {
	AxisAngles,
	OrientationVector,
	OrientationVectorDegrees,
	EulerAngles,
	Quaternion,
};

struct OrientationConfig {
	OrientationType type;
	std::vector<std::uint8_t> value;
	viam::app::v1::Orientation to_proto();
};

#endif
