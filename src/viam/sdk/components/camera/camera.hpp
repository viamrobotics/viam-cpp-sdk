/// @file components/camera/camera.hpp
///
/// @brief Defines a `Camera` component.
#pragma once

#include <bitset>
#include <chrono>
#include <string>
#include <vector>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

/// @class Camera camera.hpp "components/camera/camera.hpp"
/// @brief A `Camera` represents any physical hardware that can capture frames.
/// @ingroup Camera
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific camera implementations. This class cannot be used on its own.
class Camera : public Component {
   public:
    /// @struct intrinsic parameters
    /// @brief The properties of the camera.
    struct intrinsic_parameters {
        int width_px;
        int height_px;
        double focal_x_px;
        double focal_y_px;
        double center_x_px;
        double center_y_px;
    };

    /// @struct distortion_parameters
    /// @brief The distortion parameters of the camera.
    struct distortion_parameters {
        std::string model;
        std::vector<double> parameters;
    };

    /// @struct properties
    /// @brief The camera's supported features and settings.
    ///
    /// `supports_pcd` indicates whether the camera has a valid implementation of `get_point_cloud`.
    /// `intrinsic_parameters` contains the camera's intrinsic parameters.
    /// `distortion_parameters` contains the camera's distortion parameters.
    struct properties {
        bool supports_pcd;
        struct intrinsic_parameters intrinsic_parameters;
        struct distortion_parameters distortion_parameters;
    };

    /// @struct point_cloud
    /// @brief The points and mime type of a point cloud.
    // TODO: update documentation to show how to deserialize a `point_cloud`
    struct point_cloud {
        std::string mime_type;
        std::vector<unsigned char> pc;
    };

    const static std::string lazy_suffix;

    /// @struct raw_image
    /// @brief the raw bytes, mime type of an image, and name of the source that produced it.
    // TODO: update documentaiton to show how to deserialize a `raw_image`
    struct raw_image {
        std::string mime_type;
        std::vector<unsigned char> bytes;
        std::string source_name;
    };

    /// @struct image_collection
    /// @brief a collection of images that were collected from a camera all at the same time.
    struct image_collection {
        std::vector<raw_image> images;
        response_metadata metadata;
    };

    /// @struct depth_map
    /// @brief Represents the dimensions and depth values of a depth map.
    ///
    /// depth_map holds the width and height of a depth map, along with a vector
    /// of actual depth values. Each depth value is a 16-bit unsigned integer representing
    /// the distance from the camera to a point in the scene.
    struct depth_map {
        uint64_t width;                      ///< Width of the depth map in pixels.
        uint64_t height;                     ///< Height of the depth map in pixels.
        std::vector<uint16_t> depth_values;  ///< A flat vector of depth values.

        /// Default constructor
        depth_map() = default;

        /// Parameterized constructor for initializing depth_map with dimensions and depth values.
        /// @param w Width of the depth map.
        /// @param h Height of the depth map.
        /// @param values A vector of depth values corresponding to each pixel in the depth map.
        depth_map(uint64_t w, uint64_t h, std::vector<uint16_t> values)
            : width(w), height(h), depth_values(std::move(values)) {}
    };

    /// @brief Throws Exception if little-endian, as depth encode/decode funcs assume big-endian
    static void check_system_endianness();

    ///
    /// Encodes the dimensions and depth values of a depth map into a raw binary format
    /// (MIME type FORMAT_RAW_DEPTH).
    ///
    /// This function takes the width and height of a depth map, along with a vector
    /// of depth values, and encodes this information into a binary blob. The binary
    /// format consists of the width and height encoded as 64-bit unsigned integers
    /// (big-endian format) followed by the depth values encoded as 16-bit unsigned
    /// integers (big-endian format). This format is suitable for serialization and
    /// transmission of depth map data through gRPC.
    ///
    /// @param width The width of the depth map in pixels.
    /// @param height The height of the depth map in pixels.
    /// @param depth_values A vector containing the depth values for each pixel in the depth
    /// map.
    ///                     Each depth value is a 16-bit unsigned integer.
    /// @return A std::vector<unsigned char> representing the encoded binary data of the depth
    /// map.
    ///         The vector includes 8 bytes for width, 8 bytes for height, followed by 2 bytes
    ///         per depth value.
    /// @throws Exception: if the depth data values do not correspond to height and width.
    ///
    static std::vector<unsigned char> encode_depth_map(const Camera::depth_map& m);

