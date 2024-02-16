#include <viam/sdk/components/camera/camera.hpp>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/util/time_util.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>
#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

// NOLINTNEXTLINE
const std::string Camera::lazy_suffix = "+lazy";

API Camera::api() const {
    return API::get<Camera>();
}

API API::traits<Camera>::api() {
    return {kRDK, kComponent, "camera"};
}

// Utility functions for byte swapping
uint16_t swap_bytes_16(uint16_t value) {
    return (value >> 8) | (value << 8);
}

uint64_t swap_bytes_64(uint64_t value) {
    return ((value >> 56) & 0xff) | ((value << 40) & 0xff000000000000) |
           ((value << 24) & 0xff0000000000) | ((value << 8) & 0xff00000000) |
           ((value >> 8) & 0xff000000) | ((value >> 24) & 0xff0000) | ((value >> 40) & 0xff00) |
           ((value << 56) & 0xff00000000000000);
}

bool is_little_endian() {
    unsigned int x = 1;
    char* c = (char*)&x;
    return *c;
}

std::vector<unsigned char> Camera::encode_depth_map(const Camera::depth_map& m) {
    if (m.depth_values.size() != m.width * m.height) {
        throw Exception(
            "Number of depth values does not match the specified width and height. Expected: " +
            std::to_string(m.width * m.height) +
            ". Actual: " + std::to_string(m.depth_values.size()));
    }

    std::vector<unsigned char> data;
    data.reserve(16 + m.width * m.height * sizeof(uint16_t));

    // Convert width and height to big-endian if necessary
    const uint64_t width_be = is_little_endian() ? swap_bytes_64(m.width) : m.width;
    const uint64_t height_be = is_little_endian() ? swap_bytes_64(m.height) : m.height;

    for (int i = 7; i >= 0; --i) {
        data.push_back((width_be >> (i * 8)) & 0xFF);
    }
    for (int i = 7; i >= 0; --i) {
        data.push_back((height_be >> (i * 8)) & 0xFF);
    }

    for (auto value : m.depth_values) {
        const uint16_t value_be = is_little_endian() ? swap_bytes_16(value) : value;
        data.push_back((value_be >> 8) & 0xFF);
        data.push_back(value_be & 0xFF);
    }

    return data;
}

Camera::depth_map Camera::decode_depth_map(const std::vector<unsigned char>& data) {
    Camera::depth_map depth_map;
    if (data.size() < 24) {
        throw Exception("Data too short to contain valid depth information. Size: " +
                        std::to_string(data.size()));
    }

    // Assuming data is in big-endian format
    uint64_t width = 0;
    uint64_t height = 0;

    // Process header
    for (int i = 0; i < 8; ++i) {
        width = (width << 8) | data[8 + i];
        height = (height << 8) | data[16 + i];
    }

    auto expected_size = width * height * sizeof(uint16_t);
    if (data.size() < expected_size) {
        throw Exception("Data size does not match width and height. Actual size: " +
                        std::to_string(data.size()) +
                        ". Expected size: " + std::to_string(expected_size) + ".");
    }

    // Remaining bytes are actual depth map data
    std::vector<uint16_t> arr;
    arr.reserve(width * height);

    for (size_t i = 0; i < width * height; ++i) {
        const size_t data_index = 24 + i * sizeof(uint16_t);
        const uint16_t depth_value = static_cast<uint16_t>(
            data[data_index] << 8 | data[data_index + 1]);  // Assemble from big endian into uint16
        arr.push_back(depth_value);
    }

    depth_map.width = width;
    depth_map.height = height;
    depth_map.depth_values = std::move(arr);
    return depth_map;
}

std::string Camera::normalize_mime_type(const std::string& str) {
    std::string mime_type = str;
    if (str.size() >= Camera::lazy_suffix.size() &&
        str.compare(str.size() - Camera::lazy_suffix.size(),
                    Camera::lazy_suffix.size(),
                    Camera::lazy_suffix) == 0) {
        mime_type = mime_type.substr(0, mime_type.length() - Camera::lazy_suffix.length());
    }
    return mime_type;
}

std::string Camera::format_to_MIME_string(viam::component::camera::v1::Format format) {
    switch (format) {
        case viam::component::camera::v1::FORMAT_RAW_RGBA:
            return "image/vnd.viam.rgba";
        case viam::component::camera::v1::FORMAT_RAW_DEPTH:
            return "image/vnd.viam.dep";
        case viam::component::camera::v1::FORMAT_JPEG:
            return "image/jpeg";
        case viam::component::camera::v1::FORMAT_PNG:
            return "image/png";
        default:
            return "";
    }
}

::viam::component::camera::v1::Format Camera::MIME_string_to_format(
    const std::string& mime_string) {
    if (mime_string == "image/vnd.viam.rgba") {
        return viam::component::camera::v1::FORMAT_RAW_RGBA;
    }
    if (mime_string == "image/vnd.viam.dep") {
        return viam::component::camera::v1::FORMAT_RAW_DEPTH;
    }
    if (mime_string == "image/jpeg") {
        return viam::component::camera::v1::FORMAT_JPEG;
    }
    if (mime_string == "image/png") {
        return viam::component::camera::v1::FORMAT_PNG;
    }
    return viam::component::camera::v1::FORMAT_UNSPECIFIED;
}

Camera::raw_image Camera::from_proto(const viam::component::camera::v1::GetImageResponse& proto) {
    Camera::raw_image raw_image;
    std::string img_string = proto.image();
    const std::vector<unsigned char> bytes(img_string.begin(), img_string.end());
    raw_image.bytes = bytes;
    raw_image.mime_type = proto.mime_type();
    raw_image.source_name = "";
    return raw_image;
}

