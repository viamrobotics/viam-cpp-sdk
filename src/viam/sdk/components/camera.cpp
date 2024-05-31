#include <viam/sdk/components/camera.hpp>

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

namespace {
// UTF-8 encoding of 'DEPTHMAP' used in the header of FORMAT_RAW_DEPTH bytes payload.
const uint64_t k_magic_number = 0x44455054484D4150ULL;

// Number of bytes of the header for FORMAT_RAW_DEPTH payloads
const auto k_header_size = sizeof(uint64_t) * 3;

// Appends an int of type T in big-endian format to a byte vector and updates the offset.
template <typename T>
void append_big_endian(std::vector<unsigned char>& data, size_t* offset, T value) {
    if (data.size() < *offset + sizeof(T)) {
        throw Exception("Incorrect data size: attempted to write beyond data bounds");
    }
    T value_be = boost::endian::native_to_big(value);
    std::memcpy(&data[*offset], &value_be, sizeof(T));
    *offset += sizeof(T);
}

// Reads an int of type T from data in big-endian format and updates the offset.
// Intended to be used in a sequential manner.
template <typename T>
T read_big_endian(const std::vector<unsigned char>& data, size_t* offset) {
    if (data.size() < *offset + sizeof(T)) {
        throw std::runtime_error("Attempted to read beyond data bounds.");
    }

    T value;
    std::memcpy(&value, &data[*offset], sizeof(T));
    value = boost::endian::big_to_native(value);
    *offset += sizeof(T);

    return value;
}

}  // namespace

std::vector<unsigned char> Camera::encode_depth_map(const Camera::depth_map& m) {
    const auto& shape = m.shape();
    if (shape.size() != 2) {
        throw Exception("Depth map is not a 2D array.");
    }

    const size_t height = shape[0];
    const size_t width = shape[1];
    const size_t total_byte_count = k_header_size + height * width * sizeof(uint16_t);
    std::vector<unsigned char> data(total_byte_count);
    size_t offset = 0;

    // Network data is stored in big-endian, while most host systems are little endian.
    append_big_endian(data, &offset, k_magic_number);
    append_big_endian(data, &offset, width);
    append_big_endian(data, &offset, height);

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            append_big_endian(data, &offset, m(i, j));
        }
    }

    return data;
}

Camera::depth_map Camera::decode_depth_map(const std::vector<unsigned char>& data) {
    if (data.size() < k_header_size) {
        throw Exception("Data too short to contain valid depth information. Size: " +
                        std::to_string(data.size()));
    }

    size_t offset = 0;
    const uint64_t magic_number = read_big_endian<uint64_t>(data, &offset);
    if (magic_number != k_magic_number) {
        throw Exception(
            "Invalid header for a vnd.viam.dep encoded depth image. The data may be corrupted, or "
            "is not a Viam-encoded depth map.");
    }

    const uint64_t width = read_big_endian<uint64_t>(data, &offset);
    const uint64_t height = read_big_endian<uint64_t>(data, &offset);

    const auto expected_size = k_header_size + width * height * sizeof(uint16_t);
    if (data.size() != expected_size) {
        throw Exception("Data size does not match width, height, and depth values. Actual size: " +
                        std::to_string(data.size()) +
                        ". Expected size: " + std::to_string(expected_size) +
                        ". Width: " + std::to_string(width) + " Height: " + std::to_string(height));
    }

    std::vector<uint16_t> depth_values;
    depth_values.reserve(width * height);
    for (size_t i = 0; i < width * height; ++i) {
        depth_values.push_back(read_big_endian<uint16_t>(data, &offset));
    }

    xt::xarray<uint16_t> m = xt::xarray<uint16_t>::from_shape({height, width});
    std::copy(depth_values.begin(), depth_values.end(), m.begin());
    return m;
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
    Camera::mime_types mime_types;
    Camera::properties properties;

    const viam::component::camera::v1::DistortionParameters& distortion_parameters_proto =
        proto.distortion_parameters();
    distortion_parameters = from_proto(distortion_parameters_proto);

    const viam::component::camera::v1::IntrinsicParameters& intrinsic_parameters_proto =
        proto.intrinsic_parameters();
    intrinsic_parameters = from_proto(intrinsic_parameters_proto);

    mime_types.assign(proto.mime_types().begin(), proto.mime_types().end());

    properties.distortion_parameters = distortion_parameters;
    properties.intrinsic_parameters = intrinsic_parameters;
    properties.mime_types = mime_types;
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
