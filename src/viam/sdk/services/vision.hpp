/// @file services/vision.hpp
///
/// @brief Defines a `Vision` service.
#pragma once

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/camera.hpp>
#include <viam/sdk/services/service.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

/// @class Vision vision.hpp "services/vision.hpp"
/// @brief A vision service that runs detectors / classifiers / segmenters.
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific vision implementations. This class cannot be used on its own.
class Vision : public Service {
   public:
    API api() const override;

    /// @struct detection
    /// @brief Result of a single object detection.
    /// Pixel bbox fields are optional int64; normalized bbox fields are optional double.
    struct detection {
        boost::optional<std::int64_t> x_min;
        boost::optional<std::int64_t> y_min;
        boost::optional<std::int64_t> x_max;
        boost::optional<std::int64_t> y_max;
        boost::optional<double> x_min_normalized;
        boost::optional<double> y_min_normalized;
        boost::optional<double> x_max_normalized;
        boost::optional<double> y_max_normalized;
        std::string class_name;
        double confidence = 0.0;
    };

    /// @struct classification
    /// @brief Result of a single image classification.
    struct classification {
        std::string class_name;
        double confidence = 0.0;
    };

    using point_cloud = Camera::point_cloud;
    using raw_image = Camera::raw_image;

    /// @struct image
    /// @brief Image input to `get_detections` / `get_classifications`.
    ///
    /// Distinct from `raw_image` (a.k.a. `Camera::raw_image`) because the Vision request
    /// messages on the wire only carry bytes + mime_type — they do not carry a `source_name`.
    /// See the proto definitions for `GetDetectionsRequest` and `GetClassificationsRequest`.
    /// The `raw_image` alias remains for `capture_all_result.image`, whose wire type is the
    /// full Camera Image proto and does carry a source name.
    struct image {
        std::string mime_type;
        std::vector<unsigned char> bytes;
    };

    /// @struct point_cloud_object
    /// @brief A point cloud and its associated geometry.
    /// @note The proto field is named `point_cloud`; we use `cloud` here to avoid shadowing
    /// the `point_cloud` type alias (GCC rejects the shadowing under -fpermissive).
    struct point_cloud_object {
        point_cloud cloud;
        std::vector<GeometryConfig> geometries;
    };

    /// @struct properties
    /// @brief Describes the vision service's supported capabilities.
    struct properties {
        bool classifications_supported = false;
        bool detections_supported = false;
        bool object_point_clouds_supported = false;
    };

    /// @struct capture_options
    /// @brief Controls which data types are returned by `capture_all_from_camera`.
    struct capture_options {
        bool return_image = false;
        bool return_detections = false;
        bool return_classifications = false;
        bool return_object_point_clouds = false;
    };

    /// @struct capture_all_result
    /// @brief Aggregate result returned by `capture_all_from_camera`.
    struct capture_all_result {
        boost::optional<raw_image> image;
        std::vector<detection> detections;
        std::vector<classification> classifications;
        std::vector<point_cloud_object> objects;
        ProtoStruct extra;
    };

    /// @brief Get detections from a named camera's next image.
    inline std::vector<detection> get_detections_from_camera(const std::string& camera_name) {
        return get_detections_from_camera(camera_name, {});
    }

    /// @brief Get detections from a named camera's next image.
    /// @param extra Any additional arguments to the method.
    virtual std::vector<detection> get_detections_from_camera(const std::string& camera_name,
                                                              const ProtoStruct& extra) = 0;

    /// @brief Get detections from an image.
    inline std::vector<detection> get_detections(const image& img) {
        return get_detections(img, {});
    }

    /// @brief Get detections from an image.
    /// @param extra Any additional arguments to the method.
    virtual std::vector<detection> get_detections(const image& img, const ProtoStruct& extra) = 0;

    /// @brief Get classifications from a named camera's next image.
    /// @param count The number of classifications to return.
    inline std::vector<classification> get_classifications_from_camera(
        const std::string& camera_name, int count) {
        return get_classifications_from_camera(camera_name, count, {});
    }

    /// @brief Get classifications from a named camera's next image.
    /// @param count The number of classifications to return.
    /// @param extra Any additional arguments to the method.
    virtual std::vector<classification> get_classifications_from_camera(
        const std::string& camera_name, int count, const ProtoStruct& extra) = 0;

    /// @brief Get classifications from an image.
    /// @param count The number of classifications to return.
    inline std::vector<classification> get_classifications(const image& img, int count) {
        return get_classifications(img, count, {});
    }

    /// @brief Get classifications from an image.
    /// @param count The number of classifications to return.
    /// @param extra Any additional arguments to the method.
    virtual std::vector<classification> get_classifications(const image& img,
                                                            int count,
                                                            const ProtoStruct& extra) = 0;

    /// @brief Get point cloud objects detected by a named camera.
    /// @param mime_type The desired mime type of the point cloud (does not guarantee output type).
    inline std::vector<point_cloud_object> get_object_point_clouds(const std::string& camera_name,
                                                                   const std::string& mime_type) {
        return get_object_point_clouds(camera_name, mime_type, {});
    }

    /// @brief Get point cloud objects detected by a named camera.
    /// @param mime_type The desired mime type of the point cloud (does not guarantee output type).
    /// @param extra Any additional arguments to the method.
    virtual std::vector<point_cloud_object> get_object_point_clouds(const std::string& camera_name,
                                                                    const std::string& mime_type,
                                                                    const ProtoStruct& extra) = 0;

    /// @brief Get the properties (capabilities) of this vision service.
    inline struct properties get_properties() {
        return get_properties({});
    }

    /// @brief Get the properties (capabilities) of this vision service.
    /// @param extra Any additional arguments to the method.
    virtual struct properties get_properties(const ProtoStruct& extra) = 0;

    /// @brief Capture image, detections, classifications, and/or point clouds in a single call.
    /// @param camera_name The name of the camera to capture from.
    /// @param opts Controls which data types to include in the result.
    inline capture_all_result capture_all_from_camera(const std::string& camera_name,
                                                      const capture_options& opts) {
        return capture_all_from_camera(camera_name, opts, {});
    }

    /// @brief Capture image, detections, classifications, and/or point clouds in a single call.
    /// @param camera_name The name of the camera to capture from.
    /// @param opts Controls which data types to include in the result.
    /// @param extra Any additional arguments to the method.
    virtual capture_all_result capture_all_from_camera(const std::string& camera_name,
                                                       const capture_options& opts,
                                                       const ProtoStruct& extra) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param command The command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    /// @brief Get the status of the vision service.
    /// @return A `ProtoStruct` containing the status of the vision service.
    virtual ProtoStruct get_status() = 0;

   protected:
    explicit Vision(std::string name);
};

template <>
struct API::traits<Vision> {
    static API api();
};

bool operator==(const Vision::detection& lhs, const Vision::detection& rhs);
bool operator==(const Vision::classification& lhs, const Vision::classification& rhs);
bool operator==(const Vision::point_cloud_object& lhs, const Vision::point_cloud_object& rhs);
bool operator==(const Vision::properties& lhs, const Vision::properties& rhs);

}  // namespace sdk
}  // namespace viam
