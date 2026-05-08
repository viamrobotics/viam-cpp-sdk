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
    /// @note The field name `point_cloud` in `point_cloud_object` shadows the type alias
    /// `point_cloud` — this is intentional and matches the proto field layout.
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
        double confidence;
    };

    /// @struct classification
    /// @brief Result of a single image classification.
    struct classification {
        std::string class_name;
        double confidence;
    };

    using point_cloud = Camera::point_cloud;
    using raw_image = Camera::raw_image;

    /// @struct point_cloud_object
    /// @brief A point cloud and its associated geometry.
    /// @note The field name `point_cloud` shadows the type alias `point_cloud` — intentional,
    /// matching the proto field name.
    struct point_cloud_object {
        point_cloud point_cloud;
        std::vector<GeometryConfig> geometries;
    };

    /// @struct properties
    /// @brief Describes the vision service's supported capabilities.
    struct properties {
        bool classifications_supported;
        bool detections_supported;
        bool object_point_clouds_supported;
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

    /// @brief Get detections from a raw image.
    inline std::vector<detection> get_detections(const raw_image& image) {
        return get_detections(image, {});
    }

    /// @brief Get detections from a raw image.
    /// @param extra Any additional arguments to the method.
    virtual std::vector<detection> get_detections(const raw_image& image,
                                                  const ProtoStruct& extra) = 0;

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

    /// @brief Get classifications from a raw image.
    /// @param count The number of classifications to return.
    inline std::vector<classification> get_classifications(const raw_image& image, int count) {
        return get_classifications(image, count, {});
    }

    /// @brief Get classifications from a raw image.
    /// @param count The number of classifications to return.
    /// @param extra Any additional arguments to the method.
    virtual std::vector<classification> get_classifications(const raw_image& image,
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
