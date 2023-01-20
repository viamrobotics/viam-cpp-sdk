#include <app/v1/robot.pb.h>

class axis_angles {
	double x, y, z, theta;
};

class euler_angles {
	double yaw, roll, pitch;
};

class orientation_vector {
	double x, y, z, theta;
};

class orientation_vector_degrees {
	double x, y, z, theta;
};

class quaternion {
	double x, y, z, w;
};

class translation {
	double x, y, z;
	viam::app::v1::Translation to_proto();
};

viam::app::v1::Translation translation::to_proto() {
	viam::app::v1::Translation t;
	t.set_x(x);
	t.set_y(y);
	t.set_z(z);
	return t;
}
