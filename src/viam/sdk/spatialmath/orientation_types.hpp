#pragma once

#include <viam/sdk/common/proto_convert.hpp>

VIAM_SDK_API_FWD_NAMESPACE_BEGIN(app)

class Translation;

VIAM_SDK_API_FWD_NAMESPACE_END

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

namespace proto_convert_details {

template <>
struct to_proto<translation> {
    void operator()(const translation&, app::v1::Translation*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
