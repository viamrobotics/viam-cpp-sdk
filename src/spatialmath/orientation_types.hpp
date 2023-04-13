#pragma once

#include <app/v1/robot.pb.h>

namespace viam {
namespace cppsdk {

enum OrientationType {
    AxisAngles,
    OrientationVector,
    OrientationVectorDegrees,
    EulerAngles,
    Quaternion,
};

class axis_angles {
   public:
    double x, y, z, theta;
};

class euler_angles {
   public:
    double yaw, roll, pitch;
};

class orientation_vector {
   public:
    double x, y, z, theta;
};

class orientation_vector_degrees {
   public:
    double x, y, z, theta;
};

class quaternion {
   public:
    double x, y, z, w;
};

class translation {
   public:
    double x, y, z;
    viam::app::v1::Translation to_proto();
};
}  // namespace cppsdk
}  // namespace viam
