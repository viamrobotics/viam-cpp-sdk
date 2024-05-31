/// @file components/camera.hpp
///
/// @brief Defines a `Camera` component.
#pragma once

#include <bitset>
#include <chrono>
#include <string>
#include <vector>

#include <boost/endian/conversion.hpp>
#include <xtensor/xarray.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

/// @class Camera camera.hpp "components/camera.hpp"
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

    /// @brief The supported mime types of the camera— a type alias.
    using mime_types = std::vector<std::string>;

    /// @struct properties
    /// @brief The camera's supported features and settings.
    ///
    /// `supports_pcd` indicates whether the camera has a valid implementation of
    /// `get_point_cloud`.
    /// `intrinsic_parameters` contains the camera's intrinsic parameters.
    /// `distortion_parameters` contains the camera's distortion parameters.
    /// `mime_types` contains the mime types the camera supports.
    struct properties {
        bool supports_pcd;
        struct intrinsic_parameters intrinsic_parameters;
        struct distortion_parameters distortion_parameters;
        Camera::mime_types mime_types;
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
    /// depth_map holds the height and width data of a depth map, along with the depth values
    /// as a 2D xtensor array of 16-bit unsigned integers, where the first axis is height, and
    /// the second axis is width. Each depth value represents the distance from the camera
    /// to a point in the scene.
    using depth_map = xt::xarray<uint16_t>;

    /// Encodes the dimensions and depth values of a depth map into a raw binary format
    /// (MIME type FORMAT_RAW_DEPTH).
    ///
    /// This function takes a depth_map, and encodes this information into a binary blob. The binary
    /// format consists of "magic number" header (UTF-8 encoding for 'DEPTHMAP' in big-endian),
    /// then the width and height encoded as 64-bit unsigned integers, followed by the depth
    /// values encoded as 16-bit unsigned integers (big-endian format). This format is suitable
    /// for serialization and transmission of depth map data through gRPC.
    ///
    /// @param depth_map A type alias for a 2D xtensor array
    /// @return A std::vector<unsigned char> representing the encoded binary data of the depth
    /// map.
    ///         The vector includes 8 bytes for width, 8 bytes for height, followed by 2 bytes
    ///         per depth value.
    /// @throws Exception: if the depth map is not 2D
    ///
    static std::vector<unsigned char> encode_depth_map(const Camera::depth_map& m);

    /// Decode image data of custom MIME type FORMAT_RAW_DEPTH into a depth_map structure.
    ///
    /// This function processes a binary blob representing a depth map in a specific
    /// format and extracts the dimensions and depth values contained within.
    ///
    /// @param data A vector of unsigned chars representing the binary data of the depth map.
    /// @return A depth_map (type alias for a 2D xtensor array)
    /// @throws Exception: if the data is misformatted e.g. doesn't contain valid depth information,
    ///         or if the data size does not match the expected size based on the width and height.
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
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

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
