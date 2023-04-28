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
    double x, y, z, theta;
};

struct euler_angles {
    double yaw, roll, pitch;
};

struct orientation_vector {
    double x, y, z, theta;
};

struct orientation_vector_degrees {
    double x, y, z, theta;
};

struct quaternion {
    double x, y, z, w;
};

struct translation {
    double x, y, z;
    viam::app::v1::Translation to_proto() const;
};
}  // namespace sdk
}  // namespace viam