    /// Decode image data of custom MIME type FORMAT_RAW_DEPTH into a depth_map structure.
    ///
    /// This function processes a binary blob representing a depth map in a specific
    /// format and extracts the dimensions and depth values contained within.
    ///
    /// @param data A vector of unsigned chars representing the binary data of the depth map.
    /// @return A depth_map struct containing the width, height, and depth values extracted from the
    /// data.
    /// @throws Exception: if the data is too short to contain valid depth information or
    ///         if the data size does not match the expected size based on the width and height.
    static Camera::depth_map decode_depth_map(const std::vector<unsigned char>& data);

    /// @brief remove any extra suffix's from the mime type string.
    static std::string normalize_mime_type(const std::string& str);

    /// @brief convert a protobuf format enum with a MIME type string.
    static std::string format_to_MIME_string(viam::component::camera::v1::Format format);

    /// @brief convert a MIME type string with a protobuf format enum.
    static viam::component::camera::v1::Format MIME_string_to_format(
        const std::string& mime_string);

    /// @brief Creates a `raw_image` struct from its proto representation.
    static raw_image from_proto(const viam::component::camera::v1::GetImageResponse& proto);

    /// @brief Creates a `image_collection` struct from its proto representation.
    static image_collection from_proto(const viam::component::camera::v1::GetImagesResponse& proto);

    /// @brief Creates a `point_cloud` struct from its proto representation.
    static point_cloud from_proto(const viam::component::camera::v1::GetPointCloudResponse& proto);

    /// @brief creates an `intrinsic_parameters` struct from its proto representation.
    static intrinsic_parameters from_proto(
        const viam::component::camera::v1::IntrinsicParameters& proto);

    /// @brief creats a `distortion_parameters` struct from its proto representation.
    static distortion_parameters from_proto(
        const viam::component::camera::v1::DistortionParameters& proto);

    /// @brief creates a `properties` struct from its proto representation.
    static properties from_proto(const viam::component::camera::v1::GetPropertiesResponse& proto);

    /// @brief converts a `distortion_parameters` struct to its proto representation.
    static viam::component::camera::v1::DistortionParameters to_proto(const distortion_parameters&);

    /// @brief converts an `intrinsic_parameters` struct to its proto representation.
    static viam::component::camera::v1::IntrinsicParameters to_proto(const intrinsic_parameters&);

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

    /// @brief Get the next image from the camera as a raw image.
    /// @param mime_type the desired mime_type of the image (does not guarantee output type).
    /// @return The frame as a `raw_image`.
    inline raw_image get_image(std::string mime_type) {
        return get_image(mime_type, {});
    }

    /// @brief Get the next image from the camera as a raw image.
    /// @param mime_type the desired mime_type of the image (does not guarantee output type).
    /// @param extra any additional arguments to the method.
    /// @return The frame as a `raw_image`.
    virtual raw_image get_image(std::string mime_type, const AttributeMap& extra) = 0;

    /// @brief Get the next images from the camera as a vector of raw images with names and
    /// metadata.
    /// @return a vector of raw_images and associated response metadata.
    virtual image_collection get_images() = 0;

    /// @brief Get the next `point_cloud` from the camera.
    /// @param mime_type the desired mime_type of the point_cloud (does not guarantee output type).
    /// @return The requested `point_cloud`.
    inline point_cloud get_point_cloud(std::string mime_type) {
        return get_point_cloud(mime_type, {});
    }

    /// @brief Get the next `point_cloud` from the camera.
    /// @param mime_type the desired mime_type of the point_cloud (does not guarantee output type).
    /// @param extra any additional arguments to the method.
    /// @return The requested `point_cloud`.
    virtual point_cloud get_point_cloud(std::string mime_type, const AttributeMap& extra) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling camera.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling camera.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) = 0;

    /// @brief Get the camera's properties.
    /// @return The camera properties.
    virtual properties get_properties() = 0;

    API api() const override;

   protected:
    explicit Camera(std::string name);
};

template <>
struct API::traits<Camera> {
    static API api();
};

bool operator==(const Camera::raw_image& lhs, const Camera::raw_image& rhs);
bool operator==(const Camera::image_collection& lhs, const Camera::image_collection& rhs);
bool operator==(const Camera::point_cloud& lhs, const Camera::point_cloud& rhs);
bool operator==(const Camera::intrinsic_parameters& lhs, const Camera::intrinsic_parameters& rhs);
bool operator==(const Camera::distortion_parameters& lhs, const Camera::distortion_parameters& rhs);
bool operator==(const Camera::properties& lhs, const Camera::properties& rhs);

}  // namespace sdk
}  // namespace viam
