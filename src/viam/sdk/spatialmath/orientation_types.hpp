#pragma once

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
};

}  // namespace sdk
}  // namespace viam
