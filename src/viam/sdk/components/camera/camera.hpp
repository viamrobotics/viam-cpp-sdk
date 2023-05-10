/// @file components/camera/camera.hpp
///
/// @brief Defines a `Camera` component.
#pragma once

#include <bitset>
#include <string>
#include <vector>

#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

/// @defgroup Camera Classes related to the `Camera` component.

/// @class CameraRegistration
/// @brief Defines a `ResourceRegistration` for the `Camera` component.
/// @ingroup Camera
class CameraRegistration : public ResourceRegistration {
   public:
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
    CameraRegistration(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceRegistration(service_descriptor){};
};

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
    /// @brief the raw bytes and mime type of an image.
    // TODO: update documentaiton to show how to deserialize a `raw_image`
    struct raw_image {
        std::string mime_type;
        std::vector<unsigned char> bytes;
    };

    /// @brief Creates a `ResourceRegistration` for the `Camera` component.
    static std::shared_ptr<ResourceRegistration> resource_registration();

    /// @brief Creates a `Camera` `API`.
    static API static_api();

    /// @brief Creates a `raw_image` struct from its proto representation.
    static raw_image from_proto(viam::component::camera::v1::GetImageResponse proto);

    /// @brief Creates a `point_cloud` struct from its proto representation.
    static point_cloud from_proto(viam::component::camera::v1::GetPointCloudResponse proto);

    /// @brief creates an `intrinsic_parameters` struct from its proto representation.
    static intrinsic_parameters from_proto(viam::component::camera::v1::IntrinsicParameters proto);

    /// @brief creats a `distortion_parameters` struct from its proto representation.
    static distortion_parameters from_proto(
        viam::component::camera::v1::DistortionParameters proto);

    /// @brief creates a `properties` struct from its proto representation.
    static properties from_proto(viam::component::camera::v1::GetPropertiesResponse proto);

    /// @brief converts a `distortion_parameters` struct to its proto representation.
    static viam::component::camera::v1::DistortionParameters to_proto(distortion_parameters);

    /// @brief converts an `intrinsic_parameters` struct to its proto representation.
    static viam::component::camera::v1::IntrinsicParameters to_proto(intrinsic_parameters);

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

    /// @brief Get the next image from the camera as a raw image.
    /// @param mime_type the desired mime_type of the image (does not guarantee output type).
    /// @return The frame as a `raw_image`.
    virtual raw_image get_image(std::string mime_type) = 0;

    /// @brief Get the next `point_cloud` from the camera.
    /// @param mime_type the desired mime_type of the point_cloud (does not guarantee output type).
    /// @return The requested `point_cloud`.
    virtual point_cloud get_point_cloud(std::string mime_type) = 0;

    /// @brief Get the camera's properties.
    /// @return The camera properties.
    virtual properties get_properties() = 0;

    API dynamic_api() const override;

   protected:
    explicit Camera(std::string name) : Component(std::move(name)){};
};

bool operator==(const Camera::raw_image& lhs, const Camera::raw_image& rhs);
bool operator==(const Camera::point_cloud& lhs, const Camera::point_cloud& rhs);
bool operator==(const Camera::intrinsic_parameters& lhs, const Camera::intrinsic_parameters& rhs);
bool operator==(const Camera::distortion_parameters& lhs, const Camera::distortion_parameters& rhs);
bool operator==(const Camera::properties& lhs, const Camera::properties& rhs);

}  // namespace sdk
}  // namespace viam
