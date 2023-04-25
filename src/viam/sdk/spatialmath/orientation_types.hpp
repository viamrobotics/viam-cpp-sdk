#pragma once

#include <viam/api/app/v1/robot.pb.h>

namespace viam {
namespace sdk {

enum OrientationType {
    AxisAngles,
    OrientationVector,
    OrientationVectorDegrees,
    EulerAngles,
    Quaternion,
};

struct axis_angles {
    double x_, y_, z_, theta_;
};

struct euler_angles {
    double yaw_, roll_, pitch_;
};

struct orientation_vector {
    double x_, y_, z_, theta_;
};

struct orientation_vector_degrees {
    double x_, y_, z_, theta_;
};

struct quaternion {
    double x_, y_, z_, w_;
};

struct translation {
    double x_, y_, z_;
    viam::app::v1::Translation to_proto();
};
}  // namespace sdk
}  // namespace viam
