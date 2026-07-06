/// @file common/private/raw_image.hpp
///
/// @brief Proto conversion helpers for Camera::raw_image and Image proto.
#pragma once

#include <viam/api/component/camera/v1/camera.pb.h>
#include <viam/sdk/components/camera.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @brief Convert a proto Image to Camera::raw_image.
Camera::raw_image from_proto(const ::viam::component::camera::v1::Image& proto);

/// @brief Convert a Camera::raw_image to proto Image.
void to_proto(const Camera::raw_image& image, ::viam::component::camera::v1::Image* out);

}  // namespace impl
}  // namespace sdk
}  // namespace viam
