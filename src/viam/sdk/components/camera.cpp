#include <viam/sdk/components/camera.hpp>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/util/time_util.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/mime_types.hpp>
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

// Writes a depth value in big-endian order. Unlike append_big_endian it does no bounds check,
// so callers must have sized the buffer.
void write_big_endian_u16(unsigned char* out, uint16_t value) {
    out[0] = static_cast<unsigned char>(value >> 8);
    out[1] = static_cast<unsigned char>(value & 0xFF);
}

// Reads a big-endian depth value. Callers must have checked the buffer holds it.
uint16_t read_big_endian_u16(const unsigned char* in) {
    return static_cast<uint16_t>((in[0] << 8) | in[1]);
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

    // A row-major map is written in one pass over its storage, which the compiler can
    // vectorize; other layouts go through the map's own indexing.
    unsigned char* out = data.data() + offset;
    if (m.layout() == xt::layout_type::row_major) {
        const uint16_t* in = m.data();
        for (size_t i = 0; i < height * width; ++i) {
            write_big_endian_u16(out + 2 * i, in[i]);
        }
    } else {
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                write_big_endian_u16(out + 2 * (i * width + j), m(i, j));
            }
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

    // The size check above guarantees every value is in bounds, so they are read in one pass
    // straight into the map's storage.
    const unsigned char* in = data.data() + offset;
    xt::xarray<uint16_t> m = xt::xarray<uint16_t>::from_shape({height, width});
    if (m.layout() == xt::layout_type::row_major) {
        uint16_t* out = m.data();
        for (size_t i = 0; i < height * width; ++i) {
            out[i] = read_big_endian_u16(in + 2 * i);
        }
    } else {
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                m(i, j) = read_big_endian_u16(in + 2 * (i * width + j));
            }
        }
    }
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

bool operator==(const Camera::extrinsic_parameters& lhs, const Camera::extrinsic_parameters& rhs) {
    return std::tie(lhs.translation.data,
                    lhs.orientation.x,
                    lhs.orientation.y,
                    lhs.orientation.z,
                    lhs.orientation.theta) == std::tie(rhs.translation.data,
                                                       rhs.orientation.x,
                                                       rhs.orientation.y,
                                                       rhs.orientation.z,
                                                       rhs.orientation.theta);
}

bool operator==(const Camera::distortion_parameters& lhs,
                const Camera::distortion_parameters& rhs) {
    return lhs.model == rhs.model && lhs.parameters == rhs.parameters;
}

bool operator==(const Camera::properties& lhs, const Camera::properties& rhs) {
    return lhs.supports_pcd == rhs.supports_pcd &&
           lhs.intrinsic_parameters == rhs.intrinsic_parameters &&
           lhs.distortion_parameters == rhs.distortion_parameters &&
           lhs.extrinsic_parameters == rhs.extrinsic_parameters &&
           lhs.frame_rate == rhs.frame_rate && lhs.mime_types == rhs.mime_types &&
           lhs.default_reference_frame == rhs.default_reference_frame;
}

}  // namespace sdk
}  // namespace viam
