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

bool operator==(const axis_angles& lhs, const axis_angles& rhs);
bool operator==(const euler_angles& lhs, const euler_angles& rhs);
bool operator==(const orientation_vector& lhs, const orientation_vector& rhs);
bool operator==(const orientation_vector_degrees& lhs, const orientation_vector_degrees& rhs);
bool operator==(const quaternion& lhs, const quaternion& rhs);
bool operator==(const translation& lhs, const translation& rhs);

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
