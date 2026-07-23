#pragma once

#include <viam/sdk/common/proto_convert.hpp>

namespace viam {
namespace app {
namespace v1 {

class Translation;
}
}  // namespace app
}  // namespace viam

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
    double x = 0.0, y = 0.0, z = 0.0, theta = 0.0;
};

struct euler_angles {
    double yaw = 0.0, roll = 0.0, pitch = 0.0;
};

struct orientation_vector {
    double x = 0.0, y = 0.0, z = 0.0, theta = 0.0;
};

struct orientation_vector_degrees {
    double x = 0.0, y = 0.0, z = 0.0, theta = 0.0;
};

struct quaternion {
    double x = 0.0, y = 0.0, z = 0.0, w = 0.0;
};

struct translation {
    double x = 0.0, y = 0.0, z = 0.0;
};

namespace proto_convert_details {

template <>
struct to_proto_impl<translation> {
    void operator()(const translation&, app::v1::Translation*) const;
};

template <>
struct from_proto_impl<app::v1::Translation> {
    translation operator()(const app::v1::Translation*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
