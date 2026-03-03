#pragma once

namespace viam {
namespace sdk {

/**
 * \defgroup MimeTypes MIME type constants.
 * Well-known MIME type constants. A subset of
 * https://github.com/viamrobotics/rdk/blob/main/utils/mime.go
 * \remark For now this just includes
 * image types that can be returned from @ref Camera components, but other constants should be added
 * here if needed.
 * @{
 */

constexpr const char* kMimeTypeJPEG = "image/jpeg";
constexpr const char* kMimeTypePNG = "image/png";
constexpr const char* kMimeTypeViamDep = "image/vnd.viam.dep";
constexpr const char* kMimeTypePCD = "pointcloud/pcd";

/**@} */

}  // namespace sdk
}  // namespace viam