Camera::image_collection Camera::from_proto(
    const viam::component::camera::v1::GetImagesResponse& proto) {
    Camera::image_collection image_collection;
    std::vector<Camera::raw_image> images;
    for (const auto& img : proto.images()) {
        Camera::raw_image raw_image;
        std::string img_string = img.image();
        const std::vector<unsigned char> bytes(img_string.begin(), img_string.end());
        raw_image.bytes = bytes;
        raw_image.mime_type = format_to_MIME_string(img.format());
        raw_image.source_name = img.source_name();
        images.push_back(raw_image);
    }
    image_collection.images = std::move(images);
    image_collection.metadata = response_metadata::from_proto(proto.response_metadata());
    return image_collection;
}

Camera::point_cloud Camera::from_proto(
    const viam::component::camera::v1::GetPointCloudResponse& proto) {
    Camera::point_cloud point_cloud;
    std::string pc_string = proto.point_cloud();
    const std::vector<unsigned char> bytes(pc_string.begin(), pc_string.end());
    point_cloud.pc = bytes;
    point_cloud.mime_type = proto.mime_type();
    return point_cloud;
}

Camera::intrinsic_parameters Camera::from_proto(
    const viam::component::camera::v1::IntrinsicParameters& proto) {
    Camera::intrinsic_parameters params;
    // NOLINTNEXTLINE(bugprone-narrowing-conversions)
    params.width_px = proto.width_px();
    // NOLINTNEXTLINE(bugprone-narrowing-conversions)
    params.height_px = proto.height_px();
    params.focal_x_px = proto.focal_x_px();
    params.focal_y_px = proto.focal_y_px();
    params.center_x_px = proto.center_x_px();
    params.center_y_px = proto.center_y_px();
    return params;
}

Camera::distortion_parameters Camera::from_proto(
    const viam::component::camera::v1::DistortionParameters& proto) {
    Camera::distortion_parameters params;
    params.model = proto.model();
    params.parameters = {proto.parameters().begin(), proto.parameters().end()};
    return params;
}

Camera::properties Camera::from_proto(
    const viam::component::camera::v1::GetPropertiesResponse& proto) {
    Camera::distortion_parameters distortion_parameters;
    Camera::intrinsic_parameters intrinsic_parameters;
    Camera::properties properties;

    const viam::component::camera::v1::DistortionParameters& distortion_parameters_proto =
        proto.distortion_parameters();
    distortion_parameters = from_proto(distortion_parameters_proto);

    const viam::component::camera::v1::IntrinsicParameters& intrinsic_parameters_proto =
        proto.intrinsic_parameters();
    intrinsic_parameters = from_proto(intrinsic_parameters_proto);

    properties.distortion_parameters = distortion_parameters;
    properties.intrinsic_parameters = intrinsic_parameters;
    properties.supports_pcd = proto.supports_pcd();

    return properties;
}

viam::component::camera::v1::IntrinsicParameters Camera::to_proto(
    const Camera::intrinsic_parameters& params) {
    viam::component::camera::v1::IntrinsicParameters proto;
    proto.set_width_px(params.width_px);
    proto.set_height_px(params.height_px);
    proto.set_focal_x_px(params.focal_x_px);
    proto.set_focal_y_px(params.focal_y_px);
    proto.set_center_x_px(params.center_x_px);
    proto.set_center_y_px(params.center_y_px);
    return proto;
}
viam::component::camera::v1::DistortionParameters Camera::to_proto(
    const Camera::distortion_parameters& params) {
    viam::component::camera::v1::DistortionParameters proto;
    *proto.mutable_model() = params.model;
    *proto.mutable_parameters() = {params.parameters.begin(), params.parameters.end()};
    return proto;
}

Camera::Camera(std::string name) : Component(std::move(name)){};

bool operator==(const Camera::point_cloud& lhs, const Camera::point_cloud& rhs) {
    return lhs.mime_type == rhs.mime_type && lhs.pc == rhs.pc;
}

bool operator==(const Camera::raw_image& lhs, const Camera::raw_image& rhs) {
    return lhs.mime_type == rhs.mime_type && lhs.bytes == rhs.bytes &&
           lhs.source_name == rhs.source_name;
}

bool operator==(const Camera::image_collection& lhs, const Camera::image_collection& rhs) {
    return lhs.images == rhs.images && lhs.metadata == rhs.metadata;
}

bool operator==(const Camera::intrinsic_parameters& lhs, const Camera::intrinsic_parameters& rhs) {
    return lhs.width_px == rhs.width_px && lhs.height_px == rhs.height_px &&
           lhs.focal_x_px == rhs.focal_x_px && lhs.focal_y_px == rhs.focal_y_px &&
           lhs.center_x_px == rhs.center_x_px && lhs.center_y_px == rhs.center_y_px;
}
bool operator==(const Camera::distortion_parameters& lhs,
                const Camera::distortion_parameters& rhs) {
    return lhs.model == rhs.model && lhs.parameters == rhs.parameters;
}

bool operator==(const Camera::properties& lhs, const Camera::properties& rhs) {
    return lhs.supports_pcd == rhs.supports_pcd &&
           lhs.intrinsic_parameters == rhs.intrinsic_parameters &&
           lhs.distortion_parameters == rhs.distortion_parameters;
}

}  // namespace sdk
}  // namespace viam
