#include <viam/sdk/common/private/raw_image.hpp>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {
namespace impl {

Camera::raw_image from_proto(const ::viam::component::camera::v1::Image& proto) {
    Camera::raw_image raw_image;
    std::string img_string = proto.image();
    const std::vector<unsigned char> bytes(img_string.begin(), img_string.end());
    raw_image.bytes = bytes;
    raw_image.mime_type = proto.mime_type();
    raw_image.source_name = proto.source_name();
    return raw_image;
}

void to_proto(const Camera::raw_image& image, ::viam::component::camera::v1::Image* out) {
    const std::string img_string = bytes_to_string(image.bytes);
    out->set_source_name(image.source_name);
    out->set_mime_type(image.mime_type);
    out->set_image(img_string);
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
