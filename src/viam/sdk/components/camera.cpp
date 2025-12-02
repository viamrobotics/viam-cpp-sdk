#include <viam/sdk/components/camera.hpp>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/util/time_util.h>

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

Camera::Camera(std::string name) : Component(std::move(name)) {}

bool operator==(const Camera::point_cloud& lhs, const Camera::point_cloud& rhs) {
    return lhs.mime_type == rhs.mime_type && lhs.pc == rhs.pc;
}

bool operator==(const Camera::raw_image& lhs, const Camera::raw_image& rhs) {
    return lhs.mime_type == rhs.mime_type && lhs.bytes == rhs.bytes &&
           lhs.source_name == rhs.source_name && lhs.annotations == rhs.annotations;
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
           lhs.distortion_parameters == rhs.distortion_parameters &&
           lhs.frame_rate == rhs.frame_rate;
}

bool operator==(const Camera::annotation& lhs, const Camera::annotation& rhs) {
    return lhs.type == rhs.type && lhs.bounding_box == rhs.bounding_box &&
           lhs.text_content == rhs.text_content;
}

bool operator==(const Camera::annotations& lhs, const Camera::annotations& rhs) {
    return lhs.annotations == rhs.annotations;
}

}  // namespace sdk
}  // namespace viam
