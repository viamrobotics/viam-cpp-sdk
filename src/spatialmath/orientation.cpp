#include <string>
#include <vector>

#include "app/v1/robot.pb.h"

enum OrientationType {
	AxisAngles,
	OrientationVector,
	OrientationVectorDegrees,
	EulerAngles,
	Quaternion,
};

struct OrientationConfig {
	OrientationType type;
	// CR erodkin: confirm with Gautham that we can get away with just
	// setting x/y/z/theta in the config. should theta always be 1, like in
	// geometry?
	//
	// update: don't think we can do this. see EulerAngler, yaw/pitch/roll.
	// doesn't really map cleanly to x/y/z probably?
	double x;
	double y;
	double z;
	double theta;
	std::vector<std::uint8_t> value;
	viam::app::v1::Orientation to_proto();
};

viam::app::v1::Orientation OrientationConfig::to_proto() {
	viam::app::v1::Orientation orientation;
	switch (type) {
		case AxisAngles: {
			::viam::app::v1::Orientation_AxisAngles aa;
			aa.set_x(x);
			aa.set_y(y);
			aa.set_z(z);
			aa.set_theta(theta);
			orientation.set_allocated_axis_angles(&aa);
			return orientation;
		};
		case OrientationVector: {
			::viam::app::v1::Orientation_OrientationVectorRadians
			    ovec;
			ovec.set_x(x);
			ovec.set_y(y);
			ovec.set_z(z);
			ovec.set_theta(theta);
			orientation.set_allocated_vector_radians(&ovec);
			return orientation;
		};
		case OrientationVectorDegrees: {
			::viam::app::v1::Orientation_OrientationVectorDegrees
			    ovec;
			ovec.set_x(x);
			ovec.set_y(y);
			ovec.set_z(z);
			ovec.set_theta(theta);
			orientation.set_allocated_vector_degrees(&ovec);
			return orientation;
		};
		case EulerAngles: {
			::viam::app::v1::Orientation_EulerAngles euler;
			euler.set_yaw(x);
			orientation.set_allocated_euler_angles(&euler);
		};
		case Quaternion: {
			::viam::app::v1::Orientation_Quaternion quat;
			quat.set_x(x);
			quat.set_y(y);
			quat.set_z(z);
		}
	}
}
