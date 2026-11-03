#pragma once

#include <tuple>

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

    friend bool operator==(const axis_angles& lhs, const axis_angles& rhs) {
        return std::tie(lhs.x, lhs.y, lhs.z, lhs.theta) == std::tie(rhs.x, rhs.y, rhs.z, rhs.theta);
    }
};

struct euler_angles {
    double yaw = 0.0, roll = 0.0, pitch = 0.0;

    friend bool operator==(const euler_angles& lhs, const euler_angles& rhs) {
        return std::tie(lhs.yaw, lhs.roll, lhs.pitch) == std::tie(rhs.yaw, rhs.roll, rhs.pitch);
    }
};

struct orientation_vector {
    double x = 0.0, y = 0.0, z = 0.0, theta = 0.0;

    friend bool operator==(const orientation_vector& lhs, const orientation_vector& rhs) {
        return std::tie(lhs.x, lhs.y, lhs.z, lhs.theta) == std::tie(rhs.x, rhs.y, rhs.z, rhs.theta);
    }
};

struct orientation_vector_degrees {
    double x = 0.0, y = 0.0, z = 0.0, theta = 0.0;

    friend bool operator==(const orientation_vector_degrees& lhs,
                           const orientation_vector_degrees& rhs) {
        return std::tie(lhs.x, lhs.y, lhs.z, lhs.theta) == std::tie(rhs.x, rhs.y, rhs.z, rhs.theta);
    }
};

struct quaternion {
    double x = 0.0, y = 0.0, z = 0.0, w = 0.0;

    friend bool operator==(const quaternion& lhs, const quaternion& rhs) {
        return std::tie(lhs.x, lhs.y, lhs.z, lhs.w) == std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
    }
};

struct translation {
    double x = 0.0, y = 0.0, z = 0.0;

    friend bool operator==(const translation& lhs, const translation& rhs) {
        return std::tie(lhs.x, lhs.y, lhs.z) == std::tie(rhs.x, rhs.y, rhs.z);
    }
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
