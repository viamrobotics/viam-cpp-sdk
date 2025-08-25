#pragma once

#include <viam/sdk/components/camera.hpp>

namespace viam {
namespace sdktests {
namespace camera {

using namespace viam::sdk;

class MockCamera : public Camera {
   public:
    ProtoStruct do_command(const ProtoStruct& command) override;
    raw_image get_image(std::string mime_type, const sdk::ProtoStruct& extra) override;
    image_collection get_images(std::vector<std::string> filter_source_names,
                                const sdk::ProtoStruct& extra) override;
    point_cloud get_point_cloud(std::string mime_type, const sdk::ProtoStruct& extra) override;
    std::vector<GeometryConfig> get_geometries(const sdk::ProtoStruct& extra) override;
    properties get_properties() override;
    static std::shared_ptr<MockCamera> get_mock_camera();
    MockCamera(std::string name) : Camera(std::move(name)) {}

    const std::vector<std::string>& last_filter_source_names() const {
        return last_filter_source_names_;
    }
    const ProtoStruct& last_extra() const {
        return last_extra_;
    }

   private:
    Camera::intrinsic_parameters intrinsic_parameters_;
    Camera::distortion_parameters distortion_parameters_;
    Camera::properties camera_properties_;
    Camera::raw_image image_;
    Camera::image_collection images_;
    Camera::point_cloud pc_;
    ProtoStruct map_;
    std::vector<GeometryConfig> geometries_;
    std::vector<std::string> last_filter_source_names_;
    ProtoStruct last_extra_;
};

Camera::raw_image fake_raw_image();
Camera::image_collection fake_raw_images();
Camera::point_cloud fake_point_cloud();
Camera::intrinsic_parameters fake_intrinsic_parameters();
Camera::distortion_parameters fake_distortion_parameters();
Camera::properties fake_properties();

}  // namespace camera
}  // namespace sdktests
}  // namespace viam
